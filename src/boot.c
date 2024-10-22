#define ALIGNMENT 1 << 0           /* align loaded modules on page boundaries */
#define MEM_INFO 1 << 1            /* beyond scope */
#define FLAGS ALIGNMENT | MEM_INFO /* beyond scope */
#define MAGIC 0x1badb002           /* 'magic number' lets bootloader find the header */
#define CHECKSUM -((MAGIC) + (FLAGS))

#include "i386.h"
#include "mboot.h"
#include "main.c"
#include "vga.h"
#include <string.h>
#include <string.h>
#include <stdio.h>
#include "./strings.h"
#include "./paging.h"
#include "./utils.h"
#include "./gdt.h"
#include "./debug.h"
#include "pic.h"
#include "interrupt.h"
#include "mem.h"
#include "heap.h"
//#include <math.h>

__attribute__((section(".rodata.multiboot")))
__attribute__((aligned(4)))
const struct multiboot_header header = {
    .checksum = CHECKSUM,
    .flags = FLAGS,
    .magic = MAGIC};

__attribute__((section(".boot.bss")))
__attribute__((aligned(16))) char bootstack[4096];

__attribute__((section(".boot.text")))
__attribute__((naked)) int
_start()
{

    __asm__(
        "movl %0, %%esp\n"
        "movl %%esp, %%ebp\n" ::
            "r"(((unsigned int)bootstack) + sizeof(bootstack)));

    asm(
        "jmp _start2");
}

__attribute__((section(".boot.bss")))
__attribute__((aligned(4096)))
page_directory_entry_t boot_page_directory[1024];

__attribute__((section(".boot.text"))) void enable_boot_paging();

__attribute__((section(".boot.text"))) int _start2()
{
    enable_boot_paging();
    while (1)
    {
        asm volatile("hlt");
    }
}

__attribute__((section(".boot.data"))) extern char __kernel_higher_half_stop;


#define PAGE_SIZE 1024 * 4 * 1024

__attribute__((section(".boot.text"))) void enable_boot_paging()
{
    uint32_t boot_payload;

    asm volatile("mov %%ebx, %0\n":"=r" (boot_payload));

    unsigned int higher_half = 0xC0000000;

    unsigned int counter = higher_half;
    unsigned int higher_half_end = (unsigned int)&__kernel_higher_half_stop;

    unsigned int low_counter = 0;

    while (counter < higher_half_end)
    {
        linear_address_4m_t lad = *((linear_address_4m_t *)&counter);
        linear_address_4m_t low_lad = *((linear_address_4m_t *)&low_counter);

        page_directory_entry_4m_t page = pde4m_boot_zero();
        page.is_page = 1;
        page.sepervisor = 1;
        page.present = 1;
        page.read_write = 1;


        page_directory_entry_4m_t identity = pde4m_boot_zero();
        identity.is_page = 1;
        identity.sepervisor = 1;
        identity.present = 1;
        identity.read_write = 1;

        unsigned int *pagecast = (unsigned int *)&page;
        *pagecast |= low_counter;

        unsigned int *low_pagecast = (unsigned int *)&identity;
        *low_pagecast |= low_counter;

        boot_page_directory[lad.page_directory_entry] = (page_directory_entry_t)page;
        boot_page_directory[low_lad.page_directory_entry] = (page_directory_entry_t)identity;

        counter += PAGE_SIZE;
        low_counter += PAGE_SIZE;
    }

    cr4_t cr4 = read_cr4_boot();
    cr4.page_size_extensions = 1;
    write_cr4_boot(cr4);

    cr3_t cr3 = read_cr3_boot();
    unsigned int *cr3cast = (unsigned int *)&cr3;
    *cr3cast |= (unsigned int)&boot_page_directory;
    write_cr3_boot(cr3);

    cr0_t cr0 = read_cr0_boot();
    cr0.paging = 1;
    write_cr0_boot(cr0);

    asm volatile("mov %0, %%ebx\n"::"r" (boot_payload));

    asm volatile("jmp higher_half");

    while (1)
    {
    }
}

__attribute__((aligned(16))) char kernel_stack[16384] = {0};

__attribute__((naked)) void higher_half()
{
    __asm__(
        "movl %0, %%esp\n"
        "movl %%esp, %%ebp\n" ::
            "r"(((unsigned int)kernel_stack) + sizeof(kernel_stack)));

    asm(
        "jmp main");
    while (1)
    {
        /* code */
    }
}

void main()
{
    multiboot_info_t *boot_payload;
    asm volatile("mov %%ebx, %0\n":"=r" (boot_payload));
    vga_init(0xb8000 + 0xC0000000);
    gdt_init(((uint32_t)kernel_stack) + sizeof(kernel_stack));
    pic_remap();

    uint32_t total_memory = boot_payload->mem_lower + boot_payload->mem_upper;

    interrupts_init_isrs();

    physical_mem_init(total_memory);

    map_kernel();

    vga_clear_screen(black);


    heap_init();
    
    //page_free(block, 0);

    mem_stats_t stats = mem_stats();

    vga_printf("TOTAL: %u USED: %u\n", stats.total, stats.used);


    panic("kanguura\n");

    while (1)
    {
        /* code */
    }
    
}
