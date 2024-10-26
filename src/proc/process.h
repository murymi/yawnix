#pragma once
#include <stdint.h>
#include "paging.h"

typedef struct process_t process_t;
struct process_t{
    uint32_t user_stack_ptr;
    uint32_t stack_ptr;
    uint32_t kernel_stack;
    uint32_t kernel;
    process_t *next;
    page_directory_entry_4k_t *page_directory;
    pagelist_t *page_tables;
    pagelist_t *pages;
};

void process_init(process_t *proc, uint32_t eip, uint32_t kernel);
uint32_t process_allocate_page(process_t *proc, uint32_t address);
uint32_t process_allocate_page_handle(process_t *proc, uint32_t address);
