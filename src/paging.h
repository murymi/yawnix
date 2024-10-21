#pragma once

#include "i386.h"

#define BLOCK_SIZE 4096
//#define XBLOCK_SIZE 4096


//void clear_page_drectory();
void map_kernel();
uint32_t page_alloc_kernel_random();
uint32_t page_alloc_kernel_specific_physical(uint32_t physical_block);
void page_free(uint32_t page_ptr, int free_physical);
uint32_t page_alloc_kernel_contigious(uint32_t size);
