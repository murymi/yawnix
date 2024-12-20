#include "./paging.h"
#include "./utils.h"
#include "./drivers/vga.h"
#include "interrupt.h"
#include "debug.h"
#include "bitset.h"
#include "mem.h"
#include "mutex.h"
#include "sched.h"
#include "process.h"

extern char __kernel_higher_half_start;
extern char __kernel_higher_half_stop;



uint32_t virtual_bitset[MAX_PAGES/32];
mutex_t virtual_mutex;

uint32_t gpe_handler(cpu_context_t *context) {
    error_code_t code = *((error_code_t *)&context->error_code);

    panic("general protection violation\nTABLE %u [%u]\nDESC LOCATION: %u\nEXTERNAL: %u\n",
    code.table_indexed,
    code.index,
    code.descriptor_location,
    code.reserved
    );
    return 0;
}



static void print_pte(page_table_entry_t *pde) {
    vga_printf("___________________________PTE\n");
    vga_printf("R/W: %b, SUPER: %b, PRESENT: %b, PAGE BASE: %x, ISNULL: %u\n", 
    pde->read_write, pde->user, pde->present, pte_get_page_base_address(pde), pte_is_null(pde));
    vga_printf("___________________________END_PTE\n");
} 

static void print_pde(process_t *proc, page_directory_entry_4k_t *pde) {
    uint32_t addr = read_cr2();
    linear_address_4k_t lad = *((linear_address_4k_t *)&addr);


    uint32_t page_table = pde4k_get_page_table_base_address(pde);
    vga_printf("___________________________PDE\n");
    vga_printf("R/W: %b, SUPER: %b, PRESENT: %b, PT BASE: %x, ISNULL: %u\n", 
    pde->read_write, pde->user, pde->present, page_table, pde_is_null(pde));
    vga_printf("___________________________END_PDE\n");

    if(page_table){
        pagelist_t *pl = pagelist_search_page(proc->page_tables, page_table);
        page_table_entry_t *page_table_virt = (page_table_entry_t *)pl->page;
        vga_printf("FAULT PT: %x\n", page_table_virt);
        print_pte(&page_table_virt[lad.page_table_entry]);
    }
} 

uint32_t page_fault_handler(cpu_context_t *context) {
    page_fault_error_code_t code = *((page_fault_error_code_t *)&context->error_code);
    cr3_t cr3 = read_cr3();

    //code.p

    uint32_t addr = read_cr2();
    linear_address_4k_t lad = *((linear_address_4k_t *)&addr);

    process_t *current = shed_get_current_proc();

    print_pde(current, &current->page_directory[lad.page_directory_entry]);
    //print_pde(&current->page_directory)
    panic("---PAGE FAULT---\n ADDRESS: %x\n FROM USER: %u\n BY WRITE: %u\nBY VIOLATION: %b\n CR3: %x\nPD: %x\n",
     addr, code.from_user_mode, code.caused_by_write,code.p, cr3_get_page_directory_base(&cr3),
     current->page_directory
     );

    return 0;
}

__attribute__((aligned(4096)))
page_directory_entry_4k_t page_directory[1024];

__attribute__((aligned(4096)))
page_table_entry_t page_table[1024][1024];

uint32_t paging_kernel_page_dir(){
    return (uint32_t)page_directory;
}


static void clear_page_drectory(){
    for(int i = 0; i < 1024; i++) {
        page_directory[i] = pde4k_zero();
        page_directory[i].present = 1;
        page_directory[i].user = 1;
        page_directory[i].read_write = 1;
        pde4k_set_page_table_base_address(&page_directory[i], ((uint32_t)&page_table[i]) - 0xC0000000);
        //vga_printf("woa: %x, woest: %x\n",
        //pde4k_get_page_table_base_address(&page_directory[i]),
        // ((uint32_t)&page_table[i]) - 0xC0000000);
        // break;
    }

    for(int i = 0; i < 1024; i++) {
        for(int j = 0; j < 1024; j++) {
            page_table[i][j] = pte_zero();
        }
    }

    for(uint32_t i = 0; i < (MAX_PAGES/32); i++) {
        virtual_bitset[i] = 0;
    }
}

static void recover_page_tables() {
    uint32_t kernel_top = align_forward((uint32_t) &__kernel_higher_half_stop, BLOCK_SIZE)/BLOCK_SIZE;
    uint32_t kernel_base = align_backward((uint32_t)&__kernel_higher_half_start, BLOCK_SIZE)/BLOCK_SIZE;

    uint32_t first_kernel_page = 0xC0000000;
    linear_address_4k_t lad = *((linear_address_4k_t *)&first_kernel_page);

    for(int i = 0; i < lad.page_directory_entry; i++) {
        page_free((uint32_t)&page_table[i], 1);
    }

    if(kernel_base >= first_kernel_page/BLOCK_SIZE) {
        assert(bitset_isset(virtual_bitset,kernel_base), "Kenel page should be set");
    }
    if(kernel_top >= first_kernel_page/BLOCK_SIZE) {
        assert(bitset_isset(virtual_bitset,kernel_top-1), "Kenel page should be set");
    }

}

void paging_init() {
    clear_page_drectory();
    uint32_t kernel_base = align_backward((uint32_t)&__kernel_higher_half_start, BLOCK_SIZE);
    uint32_t kernel_top = align_forward((uint32_t) &__kernel_higher_half_stop, BLOCK_SIZE);

    for(uint32_t i = kernel_base; i < kernel_top; i += BLOCK_SIZE) {
        linear_address_4k_t lad = *((linear_address_4k_t *)&i);
        page_table_entry_t pte = pte_zero();
        pte.present = 1;
        pte.user = 1;
        pte.read_write = 1;
        pte_set_page_base_address(&pte, i - 0xC0000000);
        page_table[lad.page_directory_entry][lad.page_table_entry] = pte;

        uint32_t page_index = i/BLOCK_SIZE;

        assert(!bitset_isset(virtual_bitset, page_index), "page must be free");
        bitset_set(virtual_bitset, page_index);
    }

    for(uint32_t i = kernel_base; i < kernel_top; i += BLOCK_SIZE) {
        uint32_t page_index = i/BLOCK_SIZE;
        assert(bitset_isset(virtual_bitset, page_index), "alloc kernel page went wierld");
    }

    // NULL PTR
    bitset_set(virtual_bitset, 0);
    assert(bitset_isset(virtual_bitset, 0), "alloc null page went wierld");

    {
        // for vga
        uint32_t buf = 0xC0000000 + 0xb8000;
        linear_address_4k_t lad = *((linear_address_4k_t *)&buf);
        page_table_entry_t pte = pte_zero();
        pte.present = 1;
        pte.user = 1;
        pte.read_write = 1;
        pte_set_page_base_address(&pte, 0xb8000);
        page_table[lad.page_directory_entry][lad.page_table_entry] = pte;

        bitset_set(virtual_bitset, buf/BLOCK_SIZE);
        assert(bitset_isset(virtual_bitset, buf/BLOCK_SIZE), "alloc vga page went wierld");
    }

    virtual_mutex = mutex_init();
    recover_page_tables();

    cr3_t cr3 = read_cr3();
    cr3_set_page_directory_base(&cr3, ((uint32_t)&page_directory) - 0xC0000000);
    write_cr3(cr3);

    cr3_t new_cr3 = read_cr3();

    assert(*((uint32_t*)&new_cr3) == (uint32_t)&page_directory - 0xC0000000, "page directory set error");

    interrupt_handler_register(XPAGEFAULT, page_fault_handler);
    interrupt_handler_register(XGENERAL_PROTECTION, gpe_handler);

}

static uint32_t page_alloc_kernel_inner() {
    uint32_t first_kernel_page = 0xC0000000/BLOCK_SIZE;
    for(uint32_t i = first_kernel_page; i < MAX_PAGES; i++) {
        if(!bitset_isset(virtual_bitset, i)) {
            bitset_set(virtual_bitset, i);
            return i * BLOCK_SIZE;
        }
    }
    return 0;
}

uint32_t page_alloc_kernel_random(int kernel) {
    mutex_lock(&virtual_mutex);
    uint32_t physical_block = physical_alloc_block();
    assert(physical_block, "Physical memory alloc error");

    uint32_t virtual_block = page_alloc_kernel_inner();
    assert(virtual_block, "virtual memory alloc error");

    linear_address_4k_t lad = *((linear_address_4k_t *)&virtual_block);
    page_table_entry_t pte = pte_zero();
    pte.present = 1;
    if(kernel) {
        pte.user = 1;
    }
    pte.read_write = 1;
    pte_set_page_base_address(&pte, physical_block);
    page_table[lad.page_directory_entry][lad.page_table_entry] = pte;
    assert(pte_get_page_base_address(&pte) == physical_block, "base must be eq to physical");
    mutex_unlock(&virtual_mutex);
    return virtual_block;
}

/// @brief physical block must be unallocated
/// @param physical_block 
/// @return 
uint32_t page_alloc_kernel_specific_physical(uint32_t physical_block, int kernel, int alloc_phys) {
    mutex_lock(&virtual_mutex);
    assert((physical_block%BLOCK_SIZE) == 0, "physical block must be aligned");
    assert(physical_block, "Physical block must be unNULL");

    if(alloc_phys) {
        uint32_t physical_block2 = physical_alloc_block_specific(physical_block);
        if(block_is_not_free(physical_block)) {
            assert(physical_block2 == physical_block, "failed to allocate block (not free)");
        }
        assert(physical_block2 == physical_block, "blocks should be same");
    }

    uint32_t virtual_block = page_alloc_kernel_inner();
    assert(virtual_block, "virtual memory alloc error");

    linear_address_4k_t lad = *((linear_address_4k_t *)&virtual_block);
    page_table_entry_t pte = pte_zero();
    pte.present = 1;
    if(kernel) {
        pte.user = 1;
    }
    pte.read_write = 1;
    pte_set_page_base_address(&pte, physical_block);
    page_table[lad.page_directory_entry][lad.page_table_entry] = pte;
    assert(pte_get_page_base_address(&pte) == physical_block, "base must be eq to physical");
    mutex_unlock(&virtual_mutex);
    return virtual_block;
}

uint32_t page_kernel_virtual_t0_pyhsical(uint32_t page_ptr) {
    assert((page_ptr%BLOCK_SIZE) == 0, "page block must be aligned");
    assert(page_ptr, "page block must be unNULL");
    uint32_t block_idx = page_ptr/BLOCK_SIZE;
    assert(bitset_isset(virtual_bitset, block_idx), "attempt to get physical of unmapped\n");
    linear_address_4k_t lad = *((linear_address_4k_t *)&page_ptr);
    page_table_entry_t entry = page_table[lad.page_directory_entry][lad.page_table_entry];
    assert(*((uint32_t *)&entry), "PTE must be unNULL");
    return pte_get_page_base_address(&entry);
}

void page_free(uint32_t page_ptr, int free_physical) {
    mutex_lock(&virtual_mutex);
    assert((page_ptr%BLOCK_SIZE) == 0, "page block must be aligned");
    assert(page_ptr, "page block must be unNULL");
    uint32_t block_idx = page_ptr/BLOCK_SIZE;
    assert(bitset_isset(virtual_bitset, block_idx), "Page double free detected\n");
    linear_address_4k_t lad = *((linear_address_4k_t *)&page_ptr);

    if(free_physical){
        uint8_t *ptr = (uint8_t *)page_ptr;
        for(int i = 0; i < 4096; i++) {
            ptr[i] = 0;
        }
        page_table_entry_t entry = page_table[lad.page_directory_entry][lad.page_table_entry];
        assert(*((uint32_t *)&entry), "PTE must be unNULL");
        uint32_t physical_addr = pte_get_page_base_address(&entry);
        physical_free_block(physical_addr);
    }
    page_table[lad.page_directory_entry][lad.page_table_entry] = pte_zero();
    invalidate_page(page_ptr);
    bitset_clear(virtual_bitset, block_idx);
    mutex_unlock(&virtual_mutex);
}


uint32_t page_alloc_kernel_specific_virtual(uint32_t virtual_block, int lock, int kernel) {
    if(lock) mutex_lock(&virtual_mutex);
    uint32_t physical_block = physical_alloc_block();
    assert(physical_block, "Physical memory alloc error");

    assert(virtual_block, "virtual memory alloc error");
    assert(virtual_block%BLOCK_SIZE == 0, "virtual block must be aligned");
    uint32_t block_index = virtual_block/BLOCK_SIZE;
    assert(!bitset_isset(virtual_bitset, block_index), "Block must be free");

    linear_address_4k_t lad = *((linear_address_4k_t *)&virtual_block);
    page_table_entry_t pte = pte_zero();
    pte.present = 1;
    if(kernel) {
        pte.user = 1;
    }
    pte.read_write = 1;
    pte_set_page_base_address(&pte, physical_block);
    page_table[lad.page_directory_entry][lad.page_table_entry] = pte;
    assert(pte_get_page_base_address(&pte) == physical_block, "base must be eq to physical");
    bitset_set(virtual_bitset, block_index);
    if(lock) mutex_unlock(&virtual_mutex);
    return virtual_block;
}

uint32_t page_alloc_kernel_contigious(uint32_t size, int kernel) {
    assert(size > 0, "page allocation size must be > zero");
    mutex_lock(&virtual_mutex);
    uint32_t aligned_size  = align_forward(size, BLOCK_SIZE);
    uint32_t npages = aligned_size/BLOCK_SIZE;
    uint32_t first_page = 0;
    uint32_t first_kernel_page = 0xC0000000/BLOCK_SIZE;
    uint32_t counter = 0;
    for(uint32_t i = first_kernel_page; i < MAX_PAGES && counter < npages; i++) {
        if(bitset_isset(virtual_bitset, i)) {
            counter = 0; 
        } else {
            if(counter == 0) {
                first_page = i;
            }
            counter += 1;
        }
    }
    if(counter != npages) { 
        mutex_unlock(&virtual_mutex);
        return 0;
    }
    assert(first_page != 0, "first Page must not be null");

    for(uint32_t i = 0; i < npages; i++) {
        assert(!bitset_isset(virtual_bitset, first_page + i), "Contigious allocation failed because blocks are not free");
    }

    for(uint32_t i = 0; i < npages; i++) {
        page_alloc_kernel_specific_virtual((first_page + i) * BLOCK_SIZE, 0, kernel);
    }

    for(uint32_t i = 0; i < npages; i++) {
        assert(bitset_isset(virtual_bitset, first_page + i), "Contigious allocation failed");
    }
    mutex_unlock(&virtual_mutex);
    return first_page * BLOCK_SIZE;
}

void zero_page_table(uint32_t *page) {
    for(int i = 0; i < 1024; i++) {
        page[i] = 0;
    }
}