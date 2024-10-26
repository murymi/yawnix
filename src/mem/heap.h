#pragma once
#include "paging.h"

typedef struct heap_t heap_t;
struct heap_t
{
    int64_t block_count;
};

typedef struct block_t block_t;
struct block_t
{
    uint32_t size;
    unsigned char free:1;
    uint32_t magic:31;
    block_t *next;
    block_t *pevious;
};

typedef struct
{
    uint32_t magic;
} block_footer_t;


#define HEAP_SIZE 1024 * 1024 * 4//4mb

#define block_shift(block) ((void *)(block)) + (sizeof(block_t))
#define heap_shift(heap) ((void *)(heap)) + (sizeof(heap_t))

#define block_ushift(block) ((void *)(block)) - (sizeof(block_t))
#define heap_ushift(heap) ((void *)(heap)) - (sizeof(heap_t))


void heap_init();
void *heap_alloc(uint32_t size);
void *heap_realloc(void *ptr, uint32_t new_size);
void heap_free(void *ptr);
