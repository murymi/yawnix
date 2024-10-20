#pragma once

#include "i386.h"

#define stub_with_error_code(idx)                   \
    __attribute__((naked)) static void stub_##idx() \
    {                                               \
        asm volatile(                               \
            "push $" #idx "\n"                      \
            "jmp interrupt_common_stub\n");         \
    }

#define stub_without_error_code(idx)                \
    __attribute__((naked)) static void stub_##idx() \
    {                                               \
        asm volatile(                               \
            "pushl $0\n"                            \
            "push $" #idx "\n"                      \
            "jmp interrupt_common_stub\n");         \
    }

__attribute__((naked)) void interrupt_common_stub();

unsigned int interrupt_handler(cpu_context_t *context);

void interrupts_init_isrs();

typedef uint32_t (*interrupt_routine)(cpu_context_t *);

void interrupt_handler_register(uint32_t index, interrupt_routine handler);

#define XDIVIDE_ERROR 0
#define XBREAKPOINT 3
#define XDOUBLE_FAULT 8
#define XINVALID_TSS 10
#define XSEGMENT_NOT_PRESENT 12
#define XSTACK_SEGMENT_FAULT 12
#define XGENERAL_PROTECTION 13
#define XPAGEFAULT 14
