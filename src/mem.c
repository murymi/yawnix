#include "mem.h"
#include "vga.h"
#include "utils.h"
#include "debug.h"

uint32_t bitset[MAX_PAGES/32] = {0};

extern char __kernel_higher_half_start;
extern char __kernel_higher_half_stop;


/// @brief 
/// @param total_mem mem in kb
void physical_mem_init(uint32_t total_mem) {
    uint32_t total_bytes = total_mem * 1024;
    uint64_t high_offset = align_backward(total_bytes, 4096);
    bitsset_init(bitset, total_bytes);
    assert(bitset_isset(bitset, high_offset/BLOCK_SIZE), "unavailable mem not set");
    bitset_set(bitset, 0);
    assert(bitset_isset(bitset, 0), "null mem not set");

    uint32_t kphysical_start = align_backward((uint32_t)&__kernel_higher_half_start, BLOCK_SIZE) - 0xC0000000;
    uint32_t kphysical_stop = align_forward((uint32_t)&__kernel_higher_half_stop, BLOCK_SIZE) - 0xC0000000;

    for(uint32_t i = kphysical_start; i <= kphysical_stop; i += BLOCK_SIZE) {
        bitset_set(bitset, i/BLOCK_SIZE);
    }

    for(uint32_t i = kphysical_start; i <= kphysical_stop; i += BLOCK_SIZE) {
        assert(bitset_isset(bitset, i/BLOCK_SIZE), "alloc kernel blocks failed");
    }

    assert(!bitset_isset(bitset, kphysical_start/BLOCK_SIZE-1), "alloc block failed");
    assert(!bitset_isset(bitset, kphysical_stop/BLOCK_SIZE + 1), "alloc block failed");
}


uint32_t physical_alloc_block() {
    for(uint32_t i = 0; i < MAX_PAGES; i++) {
        if(!bitset_isset(bitset, i)) {
            bitset_set(bitset, i);
            return i * BLOCK_SIZE;
        }
    }
    return 0;
}

uint32_t physical_alloc_block_specific(uint32_t block_ptr) {
    assert((block_ptr % BLOCK_SIZE) == 0, "attempt to alloc unaligned block");
    uint32_t block_index = block_ptr/BLOCK_SIZE;
    assert(block_index < MAX_PAGES, "attempt to alloc out of bound block");
    if(!bitset_isset(bitset, block_index)) {
        bitset_set(bitset, block_index);
        assert((block_index*BLOCK_SIZE) == block_ptr, "skill issue detected alloc block spec");
        return block_ptr;
    }
    return 0;
}

void physical_free_block(uint32_t block_ptr) {
    assert((block_ptr % BLOCK_SIZE) == 0, "attempt to free unaligned block");
    uint32_t block_index = block_ptr/BLOCK_SIZE;
    assert(block_index < MAX_PAGES, "attempt to free out of bound block");
    assert(bitset_isset(bitset, block_index), "block Double free detected");
    bitset_clear(bitset, block_index);
}