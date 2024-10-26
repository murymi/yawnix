#pragma once
#include "i386.h"

void gdt_init();
void tss_set_stack0(uint32_t stack);
