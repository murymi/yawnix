#include "sched.h"
#include "interrupt.h"
#include "vga.h"
#include "pic.h"
#include "debug.h"
#include "mem.h"
#include "gdt.h"

process_t parent_process;

process_t *current_process = 0;
uint32_t first_round = 1;

static uint32_t timer_handler(cpu_context_t *context) {
    if(first_round) {
        first_round = 0;
        current_process = &parent_process;
        return (uint32_t) current_process->stack_ptr;
    }

    assert((uint32_t)current_process, "No running process");

    current_process->stack_ptr = (uint32_t)context;

    shed_pick_next();

    if(!current_process->kernel) {
        tss_set_stack0(current_process->kernel_stack);
        cpu_context_t *ctx = (cpu_context_t *)current_process->stack_ptr;
    }
    return (uint32_t) current_process->stack_ptr;
}

static void parent_sleep() {
    vga_printf("I am first process...\n");
    while (1)
    {
        asm volatile("hlt");
    }
    
}


process_t *shed_get_current_proc() {
    return current_process;
}

void sched_init() {

    interrupt_handler_register(32, timer_handler);
    process_init(&parent_process, parent_sleep, 1);
    current_process = &parent_process;
}

void shed_pick_next() {
    if(current_process->next){
        current_process = current_process->next;
    } else {
        current_process = &parent_process;
    }
}

void sched_append_process(process_t *proc) {
    if(!parent_process.next){
        parent_process.next = proc;
        proc->next = 0;
        return;
    }

    process_t *tmp = parent_process.next;
    while (tmp->next) tmp = tmp->next;
    
    tmp->next = proc;
    proc->next = 0;
}














