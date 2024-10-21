#include "heap.h"
#include "debug.h"

heap_t *heap = 0;


void heap_init()
{
    heap = 0;
    heap = (heap_t *)page_alloc_kernel_contigious(HEAP_SIZE);
    assert((uint32_t)heap > 0, "heap init failed");
    heap->block_count = 0;
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


static block_t *find_free_fitting_block(uint32_t size)
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

static block_t *find_last_block() {
    block_t *tmp = heap_shift(heap);
    while (tmp->next) tmp = tmp->next;
    assert((uint32_t)tmp, "last block != Null");
    return tmp;
}

static void *append_new_block(uint32_t size) {
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

static void split_into_two(block_t *block, uint32_t size) {
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

static void *split_existing_block(block_t *block, uint32_t size) {
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
    }
    return tmp;
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
                for(uint32_t i = 0; i < block->size && i < new_size; i++) {
                    newptr[i] = oldptr[i];
                }
                heap_free(oldptr);
                return newptr;
            }
        }
        tmp = tmp->next;
    }
    assert(0, "realloc invalid pointer detected");
    return 0;
}