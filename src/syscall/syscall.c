#include "syscall.h"
#include "debug.h"
#include "interrupt.h"

uint32_t syscall_handler(cpu_context_t *context) {
    vga_printf("syscall\n");
    vga_printf("%s\n", context->eax);
    return (uint32_t)context;
}

void syscall_init() {
    interrupt_handler_register(0x80, syscall_handler);
}