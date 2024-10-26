#pragma once

#include "i386.h"
#include "heap.h"
#include "process.h"
#include "paging.h"

void sched_init();
void shed_pick_next();
void sched_append_process(process_t *proc);
process_t *shed_get_current_proc();
