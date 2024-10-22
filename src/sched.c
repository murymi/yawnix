#include "sched.h"
#include "interrupt.h"
#include "vga.h"
#include "pic.h"
#include "debug.h"

process_t parent_process;

process_t *current_process = 0;
uint32_t first_round = 1;

static uint32_t timer_handler(cpu_context_t *context) {
    if(first_round) {
        first_round = 0;
        current_process = &parent_process;
        return (uint32_t) current_process->stack_ptr;
    }

    if(current_process) {
        current_process->stack_ptr = (uint32_t)context;
    }

    shed_pick_next();
    return (uint32_t) current_process->stack_ptr;
}

static void parent_sleep() {
    vga_printf("I am first process...\n");
    while (1)
    {
        asm volatile("hlt");
    }
    
}

void sched_init(uint32_t stack_ptr) {
    interrupt_handler_register(32, timer_handler);
    process_init(&parent_process, stack_ptr, parent_sleep);
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

static uint32_t save_context(uint32_t *stack, cpu_context_t *context) {
    uint32_t array_len = sizeof(cpu_context_t)/4;
    uint32_t *context_array = (uint32_t*)context;

    stack -= array_len;

    context->esp = stack;
    context->ebp = stack;

    for(int i = 0; i < (sizeof(cpu_context_t)/4); i++) {
        *stack = context_array[i];
        stack += 1;
    }

}

void process_init(process_t *proc, char *stack, uint32_t eip) {
    cpu_context_t context;

    eflags_t eflags = eflags_read();
    eflags.cf = 0;
    eflags.of = 0;
    eflags.pf = 0;
    eflags.zf = 0;

    context.eflags = eflags;
    context.cr3 = read_cr3();
    context.eip = eip;


    segment_selector_t data = (segment_selector_t){.index = 2, .requested_privilege_level = 0, .table_indicator = 0};
    segment_selector_t code = (segment_selector_t){.index = 1, .requested_privilege_level = 0, .table_indicator = 0};
    context.cs.selector = code;
    context.fs.selector =
    context.gs.selector =
    context.es.selector =
    context.ds.selector = data;

    save_context(stack, &context);

    proc->next = 0;
    proc->stack_ptr = context.esp;
}
