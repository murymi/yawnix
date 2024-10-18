ENTRY(_start)

__higher_half_start = 0xC0000000;
__kernel_higher_half_position = 0xC0100000;

SECTIONS {

    . = 1M;

    __kernel_start = .;

    .rodata.multiboot : {
		*(.rodata.multiboot)
	}

    .boot.text : {
        *(.boot.text)
    }

    .boot.rodata : {
        *(.boot.rodata)
    }

    .boot.data : {
        *(.boot.data)
    }

    .boot.bss : {
        *(.boot.bss)
    }
    
   . += __higher_half_start;
    __kernel_higher_half_start = .;

    .text ALIGN (4K)
    : 
    AT (ADDR(.text) - __higher_half_start)
    {
		__kernel_text_start = .;
        *(.text)
		__kernel_text_stop = .;
    }


    .rodata ALIGN(4K) : 
    AT (ADDR(.rodata) - __higher_half_start)
    {
		__kernel_rodata_start = .;
        *(.rodata)
		__kernel_rodata_stop = .;
    }
 
    .data ALIGN(4K) : 
    AT (ADDR(.data) - __higher_half_start)
    {
		__kernel_data_start = .;
        *(.data)
		__kernel_data_stop = .;
    }
 
    .bss ALIGN(4K) :
    AT (ADDR(.bss) - __higher_half_start)
     {
		__kernel_bss_start = .;
        *(COMMON)
        *(.bss)
		__kernel_bss_stop = .;

    }

    .bss.stack ALIGN(4K) : 
    AT (ADDR(.bss.stack) - __higher_half_start)
    {
		__kernel_stack_start = .;
		KEEP(*(.bss.stack))
		__kernel_stack_stop = .;
	}

    __kernel_higher_half_stop = .;
    __kernel_stop = . - __higher_half_start;
}
