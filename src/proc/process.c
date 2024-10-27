#include "process.h"
#include "debug.h"
#include "heap.h"
#include "mem.h"

static pagelist_t *pagelist_init(uint32_t page, uint32_t physical) {
    pagelist_t *pl = heap_alloc(sizeof(pagelist_t));
    assert((uint32_t)pl, "failed to allocate a page list node");
    pl->next = 0;
    pl->page = page;
    pl->physical = physical;
    return pl;
}

static void append_page_table(process_t *proc, uint32_t page, uint32_t physical) {
    if(proc->page_tables == 0) {
        proc->page_tables = pagelist_init(page, physical);
        return;
    }
    pagelist_t *tmp = proc->page_tables;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = pagelist_init(page, physical);
}


static void append_page(process_t *proc, uint32_t page, uint32_t physical) {
    if(proc->pages == 0) {
        proc->pages = pagelist_init(page, physical);
        return;
    }
    pagelist_t *tmp = proc->pages;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = pagelist_init(page, physical);
}



pagelist_t *pagelist_search_page(pagelist_t *list, uint32_t page_phys) {
    assert((page_phys%BLOCK_SIZE) == 0, "Searching unaligned page");
    if(!list) return 0;
    pagelist_t *tmp = list;
    while (list->next)
    {
        if(tmp->physical == page_phys) return tmp;
        tmp = tmp->next;
    }
    if(tmp == list) {
        if(tmp->physical == page_phys) return tmp;
    }
    return 0;
}

static uint32_t save_context(uint32_t *stack, cpu_context_t *context) {
    uint32_t array_len = sizeof(cpu_context_t)/4;
    uint32_t *context_array = (uint32_t*)context;

    stack -= array_len;

    uint32_t result = (uint32_t)stack;

    for(uint32_t i = 0; i < (sizeof(cpu_context_t)/4); i++) {
        *stack = context_array[i];
        stack += 1;
    }

    return result;
}


// return physical address
uint32_t process_allocate_page(process_t *proc, uint32_t address) {
   //assert(process->page_tables)
   assert((uint32_t)proc, "Process is null");
   assert(!proc->kernel, "Must be user process");
   assert(address, "Attempt to map null page");
    assert((address%BLOCK_SIZE) == 0, "allocate unaligned page");
    assert((uint32_t)proc->page_directory, "user process Page directory is null");
    linear_address_4k_t lad = *((linear_address_4k_t *)&address);
    assert(lad.page_directory_entry < 768, "Attempt to overwrite kernel addresses");

    page_directory_entry_4k_t *target_pde = &proc->page_directory[lad.page_directory_entry];


    if(pde_is_null(target_pde)) {
        target_pde->present = 1;
        target_pde->user = 1;
        target_pde->read_write = 1;

        uint32_t page_table = page_alloc_kernel_random(1);
        assert(page_table, "Failed to allocate page table");

        uint32_t page_table_phys = 
            page_kernel_virtual_t0_pyhsical(page_table);

        pde4k_set_page_table_base_address(
            target_pde,
            page_table_phys
        );

        append_page_table(proc, page_table, page_table_phys);
        zero_page_table((uint32_t *)page_table);
    }

    assert(!pde_is_null(target_pde), "PDE still null");

    uint32_t tptp = pde4k_get_page_table_base_address(target_pde);

    pagelist_t *tl = pagelist_search_page(proc->page_tables, tptp);
    assert((uint32_t)tl, "Page table not foundi");


    page_table_entry_t *page_table = (page_table_entry_t *)tl->page;

    page_table_entry_t *tp = &page_table[lad.page_table_entry];

    assert(!tp->present, "page already mapped");

    uint32_t pblock = physical_alloc_block();
    assert(pblock, "Failed to allocate pyhsical block");

    *tp = pte_zero();
    tp->present = 1;
    tp->user = 1;
    tp->read_write = 1;
    
    append_page(proc, address, pblock);
    pte_set_page_base_address(tp, pblock);
    return pblock;
}

// returns the page mapped on kernel addr space
uint32_t process_allocate_page_handle(process_t *proc, uint32_t address) {
    uint32_t pblock = process_allocate_page(proc, address);
    uint32_t handle = page_alloc_kernel_specific_physical(pblock, 1, 0);
    assert(handle, "Failed to map user pt to kernl");
    return handle;
}

static cr3_t proc_allocate_pagedir(process_t *proc) {
        page_directory_entry_4k_t *proc_page_dir = (page_directory_entry_4k_t *)page_alloc_kernel_random(0);
        assert((uint32_t)proc_page_dir, "Failed to create page directory");

        uint32_t proc_page_dir_phys = page_kernel_virtual_t0_pyhsical((uint32_t)proc_page_dir);

        cr3_t cr3 = read_cr3();
        uint32_t kernel_offset = 0xC0000000;
        //linear_address_4k_t lad = *((linear_address_4k_t *)&kernel_offset);
        page_directory_entry_4k_t *kernel_page_dir = (page_directory_entry_4k_t *)(cr3_get_page_directory_base(&cr3) + kernel_offset);
        //uint32_t count = 0;

        uint32_t got_kpd = paging_kernel_page_dir();

        assert((uint32_t)kernel_page_dir == got_kpd, "Wrong kernel page directory");
        proc->page_directory = proc_page_dir;
        for(int i = 0; i < 1024; i++) {
                proc_page_dir[i] = pde4k_zero();
        }

        vga_printf("USER PAGE DIR PHYS: %x, VIRT: %x\n", proc_page_dir_phys, proc_page_dir);
        cr3_set_page_directory_base(&cr3, proc_page_dir_phys);

        for(int i = 768; i < 1024; i++) {
            proc_page_dir[i] = kernel_page_dir[i];
        }

        return cr3;
}

char *message = "hello from user\n";
__attribute__((aligned(4096)))
static void test_user() {
    asm volatile("mov %0, %%eax"::"m"(message));
    asm volatile("int $0x80");
    while (1)
    {
    }
}


void process_init(process_t *proc, uint32_t eip, uint32_t kernel) {
    uint32_t stack = page_alloc_kernel_contigious(4096, 1);
    assert(stack, "Failed to alloc new stack");
    stack += 4095;

    cpu_context_t context;

    proc->page_tables = 0;
    proc->pages = 0;

    eflags_t eflags = eflags_read();
    eflags.cf = 0;
    eflags.of = 0;
    eflags.pf = 0;
    eflags.zf = 0;
    context.eflags = eflags;

    segment_selector_t data;
    segment_selector_t code;

    if(kernel) {
        proc->kernel = 1;
        context.cr3 = read_cr3();
        data = (segment_selector_t){.index = 2, .requested_privilege_level = 0, .table_indicator = 0};
        code = (segment_selector_t){.index = 1, .requested_privilege_level = 0, .table_indicator = 0};
    } else {
        proc->kernel = 0;
        context.cr3 = proc_allocate_pagedir(proc);
        code = (segment_selector_t){.index = 3, .requested_privilege_level = 3, .table_indicator = 0};
        data = (segment_selector_t){.index = 4, .requested_privilege_level = 3, .table_indicator = 0};

        uint8_t *code_page = (uint8_t *)process_allocate_page_handle(proc, 4096);
        uint8_t *old_code = (uint8_t *)test_user;
        //vga_printf("----------------- %x\n", test_user);
        for(int i = 0; i < 4096; i++) {
            code_page[i] = old_code[i];
        }
        //code_page[0] = 0x55;
        //code_page[1] = 0x89;
        //code_page[2] = 0xe5;
        //code_page[0] = 0xeb;
        //code_page[1] = 0xfe;
        eip = (uint32_t)4096;

        page_free((uint32_t) code_page, 0);

        uint32_t user_stack = process_allocate_page(proc, 8192);
        assert(user_stack, "failed to alloc user stack");

        context.esp = 0;
        context.ebp = 0;
        context.user_esp = 8192 + 4095;
        context.user_ss.selector = (segment_selector_t){
            .index = 4,
            .requested_privilege_level = 3,
            .table_indicator = 0
        };
        proc->kernel_stack = (uint32_t)stack;

    }
    context.eip = eip;
    context.cs.selector = code;
    context.fs.selector =
    context.gs.selector =
    context.es.selector =
    context.ds.selector = data;
    proc->stack_ptr = save_context((uint32_t *)stack, &context);
    proc->next = 0;
}


//void process_destroy(process_t *proc) {
//    
//}
