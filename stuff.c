
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

extern void foo();

int main(int argc, char const *argv[])
{
    foo();
    return 0;
}
