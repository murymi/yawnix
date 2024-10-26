
#include "bitset.h"
#include "vga.h"
#include "debug.h"

uint32_t bitset_isset(uint32_t bitset[MAX_PAGES/32], uint32_t index){
    uint32_t box = index/32;
    assert(index < MAX_PAGES/32, "BITSET index out of bounds");
    uint32_t bit = index % 32;
    return (bitset[box] & (1 << bit)) > 0;
}

void bitset_set(uint32_t bitset[MAX_PAGES/32], uint32_t index){
    uint32_t box = index/32;
    assert(index < MAX_PAGES/32, "BITSET index out of bounds");
    uint32_t bit = index % 32;
    bitset[box] = bitset[box] | (1 << bit);
}

void bitset_clear(uint32_t bitset[MAX_PAGES/32], uint32_t index){
    uint32_t box = index/32;
    assert(index < MAX_PAGES/32, "BITSET index out of bounds");
    uint32_t bit = index % 32;
    bitset[box] = bitset[box] & ~(1 << bit);
}

void bitsset_init(uint32_t bitset[MAX_PAGES/32], uint64_t available_mem) {
    uint32_t high_offset = align_backward(available_mem, BLOCK_SIZE)/BLOCK_SIZE;
    assert(high_offset < MAX_PAGES, "too large memory");
    //if(high_offset > (MAX_PAGES)) high_offset = (MAX_PAGES);
    for(uint32_t i = 0; i < high_offset; i++) {
        bitset_clear(bitset, i);
    }
    for(uint32_t i = high_offset; i < (MAX_PAGES); i++){
        bitset_set(bitset,i);
    }
}

