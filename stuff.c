#include "src/i386.h"
#include <stdio.h>
#include <stdint.h>
#include<stdarg.h>

void printx(const char *fmt, __gnuc_va_list args) {
    char *a = va_arg(args, char *);
    puts(a);
    //vprintf(fmt, args);
}

void printj(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);

    printx(fmt, vl);

    va_end(vl);
}

int main(int argc, char const *argv[])
{
    printj("%s %s %s %c\n", "hi", "nugu", "kumbaf", 'c');
    return 0;
}
