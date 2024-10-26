#pragma once
#include "drivers/vga.h"
#include "stdarg.h"


static inline void panic(char *fmt,...) {
    va_list vl;
    va_start(vl, fmt);
    vga_vprintf(fmt, vl);
    while (1)
    {
        asm volatile("hlt");
    }
    
}

static inline void assert(int test, char *message) {
    if(!test){
        panic("%s\n", message);
    }
}