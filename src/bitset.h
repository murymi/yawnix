#pragma once
#include <stdint.h>
#include "utils.h"
#include "paging.h"

//#define MAX_PAGES (1024L * 1024L * 1024L * 4L)/4096L

#define MAX_PAGES UINT32_MAX/4096L


//#define BLOCK_SIZE 4096L

uint32_t bitset_isset(uint32_t bitset[MAX_PAGES/32], uint32_t index);

void bitset_set(uint32_t bitset[MAX_PAGES/32], uint32_t index);

void bitset_clear(uint32_t bitset[MAX_PAGES/32], uint32_t index);

void bitsset_init(uint32_t bitset[MAX_PAGES/32], uint64_t available_mem);
