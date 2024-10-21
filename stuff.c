#include "src/i386.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

// #include <assert.h>

#define HEAP_SIZE 4096 * 1024 * 4

void assert(uint32_t y, char *message)
{
    if (y)
    {
        return;
    }
    puts(message);
    raise(SIGSEGV);
}

void *alloc_page(size_t size)
{
    void *page = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (page == (void *)-1)
    {
        printf("page alloc failed\n");
        return 0;
    }
    return page;
}

typedef struct heap_t heap_t;
struct heap_t
{
    // int64_t unused;
    int64_t block_count;
    //heap_t *next;
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
    /* data */
} block_footer_t;


heap_t *heap;

// block_t *init_block(char *buf) {
//     block_t *block = (block_t *)buf;
// }

#define block_shift(block) ((void *)(block)) + (sizeof(block_t))
#define heap_shift(heap) ((void *)(heap)) + (sizeof(heap_t))

#define block_ushift(block) ((void *)(block)) - (sizeof(block_t))
#define heap_ushift(heap) ((void *)(heap)) - (sizeof(heap_t))

void heap_init()
{
    heap = 0;
    heap = (heap_t *)alloc_page(HEAP_SIZE);
    // page_alloc_kernel_contigious(1024 * 1024 * 4);
    assert((uint32_t)heap > 0, "heap init failed");
    heap->block_count = 0;
    //heap->next = 0;
    // heap->unused = (4096) - sizeof(heap_t);
}

static int can_fit(uint32_t size)
{
    return ((HEAP_SIZE) - sizeof(heap_t)) >= (size + sizeof(block_t));
}

static void merge_block_right(block_t *block)
{
    if (block->next)
    {
        if (block->next->free)
        {
            block_t *block_next_next = block->next->next;
            if(block_next_next) {
                block_next_next->pevious = block;
            }
            block->size += (block->next->size + sizeof(block_t));
            heap->block_count -= 1;
            block->next = block_next_next;
            assert(heap->block_count >= 0, "block overflow");
        }
    }
}

static void merge_block_left(block_t *block)
{
    if (block->pevious)
    {
        if (block->pevious->free)
        {
            block->pevious->size += (block->size + sizeof(block_t));
            block->pevious->next = block->next;
            if(block->next) {
                block->next->pevious = block->pevious;
            }
            heap->block_count -= 1;
            assert(heap->block_count >= 0, "block overflow");
        }
    }
}


block_t *find_free_fitting_block(size_t size)
{
    block_t *tmp = heap_shift(heap);
    while (tmp)
    {
        if (tmp->free && tmp->size >= size)
        {
            return tmp;
        };
        tmp = tmp->next;
    }
    return 0;
}

block_t *find_last_block() {
    block_t *tmp = heap_shift(heap);
    while (tmp->next) tmp = tmp->next;
    assert((uint32_t)tmp, "last block != Null");
    return tmp;
}

void *append_new_block(uint32_t size) {
        block_t *last_block = find_last_block();
        block_t *new_block = block_shift(last_block) + last_block->size;
        uint8_t *last_heap_ptr = &((uint8_t *)heap)[HEAP_SIZE-1];
        int64_t remaining = last_heap_ptr - ((uint8_t *)block_shift(new_block));

        if(!(remaining >= size)) {
            return 0;
            //assert(0, "Heap space finished");
        }

        new_block->pevious = last_block;
        new_block->free = 0;
        new_block->next = 0;
        new_block->size = size;
        last_block->next = new_block;
        heap->block_count += 1;
        return block_shift(new_block);
}

void split_into_two(block_t *block, uint32_t size) {
    if(size == block->size || block->size < (size + (sizeof(block_t)))) {
        block->free = 0;
        return;
    }

    block_t *next_block = block->next;
    block_t *new_block = block_shift(block) + size;

    if(next_block && next_block->free) {
        assert(0, "two free neighbors detected");
    } else if(next_block) {
        block->free = 0;
        new_block->free = 1;

        new_block->next = next_block;
        next_block->pevious = new_block;

        block->next = new_block;
        new_block->pevious = block;

        new_block->size = block->size - (size + sizeof(block_t));

        block->size = size;
        heap->block_count += 1;
    } else {
        block->size = size;
        block->free = 0;
        block->next = 0;
        heap->block_count = 0;
    }
}

void *split_existing_block(block_t *block, uint32_t size) {
    assert(block->free, "block must be free");
    split_into_two(block, size);
    block->free = 0;
    return block_shift(block);
}

void *heap_alloc(uint32_t size)
{
    size += sizeof(block_footer_t);

    char *tmp;
    block_t *block;
    block_footer_t *footer;

    if (heap->block_count == 0)
    {
        if (can_fit(size))
        {
            block = heap_shift(heap);
            block->size = size;
            block->next = 0;
            block->pevious = 0;
            block->free = 0;
            heap->block_count += 1;
            tmp = block_shift(block);
            footer = (block_footer_t*)(tmp + (block->size - sizeof(block_footer_t)));
            footer->magic = 69;
            block->magic = 69;
            return tmp;
        }
        else
            return 0;
    }

    block_t *fitting_block = find_free_fitting_block(size);

    
    if(fitting_block) {
        tmp = split_existing_block(fitting_block, size);
    } else {
        tmp = append_new_block(size);
    }

    if(tmp) {
        block = block_ushift(tmp);
        footer = (block_footer_t*)(tmp + (block->size - sizeof(block_footer_t)));
        footer->magic = 69;
        block->magic = 69;
        return tmp;
    }
}

void heap_free(void *ptr)
{
    assert((uint32_t)ptr, "attempt free ptr null");
    block_t *block = block_ushift(ptr);
    block_t *tmp = heap_shift(heap);
    while (tmp)
    {
        if(tmp == block) {
            assert(!block->free, "double free detected");
            uint32_t magic = *((uint32_t*)(((char *)ptr) + (block->size - sizeof(block_footer_t))));
            assert(magic == block->magic, "heap corrupted");
            block->free = 1;
            merge_block_right(block);
            merge_block_left(block);
            return;
        }
        tmp = tmp->next;
    }
    assert(0, "freeing invalid pointer detected");
}

void *heap_realloc(void *ptr, uint32_t new_size) {
    block_t *tmp = heap_shift(heap);
    block_t *block = block_ushift(ptr);

    while (tmp)
    {
        if(tmp == block) {
            uint32_t magic = *((uint32_t*)(((char *)ptr) + (block->size - sizeof(block_footer_t))));
            assert(magic == block->magic, "heap corrupted");
            merge_block_right(block);
            if(block->size >= new_size) {
                split_into_two(block, new_size);
                return block_shift(block);
            } else {
                char *newptr = heap_alloc(new_size);
                char *oldptr = (char *)ptr;
                for(uint32_t i = 0; i < block->size, i < new_size; i++) {
                    newptr[i] = oldptr[i];
                }
                heap_free(oldptr);
                return newptr;
            }
        }
        tmp = tmp->next;
    }
    assert(0, "realloc invalid pointer detected");
}

void heap_debug() {
    block_t *tmp = heap_shift(heap);
    uint32_t i = 0;
    printf("BLOCKS: %lld\n", heap->block_count);
    while (tmp)
    {
        printf("BLOCK[%d] size: %d, free: %d\n", i, tmp->size, tmp->free);
        tmp = tmp->next;
        i += 1;
    }

    printf("debuged\n");
}




#include<time.h>
#include<stdlib.h>


void *pointers[100];
void *pointers2[100];


int maint(int argc, char const *argv[])
{
    // bitsset_init(bitset, 1024*1024*1024);

    sizeof(block_t);

    heap_init();

    char * a = heap_alloc(300);
    char * b = heap_alloc(300);
    a = heap_realloc(&a, 45);

    //a[45] = 0;

    //heap_free(a);

    heap_debug();
    

    
    //heap_free(ptr2);


    // block_t *b = heap_shift(heap);
    // b = heap_ushift(b);

    // printf("size: %ld\n",(uint32_t)b - (uint32_t)heap);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_ALLOCATIONS 1000
#define MAX_ALLOC_SIZE 1024
#define MAX_FUZZ_ITERATIONS 10000

typedef struct Allocation {
    void *ptr;
    size_t size;
} Allocation;

Allocation allocations[NUM_ALLOCATIONS];
int allocation_count = 0;



void random_fuzzer() {
    srand(time(NULL));

    for (int i = 0; i < MAX_FUZZ_ITERATIONS; i++) {
        int operation = rand() % 3; // 0 for malloc, 1 for free, 2 for reallocate
        if (operation == 0) {
            size_t size = rand() % MAX_ALLOC_SIZE + 1; // Random size between 1 and MAX_ALLOC_SIZE
            void *ptr = heap_alloc(size);
            if (ptr != NULL) {
                memset(ptr, 0, size); // Initialize memory
            }
        } else if (operation == 1 && allocation_count > 0) {
            int index = rand() % allocation_count;
            heap_free(allocations[index].ptr);
        } else if (operation == 2 && allocation_count > 0) {
            int index = rand() % allocation_count;
            size_t new_size = rand() % MAX_ALLOC_SIZE + 1;
            void *new_ptr = heap_realloc(allocations[index].ptr, new_size);
            if (new_ptr != NULL) {
                allocations[index].ptr = new_ptr;
                allocations[index].size = new_size;
                // Optionally initialize new memory region
                if (new_size > allocations[index].size) {
                    //memset((char *)new_ptr + allocations[index].size, 0, new_size - allocations[index].size);
                }
            } else {
                printf("Reallocation failed for pointer %p\n", allocations[index].ptr);
            }
        }

        // Introduce random sleeps to mimic real-world usage and increase randomness
        usleep(rand() % 1000); // Sleep for a random time (up to 1ms)
    }
}

int main() {
    heap_init();
    random_fuzzer();
    
    // Free any remaining allocations
    for (int i = 0; i < allocation_count; i++) {
        heap_free(allocations[i].ptr);
    }

    return 0;
}
