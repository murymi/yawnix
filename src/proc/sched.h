#pragma once

#include "i386.h"
#include "heap.h"
#include "process.h"
#include "paging.h"

void sched_init(uint32_t stack_ptr);
void shed_pick_next();
void process_init(process_t *proc, char *stack, uint32_t eip, uint32_t kernel);
void sched_append_process(process_t *proc);
process_t *shed_get_current_proc();
