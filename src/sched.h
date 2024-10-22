#pragma once

#include "i386.h"
#include "heap.h"

typedef struct process_t process_t;
struct process_t{
    //cpu_context_t context;
    uint32_t *stack_ptr;
    process_t *next;
} ;

void sched_init();
void shed_pick_next();
void process_init(process_t *proc, char *stack, uint32_t eip);
void sched_append_process(process_t *proc);
