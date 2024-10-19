#pragma once

#include "i386.h"

#define stub_with_error_code(idx)                   \
    __attribute__((naked)) static void stub_##idx() \
    {                                               \
        asm volatile(                               \
            "push $" #idx "\n"                      \
            "jmp interrupt_common_stub\n");                   \
    }

#define stub_without_error_code(idx)                \
    __attribute__((naked)) static void stub_##idx() \
    {                                               \
        asm volatile(                               \
            "pushl $0\n"                            \
            "push $" #idx "\n"                      \
            "jmp interrupt_common_stub\n");                   \
    }

__attribute__((naked)) void interrupt_common_stub();

unsigned int interrupt_handler(cpu_context_t *context);

void interrupts_init_isrs();