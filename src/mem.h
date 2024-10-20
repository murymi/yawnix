#pragma once
#include "bitset.h"


void physical_mem_init(uint32_t total_mem);
uint32_t physical_alloc_block();
void physical_free_block(uint32_t block_ptr);
uint32_t physical_alloc_block_specific(uint32_t block_ptr);
