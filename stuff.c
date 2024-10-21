#include "src/i386.h"
#include <stdio.h>
#include <stdint.h>
#include<stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>

void *alloc_page() {
    void *page = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(page == (void *)-1) {
        printf("page alloc failed\n");
        return 0;
    }
    return page;
}

typedef struct heap_t heap_t;
struct heap_t {
    uint32_t unused;
    uint32_t block_count;
    heap_t *next;
};

typedef struct block_t block_t;
struct block_t
{
    uint32_t size;
    block_t *next;
    block_t *pevious;
};

heap_t *heap;


heap_t *init_heap() {
    heap_t *page = (heap_t *)alloc_page();
    page->unused = 4096 - sizeof(heap_t);
    page->next = 0;
    page->block_count = 0;
    return page;
}

void *heap_alloc(heap_t *heap, size_t size) {
    if(heap->unused < (size + sizeof(block_t))) {

    } else return 0;
}

int main(int argc, char const *argv[])
{
    //bitsset_init(bitset, 1024*1024*1024);

    //sizeof(heap_t);


    heap = init_heap();

    printf("size: %ld\n",heap->used);
}
