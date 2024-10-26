#pragma once
#include <stdint.h>
#include "i386.h"

uint32_t syscall_handler(cpu_context_t *context);
void syscall_init();