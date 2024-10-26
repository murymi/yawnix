#pragma once

#include "i386.h"

#define BLOCK_SIZE 4096

typedef struct pagelist_t pagelist_t;
struct pagelist_t{
    uint32_t physical;
    uint32_t page;
    pagelist_t *next;
};


void paging_init();
uint32_t page_alloc_kernel_random(int kernel);
uint32_t page_alloc_kernel_specific_physical(uint32_t physical_block, int kernel, int alloc_phys);
void page_free(uint32_t page_ptr, int free_physical);
uint32_t page_alloc_kernel_contigious(uint32_t size, int kernel);
uint32_t page_kernel_virtual_t0_pyhsical(uint32_t page_ptr);
uint32_t paging_kernel_page_dir();
pagelist_t *pagelist_search_page(pagelist_t *list, uint32_t page_phys);

static inline int pde_is_null(page_directory_entry_4k_t *pde) {
    return *((uint32_t *)pde) == 0;
}

static inline int pte_is_null(page_table_entry_t *pde) {
    return *((uint32_t *)pde) == 0;
}

void zero_page_table(uint32_t *page);