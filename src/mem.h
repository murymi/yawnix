#pragma once
#include "bitset.h"

typedef struct
{
    uint32_t total;
    uint32_t used;
    /* data */
}mem_stats_t;


void physical_mem_init(uint32_t total_mem);
uint32_t physical_alloc_block();
void physical_free_block(uint32_t block_ptr);
uint32_t physical_alloc_block_specific(uint32_t block_ptr);
mem_stats_t mem_stats();
