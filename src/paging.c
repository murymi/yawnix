#include "./paging.h"
#include "./utils.h"
#include "./drivers/vga.h"

extern char __kernel_higher_half_start;
extern char __kernel_higher_half_stop;

extern char __kernel_start;
extern char __kernel_stop;




__attribute__((aligned(4096)))
page_directory_entry_4k_t page_directory[1024];

__attribute__((aligned(4096)))
page_table_entry_t page_table[1024][1024];


static void clear_page_drectory(){
    for(int i = 0; i < 1024; i++) {
        page_directory[i] = pde4k_zero();
        page_directory[i].present = 1;
        page_directory[i].sepervisor = 1;
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

    //uint32_t page_table_base = &page_table;
}

void map_kernel() {
    clear_page_drectory();

    uint32_t kernel_base = align_backward((uint32_t)&__kernel_higher_half_start, BLOCK_SIZE);
    uint32_t kernel_top = align_forward((uint32_t) &__kernel_higher_half_stop, BLOCK_SIZE);
    //uint32_t kernel_phybase = kernel_base - 0xC0000000;
    //align_forward((uint32_t) &__kernel_start, BLOCK_SIZE);


    //vga_printf("base 0x%x, top 0x%x\n", kernel_base, kernel_top);

    for(;kernel_base < kernel_top; kernel_base += BLOCK_SIZE) {
        linear_address_4k_t lad = *((linear_address_4k_t *)&kernel_base);
        page_table_entry_t pte = pte_zero();
        pte.present = 1;
        pte.sepervisor = 1;
        pte.read_write = 1;
        pte_set_page_base_address(&pte, kernel_base - 0xC0000000);
        page_table[lad.page_directory_entry][lad.page_table_entry] = pte;
        //vga_printf("pde: %u, pte: %u, offset: %u offc: %u\n", 
        //lad.page_directory_entry, lad.page_table_entry,
        // kernel_base - 0xC0000000,
        // pte_get_page_base_address(&page_table[lad.page_directory_entry][lad.page_table_entry])
        //);
        //break;
    }

    {
        // for vga
        uint32_t buf = 0xC0000000 + 0xb8000;
        linear_address_4k_t lad = *((linear_address_4k_t *)&buf);
        page_table_entry_t pte = pte_zero();
        pte.present = 1;
        pte.sepervisor = 1;
        pte.read_write = 1;
        pte_set_page_base_address(&pte, 0xb8000);
        page_table[lad.page_directory_entry][lad.page_table_entry] = pte;
    }

    cr3_t cr3 = read_cr3();
    cr3_set_page_directory_base(&cr3, ((uint32_t)&page_directory) - 0xC0000000);
    write_cr3(cr3);

    //vga_printf("base: %u dir base: %u\n", cr3_get_page_directory_base(&cr3), ((uint32_t)&page_directory) - 0xC0000000);
}