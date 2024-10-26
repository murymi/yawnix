#include "src/hashmap.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>


int main(int argc, char const *argv[])
{
    HashMap hm;

    hashmap_put(&hm, "hello", "world");

    return 0;
}
