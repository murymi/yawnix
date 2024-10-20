#include "src/i386.h"
#include <stdio.h>
#include <stdint.h>
#include<stdarg.h>

#include "/home/vic/Desktop/C/src/utils.h"


#define MAX_PAGES (1024L * 1024L * 1024L * 4L)/4096L
#define BLOCK_SIZE 4096L

uint32_t bitset[MAX_PAGES/32];

uint32_t bitset_isset(uint32_t bitset[MAX_PAGES/32], uint32_t index){
    uint32_t box = index/32;
    uint32_t bit = index % 32;
    return (bitset[box] & (1 << bit)) > 0;
}

void bitset_set(uint32_t bitset[MAX_PAGES/32], uint32_t index){
    uint32_t box = index/32;
    uint32_t bit = index % 32;
    bitset[box] = bitset[box] | (1 << bit);
}

void bitset_clear(uint32_t bitset[MAX_PAGES/32], uint32_t index){
    uint32_t box = index/32;
    uint32_t bit = index % 32;
    bitset[box] = bitset[box] & ~(1 << bit);
}

void bitsset_init(uint32_t bitset[MAX_PAGES/32], uint64_t available_mem) {
    uint64_t high_offset = align_backward(available_mem, BLOCK_SIZE)/BLOCK_SIZE;
    if(high_offset > (MAX_PAGES)) high_offset = (MAX_PAGES);
    for(int i = 0; i < high_offset; i++) {
        bitset_clear(bitset, i);
    }
    for(int i = high_offset; i < (MAX_PAGES); i++){
        bitset_set(bitset,i);
    } 
}


int main(int argc, char const *argv[])
{
    bitsset_init(bitset, 1024*1024*1024);

    printf("size: %d %d\n", bitset_isset(bitset, 50), bitset[50]);
}
