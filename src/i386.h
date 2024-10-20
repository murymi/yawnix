#pragma once

#include <stdint.h>
/* CR3 — Contains the physical address of the base of the page directory and two flags (PCD
and PWT). This register is also known as the page-directory base register (PDBR). Only
the most-significant bits (less the lower 12 bits) of the base address are specified; the lower
12 bits of the address are assumed to be 0. The page directory must thus be aligned to a
page (4-KByte) boundary. The PCD and PWT flags control caching of the page directory
in the processor’s internal data caches (they do not control TLB caching of page-directory
information).
When using the physical address extension, the CR3 register contains the base address of
the page-directory-pointer table In IA-32e mode, the CR3 register contains the base
address of the PML4 table.

See also: Section 3.8, “36-Bit Physical Addressing Using the PAE Paging Mechanism”.
*/
typedef struct
{
    unsigned int res0 : 3;
    /// Page-level Writes Transparent (bit 3 of CR3) — Controls the write-through or write-
    /// back caching policy of the current page directory. When the PWT flag is set, write-
    /// through caching is enabled; when the flag is clear, write-back caching is enabled. This
    /// flag affects only internal caches (both L1 and L2, when present). The processor ignores
    /// this flag if paging is not used (the PG flag in register CR0 is clear) or the CD (cache
    /// disable) flag in CR0 is set.
    /// See also: Section 10.5, “Cache Control” (for more information about the use of this
    /// flag) and Section 3.7.6, “Page-Directory and Page-Table Entries” (for a description of
    /// a companion PCD flag in the page-directory and page-table entries).
    unsigned int page_level_writes_transparent : 1;
    /// Page-level Cache Disable (bit 4 of CR3) — Controls caching of the current page
    /// directory. When the PCD flag is set, caching of the page-directory is prevented; when
    /// the flag is clear, the page-directory can be cached. This flag affects only the processor’s
    /// internal caches (both L1 and L2, when present). The processor ignores this flag if
    /// paging is not used (the PG flag in register CR0 is clear) or the CD (cache disable) flag
    /// in CR0 is set.
    /// See also: Chapter 10, Memory Cache Control (for more about the use of the PCD flag)
    /// and Section 3.7.6, “Page-Directory and Page-Table Entries” (for a description of a
    /// companion PCD flag in page-directory and page-table entries).
    unsigned int disable_page_level_cache : 1;
    unsigned int res1 : 7;

    unsigned int page_directory_base : 20;
}
__attribute__((packed))
__attribute__((aligned(4)))
cr3_t;

// inline cr3_t read_cr3();
__attribute__((section(".boot.text"))) static inline cr3_t read_cr3_boot()
{
    // alignof(unsigned int);
    cr3_t result;
    asm volatile(
        "mov %%cr3, %0"
        : "=r"(result));
    return result;
}

__attribute__((section(".boot.text"))) static inline void write_cr3_boot(cr3_t new)
{
    asm volatile(
        "mov %0, %%cr3"
        : : "r"(new));
}

static inline void write_cr3(cr3_t new)
{
    asm volatile(
        "mov %0, %%cr3"
        : : "r"(new));
}

static inline cr3_t read_cr3()
{
    cr3_t result;
    asm volatile(
        "mov %%cr3, %0"
        : "=r"(result));
    return result;
}

static inline void cr3_set_page_directory_base(cr3_t *cr3, uint32_t pd)
{
    unsigned int *cr3cast = (unsigned int *)cr3;
    cr3->page_directory_base = 0;
    *cr3cast |= pd;
}

static inline uint32_t cr3_get_page_directory_base(cr3_t *cr3)
{
    unsigned int *cr3cast = (unsigned int *)cr3;
    //*cr3cast |= pd;
    return (*cr3cast >> 12) << 12;
}

/*
CR0 — Contains system control flags that control operating mode and states of the
 processor.
*/
typedef struct
{
    /// Protection Enable (bit 0 of CR0) — Enables protected mode when set; enables real-
    /// address mode when clear. This flag does not enable paging directly. It only enables
    /// segment-level protection. To enable paging, both the PE and PG flags must be set.
    /// See also: Section 9.9, “Mode Switching”.
    unsigned int protection : 1;
    /// Monitor Coprocessor (bit 1 of CR0). — Controls the interaction of the WAIT (or
    /// FWAIT) instruction with the TS flag (bit 3 of CR0). If the MP flag is set, a WAIT
    /// instruction generates a device-not-available exception (#NM) if the TS flag is also set.
    /// If the MP flag is clear, the WAIT instruction ignores the setting of the TS flag. Table 9-2
    /// shows the recommended setting of this flag, depending on the IA-32 processor and x87
    /// FPU or math coprocessor present in the system. Table 2-1 shows the interaction of the
    /// MP, EM, and TS flags.
    unsigned int monitor_coprocessor : 1;
    /// Emulation (bit 2 of CR0) — Indicates that the processor does not have an internal or
    /// external x87 FPU when set; indicates an x87 FPU is present when clear. This flag also
    /// affects the execution of MMX/SSE/SSE2/SSE3 instructions.
    /// When the EM flag is set, execution of an x87 FPU instruction generates a device-not-
    /// available exception (#NM). This flag must be set when the processor does not have an
    /// internal x87 FPU or is not connected to an external math coprocessor. Setting this flag
    /// forces all floating-point instructions to be handled by software emulation. Table 9-2
    /// shows the recommended setting of this flag, depending on the IA-32 processor and x87
    /// FPU or math coprocessor present in the system. Table 2-1 shows the interaction of the
    /// EM, MP, and TS flags.
    /// Also, when the EM flag is set, execution of an MMX instruction causes an invalid-
    /// opcode exception (#UD) to be generated (see Table 11-1). Thus, if an IA-32 processor
    /// incorporates MMX technology, the EM flag must be set to 0 to enable execution of
    /// MMX instructions.
    /// Similarly for SSE/SSE2/SSE3 extensions, when the EM flag is set, execution of most
    /// SSE/SSE2/SSE3 instructions causes an invalid opcode exception (#UD) to be gener-
    /// ated (see Table 12-1). If an IA-32 processor incorporates the SSE/SSE2/SSE3 exten-
    /// sions, the EM flag must be set to 0 to enable execution of these extensions.
    /// SSE/SSE2/SSE3 instructions not affected by the EM flag include: PAUSE,
    /// PREFETCHh, SFENCE, LFENCE, MFENCE, MOVNTI, and CLFLUSH.
    unsigned int emulation : 1;
    /// Task Switched (bit 3 of CR0) — Allows the saving of the x87 FPU/MMX/SSE/SSE2/
    /// SSE3 context on a task switch to be delayed until an x87 FPU/MMX/SSE/SSE2/SSE3
    /// instruction is actually executed by the new task. The processor sets this flag on every
    /// task switch and tests it when executing x87 FPU/MMX/SSE/SSE2/SSE3 instructions.
    ///
    /// - If the TS flag is set and the EM flag (bit 2 of CR0) is clear, a device-not-available
    /// exception (#NM) is raised prior to the execution of any x87 FPU/MMX/SSE/
    /// SSE2/SSE3 instruction; with the exception of PAUSE, PREFETCHh, SFENCE,
    /// LFENCE, MFENCE, MOVNTI, and CLFLUSH. See the paragraph below for the
    /// special case of the WAIT/FWAIT instructions.
    /// - If the TS flag is set and the MP flag (bit 1 of CR0) and EM flag are clear, an #NM
    /// exception is not raised prior to the execution of an x87 FPU WAIT/FWAIT
    /// instruction.
    /// - If the EM flag is set, the setting of the TS flag has no affect on the execution of
    /// x87 FPU/MMX/SSE/SSE2/SSE3 instructions.
    ///
    /// The processor does not automatically save the context of the x87 FPU, XMM, and
    /// MXCSR registers on a task switch. Instead, it sets the TS flag, which causes the
    /// processor to raise an #NM exception whenever it encounters an x87 FPU/MMX/SSE
    /// /SSE2/SSE3 instruction in the instruction stream for the new task (with the exception
    /// of the instructions listed above).
    ///
    /// The fault handler for the #NM exception can then be used to clear the TS flag (with the CLTS
    /// instruction) and save the context of the x87 FPU, XMM, and MXCSR registers. If the task never
    /// encounters an x87 FPU/MMX/SSE/SSE2/SSE3 instruction; the x87 FPU/MMX/SSE/SSE2/
    /// SSE3 context is never saved.
    unsigned int task_switched : 1;
    /// Extension Type (bit 4 of CR0) — Reserved in the Pentium 4, Intel Xeon, P6 family,
    /// and Pentium processors. In the Pentium 4, Intel Xeon, and P6 family processors, this
    /// flag is hardcoded to 1. In the Intel386 and Intel486 processors, this flag indicates
    /// support of Intel 387 DX math coprocessor instructions when set.
    unsigned int extension_type : 1;
    /// Numeric Error (bit 5 of CR0) — Enables the native (internal) mechanism for
    /// reporting x87 FPU errors when set; enables the PC-style x87 FPU error reporting
    /// mechanism when clear. When the NE flag is clear and the IGNNE# input is asserted,
    /// x87 FPU errors are ignored. When the NE flag is clear and the IGNNE# input is deas-
    /// serted, an unmasked x87 FPU error causes the processor to assert the FERR# pin to
    /// generate an external interrupt and to stop instruction execution immediately before
    /// executing the next waiting floating-point instruction or WAIT/FWAIT instruction.
    /// The FERR# pin is intended to drive an input to an external interrupt controller (the
    /// FERR# pin emulates the ERROR# pin of the Intel 287 and Intel 387 DX math copro-
    /// cessors). The NE flag, IGNNE# pin, and FERR# pin are used with external logic to
    /// implement PC-style error reporting.
    /// See also: “Software Exception Handling” in Chapter 8 and Appendix D in the IA-32
    /// Intel Architecture Software Developer’s Manual, Volume 1.
    unsigned int numeric_error : 1;
    unsigned int reserved0 : 10;
    /// Write Protect (bit 16 of CR0) — Inhibits supervisor-level procedures from writing
    /// into user-level read-only pages when set; allows supervisor-level procedures to write
    /// into user-level read-only pages when clear. This flag facilitates implementation of the
    /// copy-on-write method of creating a new process (forking) used by operating systems
    /// such as UNIX*.
    unsigned int write_protect : 1;
    unsigned int reserved1 : 1;
    /// Alignment Mask (bit 18 of CR0) — Enables automatic alignment checking when set;
    /// disables alignment checking when clear. Alignment checking is performed only when
    /// the AM flag is set, the AC flag in the EFLAGS register is set, CPL is 3, and the
    /// processor is operating in either protected or virtual-8086 mode.
    unsigned int alignment : 1;

    unsigned int reserved2 : 10;
    /// Not Write-through (bit 29 of CR0) — When the NW and CD flags are clear, write-
    /// back (for Pentium 4, Intel Xeon, P6 family, and Pentium processors) or write-through
    /// (for Intel486 processors) is enabled for writes that hit the cache and invalidation cycles
    /// are enabled. See Table 10-5 for detailed information about the affect of the NW flag on
    /// caching for other settings of the CD and NW flags.
    unsigned int not_write_through : 1;
    /// Cache Disable (bit 30 of CR0) — When the CD and NW flags are clear, caching of
    /// memory locations for the whole of physical memory in the processor’s internal (and
    /// external) caches is enabled. When the CD flag is set, caching is restricted as described
    /// in Table 10-5. To prevent the processor from accessing and updating its caches, the CD
    /// flag must be set and the caches must be invalidated so that no cache hits can occur.
    /// See also: Section 10.5.3, “Preventing Caching” and Section 10.5, “Cache Control”.
    unsigned int cache_disable : 1;
    /// Paging (bit 31 of CR0) — Enables paging when set; disables paging when clear.
    /// When paging is disabled, all linear addresses are treated as physical addresses. The PG
    /// flag has no effect if the PE flag (bit 0 of register CR0) is not also set; setting the PG
    /// flag when the PE flag is clear causes a general-protection exception (#GP). See also:
    /// Section 3.6, “Paging (Virtual Memory) Overview”.
    /// On IA-32 processors that support Intel® EM64T, enabling and disabling IA-32e mode
    /// operation also requires modifying CR0.PG.
    unsigned int paging : 1;
}
__attribute__((packed))
__attribute__((aligned(4)))
cr0_t;

// inline cr3_t read_cr3();
__attribute__((section(".boot.text"))) static inline cr0_t read_cr0_boot()
{
    cr0_t result;
    asm volatile(
        "mov %%cr0, %0"
        : "=r"(result));
    return result;
}

__attribute__((section(".boot.text"))) static inline void write_cr0_boot(cr0_t new)
{
    asm volatile(
        "mov %0, %%cr0"
        : : "r"(new));
}

// inline cr3_t read_cr3();
static inline cr0_t read_cr0()
{
    cr0_t result;
    asm volatile(
        "mov %%cr0, %0"
        : "=r"(result));
    return result;
}

static inline void write_cr0(cr0_t new)
{
    asm volatile(
        "mov %0, %%cr0"
        : : "r"(new));
}

/// CR4 — Contains a group of flags that enable several architectural extensions, and indicate
/// operating system or executive support for specific processor capabilities. The control
/// registers can be read and loaded (or modified) using the move-to-or-from-control-registers
/// forms of the MOV instruction. In protected mode, the MOV instructions allow the control
/// registers to be read or loaded (at privilege level 0 only). This restriction means that
/// application programs or operating-system procedures (running at privilege levels 1, 2, or
/// 3) are prevented from reading or loading the control registers.
typedef struct
{
    /// Virtual-8086 Mode Extensions (bit 0 of CR4) — Enables interrupt- and exception-
    /// handling extensions in virtual-8086 mode when set; disables the extensions when clear.
    /// Use of the virtual mode extensions can improve the performance of virtual-8086 appli-
    /// cations by eliminating the overhead of calling the virtual-8086 monitor to handle inter-
    /// rupts and exceptions that occur while executing an 8086 program and, instead,
    /// redirecting the interrupts and exceptions back to the 8086 program’s handlers. It also
    /// provides hardware support for a virtual interrupt flag (VIF) to improve reliability of
    /// running 8086 programs in multitasking and multiple-processor environments.
    /// See also: Section 16.3, “Interrupt and Exception Handling in Virtual-8086 Mode”.
    unsigned int vitual8086_mode_extensions : 1;
    /// Protected-Mode Virtual Interrupts (bit 1 of CR4) — Enables hardware support for
    /// a virtual interrupt flag (VIF) in protected mode when set; disables the VIF flag in
    /// protected mode when clear.
    /// See also: Section 16.4, “Protected-Mode Virtual Interrupts”.
    unsigned int protected_mode_virtual_interrupts : 1;
    /// Time Stamp Disable (bit 2 of CR4) — Restricts the execution of the RDTSC instruc-
    /// tion to procedures running at privilege level 0 when set; allows RDTSC instruction to
    /// be executed at any privilege level when clear.
    unsigned int timestamp_disable : 1;
    /// Debugging Extensions (bit 3 of CR4) — References to debug registers DR4 and DR5
    /// cause an undefined opcode (#UD) exception to be generated when set; when clear,
    /// processor aliases references to registers DR4 and DR5 for compatibility with software
    /// written to run on earlier IA-32 processors.
    /// See also: Section 15.2.2, “Debug Registers DR4 and DR5”.
    unsigned int debugging_extensions : 1;
    /// Page Size Extensions (bit 4 of CR4) — Enables 4-MByte pages when set; restricts
    /// pages to 4 KBytes when clear.
    /// See also: Section 3.6.1, “Paging Options”.
    unsigned int page_size_extensions : 1;
    /// Physical Address Extension (bit 5 of CR4) — When set, enables paging mechanism
    /// to reference greater-or-equal-than-36-bit physical addresses. When clear, restricts
    /// physical addresses to 32 bits. PAE must be enabled to enable IA-32e mode operation.
    /// Enabling and disabling IA-32e mode operation also requires modifying CR4.PAE.
    /// See also: Section 3.8, “36-Bit Physical Addressing Using the PAE Paging
    /// Mechanism”.
    unsigned int physical_address_extension : 1;
    /// Machine-Check Enable (bit 6 of CR4) — Enables the machine-check exception
    /// when set; disables the machine-check exception when clear.
    /// See also: Chapter 14, Machine-Check Architecture.
    unsigned int machine_check_enable : 1;
    /// Page Global Enable (bit 7 of CR4) — (Introduced in the P6 family processors.)
    /// Enables the global page feature when set; disables the global page feature when clear.
    /// The global page feature allows frequently used or shared pages to be marked as global
    /// to all users (done with the global flag, bit 8, in a page-directory or page-table entry).
    /// Global pages are not flushed from the translation-lookaside buffer (TLB) on a task
    /// switch or a write to register CR3.
    ///
    ///  When enabling the global page feature, paging must be enabled (by setting the PG flag
    /// in control register CR0) before the PGE flag is set. Reversing this sequence may affect
    /// program correctness, and processor performance will be impacted.
    /// See also: Section 3.12, “Translation Lookaside Buffers (TLBs)”.
    unsigned int page_global_enable : 1;
    /// Performance-Monitoring Counter Enable (bit 8 of CR4) — Enables execution of
    /// the RDPMC instruction for programs or procedures running at any protection level
    /// when set; RDPMC instruction can be executed only at protection level 0 when clear.
    unsigned int performance_monitoring_counter_enable : 1;
    /// Operating System Support for FXSAVE and FXRSTOR instructions (bit 9 of
    /// CR4) — When set, this flag: (1) indicates to software that the operating system
    /// supports the use of the FXSAVE and FXRSTOR instructions, (2) enables the FXSAVE
    /// and FXRSTOR instructions to save and restore the contents of the XMM and MXCSR
    /// registers along with the contents of the x87 FPU and MMX registers, and (3) enables
    /// the processor to execute SSE/SSE2/SSE3 instructions, with the exception of the
    /// PAUSE, PREFETCHh, SFENCE, LFENCE, MFENCE, MOVNTI, and CLFLUSH.
    ///
    /// If this flag is clear, the FXSAVE and FXRSTOR instructions will save and restore the
    /// contents of the x87 FPU and MMX instructions, but they may not save and restore the
    /// contents of the XMM and MXCSR registers. Also, the processor will generate an
    /// invalid opcode exception (#UD) if it attempts to execute any SSE/SSE2/SSE3 instruc-
    /// tion, with the exception of PAUSE, PREFETCHh, SFENCE, LFENCE, MFENCE,
    /// MOVNTI, and CLFLUSH. The operating system or executive must explicitly set this
    /// flag.
    ///
    /// NOTE
    /// - CPUID feature flags FXSR, SSE, SSE2, and SSE3 indicate availability
    /// of the FXSAVE/FXRESTOR instructions, SSE extensions, SSE2
    /// extensions, and SSE3 extensions respectively. The OSFXSR bit
    /// provides operating system software with a means of enabling these
    /// features and indicating that the operating system supports the features.
    unsigned int osfxsr : 1;
    /// Operating System Support for Unmasked SIMD Floating-Point Exceptions (bit 10
    /// of CR4) — When set, indicates that the operating system supports the handling of
    /// unmasked SIMD floating-point exceptions through an exception handler that is invoked
    /// when a SIMD floating-point exception (#XF) is generated. SIMD floating-point excep-
    /// tions are only generated by SSE/SSE2/SSE3 SIMD floating-point instructions.
    ///
    /// The operating system or executive must explicitly set this flag. If this flag is not set, the
    /// processor will generate an invalid opcode exception (#UD) whenever it detects an
    /// unmasked SIMD floating-point exception.
    unsigned int osxmmexcpt : 1;

    unsigned int res0 : 21;
}
__attribute__((packed))
__attribute__((aligned(4)))
cr4_t;

__attribute__((section(".boot.text"))) static inline cr4_t read_cr4_boot()
{
    cr4_t result;
    asm volatile(
        "mov %%cr4, %0"
        : "=r"(result));
    return result;
}

__attribute__((section(".boot.text"))) static inline void write_cr4_boot(cr4_t new)
{
    asm volatile(
        "mov %0, %%cr4"
        : : "r"(new));
}

/// Linear Address Translation (4-KByte Pages)
typedef struct
{
    /// **Page-offset**
    /// Bits 0 through 11 provides an offset to a physical address in the page.
    unsigned int page_offset : 12;
    /// **Page-table entry** — Bits 12 through 21 of the linear address provide an offset to an entry
    /// in the selected page table. This entry provides the base physical address of a page in
    /// physical memory.
    unsigned int page_table_entry : 10;
    /// **Page-directory entry** — Bits 22 through 31 provide an offset to an entry in the page
    /// directory. The selected entry provides the base physical address of a page table.
    unsigned int page_directory_entry : 10;
}
__attribute__((packed))
__attribute__((aligned(4)))
linear_address_4k_t;

/// Linear Address Translation (4-MByte Pages)
typedef struct
{
    /// Page offset—Bits 0 through 21 provides an offset to a physical address in the page.
    unsigned int page_offset : 22;
    /// **Page directory entry** —Bits 22 through 31 provide an offset to an entry in the page
    /// directory. The selected entry provides the base physical address of a 4-MByte page.
    unsigned int page_directory_entry : 10;
} __attribute__((packed))
__attribute__((aligned(4)))
linear_address_4m_t;

/// Format of Page-Directory and Page-Table Entries for 4-KByte Pages
/// and 32-Bit Physical Addresses
typedef struct
{
    /// **Present (P) flag, bit 0**
    /// Indicates whether the page or page table being pointed to by the entry is
    /// currently loaded in physical memory. When the flag is set, the page is in phys-
    /// ical memory and address translation is carried out. When the flag is clear, the
    /// page is not in memory and, if the processor attempts to access the page, it
    /// generates a page-fault exception (#PF).
    /// The processor does not set or clear this flag; it is up to the operating system or
    /// executive to maintain the state of the flag.
    /// If the processor generates a page-fault exception, the operating system gener-
    /// ally needs to carry out the following operations:
    /// - 1. Copy the page from disk storage into physical memory.
    /// - 2.
    ///  Load the page address into the page-table or page-directory entry and set
    /// its present flag. Other flags, such as the dirty and accessed flags, may also
    /// be set at this time.
    /// - 3. Invalidate the current page-table entry in the TLB (see Section 3.12,
    /// “Translation Lookaside Buffers (TLBs)”, for a discussion of TLBs and
    /// how to invalidate them).
    /// - 4. Return from the page-fault handler to restart the interrupted program (or
    /// task).
    int present : 1;
    /// **Read/write (R/W) flag, bit 1**
    /// Specifies the read-write privileges for a page or group of pages (in the case of
    /// a page-directory entry that points to a page table). When this flag is clear, the
    /// page is read only; when the flag is set, the page can be read and written into.
    /// This flag interacts with the U/S flag and the WP flag in register CR0. See
    /// Section 4.11, “Page-Level Protection”, and Table 4-3 for a detailed discussion
    /// of the use of these flags.
    int read_write : 1;
    /// **User/supervisor (U/S) flag, bit 2**
    /// Specifies the user-supervisor privileges for a page or group of pages (in the
    /// case of a page-directory entry that points to a page table). When this flag is
    /// clear, the page is assigned the supervisor privilege level; when the flag is set,
    /// the page is assigned the user privilege level. This flag interacts with the R/W
    /// flag and the WP flag in register CR0. See Section 4.11, “Page-Level Protec-
    /// tion”, and Table 4-3 for a detail discussion of the use of these flags.
    int sepervisor : 1;
    /// **Page-level write-through (PWT) flag, bit 3**
    /// Controls the write-through or write-back caching policy of individual pages or
    /// page tables. When the PWT flag is set, write-through caching is enabled for the
    /// associated page or page table; when the flag is clear, write-back caching is
    /// enabled for the associated page or page table. The processor ignores this flag if
    /// the CD (cache disable) flag in CR0 is set. See Section 10.5, “Cache Control”,
    /// for more information about the use of this flag. See Section 2.5, “Control
    /// Registers”, for a description of a companion PWT flag in control register CR3.
    int page_level_write_through : 1;
    /// **Page-level cache disable (PCD) flag, bit 4**
    /// Controls the caching of individual pages or page tables. When the PCD flag is
    /// set, caching of the associated page or page table is prevented; when the flag is
    /// clear, the page or page table can be cached. This flag permits caching to be
    /// disabled for pages that contain memory-mapped I/O ports or that do not
    /// provide a performance benefit when cached. The processor ignores this flag
    /// (assumes it is set) if the CD (cache disable) flag in CR0 is set. See Chapter 10,
    /// Memory Cache Control, for more information about the use of this flag. See
    /// Section 2.5, “Control Registers”, for a description of a companion PCD flag in
    /// control register CR3.
    int page_level_cache_disable : 1;
    /// **Accessed (A) flag, bit 5**
    /// Indicates whether a page or page table has been accessed (read from or written
    /// to) when set. Memory management software typically clears this flag when a
    /// page or page table is initially loaded into physical memory. The processor then
    /// sets this flag the first time a page or page table is accessed.
    ///
    /// This flag is a “sticky” flag, meaning that once set, the processor does not
    /// implicitly clear it. Only software can clear this flag. The accessed and dirty
    /// flags are provided for use by memory management software to manage the
    /// transfer of pages and page tables into and out of physical memory.
    /// **NOTE:**
    /// The accesses used by the processor to set this bit may or may not be
    /// exposed to the processor’s Self-Modifying Code detection logic. If the
    /// processor is executing code from the same memory area that is being used for
    /// page table structures, the setting of the bit may or may not result in an imme-
    /// diate change to the executing code stream.
    int accessed : 1;
    /// **bit 6**
    int reserved : 1;
    /// **Page size (PS) flag, bit 7 page-directory entries for 4-KByte pages**
    /// Determines the page size. When this flag is clear, the page size is 4 KBytes and
    /// the page-directory entry points to a page table. When the flag is set, the page
    /// size is 4 MBytes for normal 32-bit addressing (and 2 MBytes if extended phys-
    /// ical addressing is enabled) and the page-directory entry points to a page. If the
    /// page-directory entry points to a page table, all the pages associated with that
    /// page table will be 4-KByte pages.
    int is_page : 1;
    /// **Global (G) flag, bit 8**
    /// (Introduced in the Pentium Pro processor) — Indicates a global page when set.
    /// When a page is marked global and the page global enable (PGE) flag in register
    /// CR4 is set, the page-table or page-directory entry for the page is not invalidated
    /// in the TLB when register CR3 is loaded or a task switch occurs. This flag is
    /// provided to prevent frequently used pages (such as pages that contain kernel or
    /// other operating system or executive code) from being flushed from the TLB.
    /// Only software can set or clear this flag. For page-directory entries that point to
    /// page tables, this flag is ignored and the global characteristics of a page are set
    /// in the page-table entries. See Section 3.12, “Translation Lookaside Buffers
    /// (TLBs)”, for more information about the use of this flag. (This bit is reserved
    /// in Pentium and earlier IA-32 processors.)
    int global : 1;

    int available_bits : 3;

    /// (Page-directory entries for 4-KByte page tables) — Specifies the physical
    /// address of the first byte of a page table. The bits in this field are interpreted as
    /// the 20 most-significant bits of the physical address, which forces page tables to
    /// be aligned on 4-KByte boundaries.
    int page_table_base_address : 20;
} __attribute__((packed))
__attribute__((aligned(4)))
page_directory_entry_4k_t;

static inline void pde4k_set_page_table_base_address(page_directory_entry_4k_t *self, unsigned int address)
{
    unsigned int *tmp = (unsigned int *)self;
    *tmp |= address;
}

static inline unsigned int pde4k_get_page_table_base_address(page_directory_entry_4k_t *self)
{
    unsigned int *tmp = (unsigned int *)self;
    return (*tmp >> 12) << 12;
}

static inline page_directory_entry_4k_t pde4k_zero()
{
    unsigned int tmp = 0;
    return *(page_directory_entry_4k_t *)&tmp;
}

/// Format of Page-Directory Entries for 4-MByte Pages and 32-Bit Addresses
typedef struct
{
    /// **Present (P) flag, bit 0**
    /// Indicates whether the page or page table being pointed to by the entry is
    /// currently loaded in physical memory. When the flag is set, the page is in phys-
    /// ical memory and address translation is carried out. When the flag is clear, the
    /// page is not in memory and, if the processor attempts to access the page, it
    /// generates a page-fault exception (#PF).
    /// The processor does not set or clear this flag; it is up to the operating system or
    /// executive to maintain the state of the flag.
    /// If the processor generates a page-fault exception, the operating system gener-
    /// ally needs to carry out the following operations:
    /// - 1. Copy the page from disk storage into physical memory.
    /// - 2.
    ///  Load the page address into the page-table or page-directory entry and set
    /// its present flag. Other flags, such as the dirty and accessed flags, may also
    /// be set at this time.
    /// - 3. Invalidate the current page-table entry in the TLB (see Section 3.12,
    /// “Translation Lookaside Buffers (TLBs)”, for a discussion of TLBs and
    /// how to invalidate them).
    /// - 4. Return from the page-fault handler to restart the interrupted program (or
    /// task).
    int present : 1;
    /// **Read/write (R/W) flag, bit 1**
    /// Specifies the read-write privileges for a page or group of pages (in the case of
    /// a page-directory entry that points to a page table). When this flag is clear, the
    /// page is read only; when the flag is set, the page can be read and written into.
    /// This flag interacts with the U/S flag and the WP flag in register CR0. See
    /// Section 4.11, “Page-Level Protection”, and Table 4-3 for a detailed discussion
    /// of the use of these flags.
    int read_write : 1;
    /// **User/supervisor (U/S) flag, bit 2**
    /// Specifies the user-supervisor privileges for a page or group of pages (in the
    /// case of a page-directory entry that points to a page table). When this flag is
    /// clear, the page is assigned the supervisor privilege level; when the flag is set,
    /// the page is assigned the user privilege level. This flag interacts with the R/W
    /// flag and the WP flag in register CR0. See Section 4.11, “Page-Level Protec-
    /// tion”, and Table 4-3 for a detail discussion of the use of these flags.
    int sepervisor : 1;
    /// **Page-level write-through (PWT) flag, bit 3**
    /// Controls the write-through or write-back caching policy of individual pages or
    /// page tables. When the PWT flag is set, write-through caching is enabled for the
    /// associated page or page table; when the flag is clear, write-back caching is
    /// enabled for the associated page or page table. The processor ignores this flag if
    /// the CD (cache disable) flag in CR0 is set. See Section 10.5, “Cache Control”,
    /// for more information about the use of this flag. See Section 2.5, “Control
    /// Registers”, for a description of a companion PWT flag in control register CR3.
    int page_level_write_through : 1;
    /// **Page-level cache disable (PCD) flag, bit 4**
    /// Controls the caching of individual pages or page tables. When the PCD flag is
    /// set, caching of the associated page or page table is prevented; when the flag is
    /// clear, the page or page table can be cached. This flag permits caching to be
    /// disabled for pages that contain memory-mapped I/O ports or that do not
    /// provide a performance benefit when cached. The processor ignores this flag
    /// (assumes it is set) if the CD (cache disable) flag in CR0 is set. See Chapter 10,
    /// Memory Cache Control, for more information about the use of this flag. See
    /// Section 2.5, “Control Registers”, for a description of a companion PCD flag in
    /// control register CR3.
    int page_level_cache_disable : 1;
    /// **Accessed (A) flag, bit 5**
    /// Indicates whether a page or page table has been accessed (read from or written
    /// to) when set. Memory management software typically clears this flag when a
    /// page or page table is initially loaded into physical memory. The processor then
    /// sets this flag the first time a page or page table is accessed.
    ///
    /// This flag is a “sticky” flag, meaning that once set, the processor does not
    /// implicitly clear it. Only software can clear this flag. The accessed and dirty
    /// flags are provided for use by memory management software to manage the
    /// transfer of pages and page tables into and out of physical memory.
    /// **NOTE:**
    /// The accesses used by the processor to set this bit may or may not be
    /// exposed to the processor’s Self-Modifying Code detection logic. If the
    /// processor is executing code from the same memory area that is being used for
    /// page table structures, the setting of the bit may or may not result in an imme-
    /// diate change to the executing code stream.
    int accsessed : 1;

    /// **Dirty (D) flag, bit 6**
    /// Indicates whether a page has been written to when set. (This flag is not used in
    /// page-directory entries that point to page tables.) Memory management soft-
    /// ware typically clears this flag when a page is initially loaded into physical
    /// memory. The processor then sets this flag the first time a page is accessed for
    /// a write operation.
    ///
    /// This flag is “sticky,” meaning that once set, the processor does not implicitly
    /// clear it. Only software can clear this flag. The dirty and accessed flags are
    /// provided for use by memory management software to manage the transfer of
    /// pages and page tables into and out of physical memory.
    /// **NOTE:**
    ///  The accesses used by the processor to set this bit may or may not be
    /// exposed to the processor’s Self-Modifying Code detection logic. If the
    /// processor is executing code from the same memory area that is being used for
    /// page table structures, the setting of the bit may or may not result in an imme-
    /// diate change to the executing code stream.
    int dirty : 1;

    /// **Page size (PS) flag, bit 7 page-directory entries for 4-KByte pages**
    /// Determines the page size. When this flag is clear, the page size is 4 KBytes and
    /// the page-directory entry points to a page table. When the flag is set, the page
    /// size is 4 MBytes for normal 32-bit addressing (and 2 MBytes if extended phys-
    /// ical addressing is enabled) and the page-directory entry points to a page. If the
    /// page-directory entry points to a page table, all the pages associated with that
    /// page table will be 4-KByte pages.
    int is_page : 1;
    /// **Global (G) flag, bit 8**
    /// (Introduced in the Pentium Pro processor) — Indicates a global page when set.
    /// When a page is marked global and the page global enable (PGE) flag in register
    /// CR4 is set, the page-table or page-directory entry for the page is not invalidated
    /// in the TLB when register CR3 is loaded or a task switch occurs. This flag is
    /// provided to prevent frequently used pages (such as pages that contain kernel or
    /// other operating system or executive code) from being flushed from the TLB.
    /// Only software can set or clear this flag. For page-directory entries that point to
    /// page tables, this flag is ignored and the global characteristics of a page are set
    /// in the page-table entries. See Section 3.12, “Translation Lookaside Buffers
    /// (TLBs)”, for more information about the use of this flag. (This bit is reserved
    /// in Pentium and earlier IA-32 processors.)
    int global : 1;
    /// **Reserved and available-to-software bits**
    /// For all IA-32 processors. Bits 9, 10, and 11 are available for use by software.
    /// (When the present bit is clear, bits 1 through 31 are available to software, see
    /// Figure 3-16.) In a page-directory entry that points to a page table, bit 6 is
    /// reserved and should be set to 0. When the PSE and PAE flags in control register
    /// CR4 are set, the processor generates a page fault if reserved bits are not set to 0.
    /// For Pentium II and earlier processors. Bit 7 in a page-table entry is reserved and
    /// should be set to 0. For a page-directory entry for a 4-MByte page, bits 12
    /// through 21 are reserved and must be set to 0.
    /// For Pentium III and later processors. For a page-directory entry for a 4-MByte
    /// page, bits 13 through 21 are reserved and must be set to 0.
    int available_bits : 3;
    /// **Page attribute table index (PAT) flag, bit 7 in page-table entries for 4-KByte pages and
    /// bit 12 in page-directory entries for 4-MByte pages**
    /// (Introduced in the Pentium III processor) — Selects PAT entry. For processors
    /// that support the page attribute table (PAT), this flag is used along with the
    /// PCD and PWT flags to select an entry in the PAT, which in turn selects the
    /// memory type for the page (see Section 10.12, “Page Attribute Table (PAT)”).
    /// For processors that do not support the PAT, this bit is reserved and should be
    /// set to 0.
    int page_attribute_table_index : 1;

    int reserved : 9;
    /// **(Page-directory entries for 4-MByte pages)** — Specifies the physical address
    /// of the first byte of a 4-MByte page. Only bits 22 through 31 of this field are
    /// used (and bits 12 through 21 are reserved and must be set to 0, for IA-32
    /// processors through the Pentium II processor). The base address bits are inter-
    /// preted as the 10 most-significant bits of the physical address, which forces
    /// 4-MByte pages to be aligned on 4-MByte boundaries.
    int page_base_address : 10;
} __attribute__((packed))
__attribute__((aligned(4)))
page_directory_entry_4m_t;

static inline void pde4m_set_page_base_address(page_directory_entry_4m_t *self, unsigned int address)
{
    unsigned int *tmp = (unsigned int *)self;
    *tmp |= address;
}

static inline unsigned int pde4m_get_page_base_address(page_directory_entry_4m_t *self)
{
    unsigned int *tmp = (unsigned int *)self;
    return (*tmp >> 22) << 22;
}

static inline page_directory_entry_4m_t pde4m_zero()
{
    unsigned int tmp = 0;
    return *(page_directory_entry_4m_t *)&tmp;
}

__attribute__((section(".boot.text"))) static inline page_directory_entry_4m_t pde4m_boot_zero()
{
    unsigned int tmp = 0;
    return *(page_directory_entry_4m_t *)&tmp;
}

typedef union
{
    page_directory_entry_4m_t m4;
    page_directory_entry_4k_t k4;
} page_directory_entry_t;

static inline unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    __asm__(
        "in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

static inline void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__(
        "out %%al, %%dx" : : "a"(data), "d"(port));
}

/// Format of Page-Directory and Page-Table Entries for 4-KByte Pages
/// and 32-Bit Physical Addresses
typedef struct
{
    /// **Present (P) flag, bit 0**
    /// Indicates whether the page or page table being pointed to by the entry is
    /// currently loaded in physical memory. When the flag is set, the page is in phys-
    /// ical memory and address translation is carried out. When the flag is clear, the
    /// page is not in memory and, if the processor attempts to access the page, it
    /// generates a page-fault exception (#PF).
    /// The processor does not set or clear this flag; it is up to the operating system or
    /// executive to maintain the state of the flag.
    /// If the processor generates a page-fault exception, the operating system gener-
    /// ally needs to carry out the following operations:
    /// - 1. Copy the page from disk storage into physical memory.
    /// - 2.
    ///  Load the page address into the page-table or page-directory entry and set
    /// its present flag. Other flags, such as the dirty and accessed flags, may also
    /// be set at this time.
    /// - 3. Invalidate the current page-table entry in the TLB (see Section 3.12,
    /// “Translation Lookaside Buffers (TLBs)”, for a discussion of TLBs and
    /// how to invalidate them).
    /// - 4. Return from the page-fault handler to restart the interrupted program (or
    /// task).
    unsigned int present : 1;
    /// **Read/write (R/W) flag, bit 1**
    /// Specifies the read-write privileges for a page or group of pages (in the case of
    /// a page-directory entry that points to a page table). When this flag is clear, the
    /// page is read only; when the flag is set, the page can be read and written into.
    /// This flag interacts with the U/S flag and the WP flag in register CR0. See
    /// Section 4.11, “Page-Level Protection”, and Table 4-3 for a detailed discussion
    /// of the use of these flags.
    unsigned int read_write : 1;
    /// **User/supervisor (U/S) flag, bit 2**
    /// Specifies the user-supervisor privileges for a page or group of pages (in the
    /// case of a page-directory entry that points to a page table). When this flag is
    /// clear, the page is assigned the supervisor privilege level; when the flag is set,
    /// the page is assigned the user privilege level. This flag interacts with the R/W
    /// flag and the WP flag in register CR0. See Section 4.11, “Page-Level Protec-
    /// tion”, and Table 4-3 for a detail discussion of the use of these flags.
    unsigned int sepervisor : 1;
    /// **Page-level write-through (PWT) flag, bit 3**
    /// Controls the write-through or write-back caching policy of individual pages or
    /// page tables. When the PWT flag is set, write-through caching is enabled for the
    /// associated page or page table; when the flag is clear, write-back caching is
    /// enabled for the associated page or page table. The processor ignores this flag if
    /// the CD (cache disable) flag in CR0 is set. See Section 10.5, “Cache Control”,
    /// for more information about the use of this flag. See Section 2.5, “Control
    /// Registers”, for a description of a companion PWT flag in control register CR3.
    unsigned int page_level_write_through : 1;
    /// **Page-level cache disable (PCD) flag, bit 4**
    /// Controls the caching of individual pages or page tables. When the PCD flag is
    /// set, caching of the associated page or page table is prevented; when the flag is
    /// clear, the page or page table can be cached. This flag permits caching to be
    /// disabled for pages that contain memory-mapped I/O ports or that do not
    /// provide a performance benefit when cached. The processor ignores this flag
    /// (assumes it is set) if the CD (cache disable) flag in CR0 is set. See Chapter 10,
    /// Memory Cache Control, for more information about the use of this flag. See
    /// Section 2.5, “Control Registers”, for a description of a companion PCD flag in
    /// control register CR3.
    unsigned int page_level_cache_disable : 1;
    /// **Accessed (A) flag, bit 5**
    /// Indicates whether a page or page table has been accessed (read from or written
    /// to) when set. Memory management software typically clears this flag when a
    /// page or page table is initially loaded into physical memory. The processor then
    /// sets this flag the first time a page or page table is accessed.
    ///
    /// This flag is a “sticky” flag, meaning that once set, the processor does not
    /// implicitly clear it. Only software can clear this flag. The accessed and dirty
    /// flags are provided for use by memory management software to manage the
    /// transfer of pages and page tables into and out of physical memory.
    /// **NOTE:**
    /// The accesses used by the processor to set this bit may or may not be
    /// exposed to the processor’s Self-Modifying Code detection logic. If the
    /// processor is executing code from the same memory area that is being used for
    /// page table structures, the setting of the bit may or may not result in an imme-
    /// diate change to the executing code stream.
    unsigned int accessed : 1;
    /// **Dirty (D) flag, bit 6**
    /// Indicates whether a page has been written to when set. (This flag is not used in
    /// page-directory entries that point to page tables.) Memory management soft-
    /// ware typically clears this flag when a page is initially loaded into physical
    /// memory. The processor then sets this flag the first time a page is accessed for
    /// a write operation.
    ///
    /// This flag is “sticky,” meaning that once set, the processor does not implicitly
    /// clear it. Only software can clear this flag. The dirty and accessed flags are
    /// provided for use by memory management software to manage the transfer of
    /// pages and page tables into and out of physical memory.
    /// **NOTE:**
    ///  The accesses used by the processor to set this bit may or may not be
    /// exposed to the processor’s Self-Modifying Code detection logic. If the
    /// processor is executing code from the same memory area that is being used for
    /// page table structures, the setting of the bit may or may not result in an imme-
    /// diate change to the executing code stream.
    unsigned int dirty : 1;
    /// **Page attribute table index (PAT) flag, bit 7 in page-table entries for 4-KByte pages and
    /// bit 12 in page-directory entries for 4-MByte pages**
    /// (Introduced in the Pentium III processor) — Selects PAT entry. For processors
    /// that support the page attribute table (PAT), this flag is used along with the
    /// PCD and PWT flags to select an entry in the PAT, which in turn selects the
    /// memory type for the page (see Section 10.12, “Page Attribute Table (PAT)”).
    /// For processors that do not support the PAT, this bit is reserved and should be
    /// set to 0.
    unsigned int page_attribute_table_index : 1;
    /// **Global (G) flag, bit 8**
    /// (Introduced in the Pentium Pro processor) — Indicates a global page when set.
    /// When a page is marked global and the page global enable (PGE) flag in register
    /// CR4 is set, the page-table or page-directory entry for the page is not invalidated
    /// in the TLB when register CR3 is loaded or a task switch occurs. This flag is
    /// provided to prevent frequently used pages (such as pages that contain kernel or
    /// other operating system or executive code) from being flushed from the TLB.
    /// Only software can set or clear this flag. For page-directory entries that point to
    /// page tables, this flag is ignored and the global characteristics of a page are set
    /// in the page-table entries. See Section 3.12, “Translation Lookaside Buffers
    /// (TLBs)”, for more information about the use of this flag. (This bit is reserved
    /// in Pentium and earlier IA-32 processors.)
    unsigned int global : 1;

    unsigned int available_bits : 3;

    /// **Page base address**, bits 12 through 32
    /// (Page-table entries for 4-KByte pages) — Specifies the physical address of the
    /// first byte of a 4-KByte page. The bits in this field are interpreted as the 20 most-
    /// significant bits of the physical address, which forces pages to be aligned on
    /// 4-KByte boundaries.
    unsigned int page_base_address : 20;
}
__attribute__((packed))
__attribute__((aligned(4)))
page_table_entry_t;

static inline page_table_entry_t pte_zero()
{
    unsigned int tmp = 0;
    return *(page_table_entry_t *)&tmp;
}

static inline void pte_set_page_base_address(page_table_entry_t *self, unsigned int address)
{
    unsigned int *tmp = (unsigned int *)self;
    self->page_base_address = 0;
    *tmp |= address;
}

static inline unsigned int pte_get_page_base_address(page_table_entry_t *self)
{
    unsigned int *tmp = (unsigned int *)self;
    return (*tmp >> 12) << 12;
}

typedef struct
{
    /// The accessed bit indicates whether the segment has been accessed since the last time the oper-
    /// ating-system or executive cleared the bit. The processor sets this bit whenever it loads a segment
    /// selector for the segment into a segment register, assuming that the type of memory that contains
    /// the segment descriptor supports processor writes. The bit remains set until explicitly cleared.
    /// This bit can be used both for virtual memory management and for debugging.
    unsigned int accessed : 1;
    /// Data segments can be read-
    /// only or read/write segments, depending on the setting of the write-enable bit.
    /// Stack segments are data segments which must be read/write segments.
    ///
    /// Loading the SS register
    /// with a segment selector for a nonwritable data segment generates a general-protection exception
    /// (#GP).
    unsigned int write_enabled : 1;
    /// If the size of a stack segment needs to be changed dynamically, the stack segment can be
    /// an expand-down data segment (expansion-direction flag set). Here, dynamically changing the
    /// segment limit causes stack space to be added to the bottom of the stack. If the size of a stack
    /// segment is intended to remain static, the stack segment may be either an expand-up or expand-
    /// down type.
    unsigned int expand_down : 1;
    unsigned int is_code : 1;

    // pub inline fn toInt(self: DataType) u4 {
    //     return @bitCast(self);
    // }
}
//__attribute__((aligned(1)))
__attribute__((packed))
segment_type_data_t;

static inline char segment_type_data_to_int(segment_type_data_t seg)
{
    return *((char *)&seg);
}

typedef struct
{
    /// The accessed bit indicates whether the segment has been accessed since the last time the oper-
    /// ating-system or executive cleared the bit. The processor sets this bit whenever it loads a segment
    /// selector for the segment into a segment register, assuming that the type of memory that contains
    /// the segment descriptor supports processor writes. The bit remains set until explicitly cleared.
    /// This bit can be used both for virtual memory management and for debugging.
    unsigned int accessed : 1;
    /// Code segments can be execute-only or execute/read, depending
    /// on the setting of the read-enable bit. An execute/read segment might be used when constants or
    /// other static data have been placed with instruction code in a ROM. Here, data can be read from
    /// the code segment either by using an instruction with a CS override prefix or by loading a
    /// segment selector for the code segment in a data-segment register (the DS, ES, FS, or GS regis-
    /// ters). In protected mode, code segments are not writable.
    unsigned read_enabled : 1;
    /// Code segments can be either conforming or nonconforming. A transfer of execution into a more-
    /// privileged conforming segment allows execution to continue at the current privilege level. A
    /// transfer into a nonconforming segment at a different privilege level results in a general-protec-
    /// tion exception (#GP), unless a call gate or task gate is used (see Section 4.8.1, “Direct Calls or
    /// Jumps to Code Segments”, for more information on conforming and nonconforming code
    /// segments). System utilities that do not access protected facilities and handlers for some types of
    /// exceptions (such as, divide error or overflow) may be loaded in conforming code segments. Util-
    /// ities that need to be protected from less privileged programs and procedures should be placed in
    /// nonconforming code segments.
    /// **NOTE**
    ///
    ///  - Execution cannot be transferred by a call or a jump to a less-privileged
    /// (numerically higher privilege level) code segment, regardless of whether the
    /// target segment is a conforming or nonconforming code segment. Attempting
    /// such an execution transfer will result in a general-protection exception.
    ///
    /// All data segments are nonconforming, meaning that they cannot be accessed by less privileged
    /// programs or procedures (code executing at numerically high privilege levels). Unlike code
    /// segments, however, data segments can be accessed by more privileged programs or procedures
    /// (code executing at numerically lower privilege levels) without using a special access gate.
    /// If the segment descriptors in the GDT or an LDT are placed in ROM, the processor can enter an
    /// indefinite loop if software or the processor attempts to update (write to) the ROM-based
    /// segment descriptors. To prevent this problem, set the accessed bits for all segment descriptors
    /// placed in a ROM. Also, remove operating-system or executive code that attempts to modify
    /// segment descriptors located in ROM.
    unsigned int conforming : 1;
    unsigned int is_code : 1;
}
__attribute__((packed))
segment_type_code_t;

typedef struct
{
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
}
//__attribute__((aligned(1)))
__attribute__((packed))
segment_type_system_t;

static inline segment_type_system_t segment_type_system_zero()
{
    return (segment_type_system_t){.a = 0, .b = 0, .c = 0, .d = 0};
}

static inline char segment_type_system_toint(segment_type_system_t self)
{
    return *((char *)&self);
}

static inline segment_type_system_t segment_type_system_tss32available()
{
    return (segment_type_system_t){.a = 1, .b = 0, .c = 0, .d = 1};
}

static inline segment_type_system_t segment_type_system_interrupt_gate32()
{
    return (segment_type_system_t){.a = 0, .b = 1, .c = 1, .d = 1};
}

static inline segment_type_system_t segment_type_system_trap_gate32()
{
    return (segment_type_system_t){.a = 1, .b = 1, .c = 1, .d = 1};

    // return .{ .@"3" = 1, .@"2" = 1, .@"1" = 1, .@"0" = 1 };
}

// typedef enum { data, code, system } segment_type_t;

typedef union
{
    segment_type_data_t data;
    segment_type_code_t code;
    segment_type_system_t system;
}
//__attribute__((aligned(1)))
__attribute__((packed))
segment_type_t;

typedef struct
{
    /// **S (descriptor type) flag**
    ///
    /// Specifies whether the segment descriptor is for a system segment (S flag is
    /// clear) or a code or data segment (S flag is set).
    unsigned int descriptor_type : 1;
    /// **DPL (descriptor privilege level) field**
    ///
    /// Specifies the privilege level of the segment. The privilege level can range from
    /// 0 to 3, with 0 being the most privileged level. The DPL is used to control access
    /// to the segment. See Section 4.5, “Privilege Levels”, for a description of the
    /// relationship of the DPL to the CPL of the executing code segment and the RPL
    /// of a segment selector.
    unsigned int privilege_level : 2;
    /// **P (segment-present) flag**
    ///
    /// Indicates whether the segment is present in memory (set) or not present (clear).
    /// If this flag is clear, the processor generates a segment-not-present exception
    /// (#NP) when a segment selector that points to the segment descriptor is loaded
    /// into a segment register. Memory management software can use this flag to
    /// control which segments are actually loaded into physical memory at a given
    /// time. It offers a control in addition to paging for managing virtual memory.
    /// Figure 3-9 shows the format of a segment descriptor when the segment-present
    /// flag is clear. When this flag is clear, the operating system or executive is free
    /// to use the locations marked “Available” to store its own data, such as informa-
    /// tion regarding the whereabouts of the missing segment.
    unsigned int present : 1;
    unsigned int limit_high : 4;
    // limit_high: u4 = std.math.maxInt(u4),
    unsigned avl : 1;
    /// **L (64-bit code segment) flag**
    ///
    /// In IA-32e mode, bit 21 of the second doubleword of the segment descriptor
    /// indicates whether a code segment contains native 64-bit code. A value of 1
    /// indicates instructions in this code segment are executed in 64-bit mode. A
    /// value of 0 indicates the instructions in this code segment are executed in
    /// compatibility mode. If L-bit is set, then D-bit must be cleared. When not in
    /// IA-32e mode or for non-code segments, bit 21 is reserved and should always
    /// be set to 0.
    unsigned int l : 1;
    /// **D/B (default operation size/default stack pointer size and/or upper bound) flag**
    ///
    /// Performs different functions depending on whether the segment descriptor is
    /// an executable code segment, an expand-down data segment, or a stack
    /// segment. (This flag should always be set to 1 for 32-bit code and data segments
    /// and to 0 for 16-bit code and data segments.)
    /// - **Executable code segment.** The flag is called the D flag and it indicates the
    /// default length for effective addresses and operands referenced by instruc-
    /// tions in the segment. If the flag is set, 32-bit addresses and 32-bit or 8-bit
    /// operands are assumed; if it is clear, 16-bit addresses and 16-bit or 8-bit
    /// operands are assumed.
    /// The instruction prefix 66H can be used to select an operand size other than
    /// the default, and the prefix 67H can be used select an address size other than
    /// the default.
    /// - **Stack segment (data segment pointed to by the SS register).** The flag is
    /// called the B (big) flag and it specifies the size of the stack pointer used for
    /// implicit stack operations (such as pushes, pops, and calls). If the flag is set,
    /// a 32-bit stack pointer is used, which is stored in the 32-bit ESP register; if
    /// the flag is clear, a 16-bit stack pointer is used, which is stored in the 16-bit
    /// SP register. If the stack segment is set up to be an expand-down data
    /// segment (described in the next paragraph), the B flag also specifies the
    /// upper bound of the stack segment.
    /// - **Expand-down data segment.** The flag is called the B flag and it specifies
    /// the upper bound of the segment. If the flag is set, the upper bound is
    /// FFFFFFFFH (4 GBytes); if the flag is clear, the upper bound is FFFFH
    /// (64 KBytes).
    unsigned int db : 1;
    /// **G (granularity) flag**
    ///
    /// Determines the scaling of the segment limit field. When the granularity flag is
    /// clear, the segment limit is interpreted in byte units; when flag is set, the
    /// segment limit is interpreted in 4-KByte units. (This flag does not affect the
    /// granularity of the base address; it is always byte granular.) When the granu-
    /// larity flag is set, the twelve least significant bits of an offset are not tested when
    /// checking the offset against the segment limit. For example, when the granu-
    /// larity flag is set, a limit of 0 results in valid offsets from 0 to 4095.
    unsigned granularity : 1;
}
//__attribute__((aligned(1)))
__attribute__((packed))
segment_flags_t;

/// A segment descriptor is a data structure in a GDT or LDT that provides the processor with the
/// size and location of a segment, as well as access control and status information. Segment
/// descriptors are typically created by compilers, linkers, loaders, or the operating system or exec-
/// utive, but not application programs. Figure 3-8 illustrates the general descriptor format for all
/// types of segment descriptors.
typedef struct
{
    /// Specifies the size of the segment. The processor puts together the two segment
    /// limit fields to form a 20-bit value. The processor interprets the segment limit
    /// in one of two ways, depending on the setting of the G (granularity) flag:
    /// - If the granularity flag is clear, the segment size can range from 1 byte to 1
    /// MByte, in byte increments.
    /// - If the granularity flag is set, the segment size can range from 4 KBytes to
    /// 4 GBytes, in 4-KByte increments.
    ///
    /// The processor uses the segment limit in two different ways, depending on
    /// whether the segment is an expand-up or an expand-down segment. See Section
    /// 3.4.5.1, “Code- and Data-Segment Descriptor Types”, for more information
    /// about segment types. For expand-up segments, the offset in a logical address
    /// can range from 0 to the segment limit. Offsets greater than the segment limit
    /// generate general-protection exceptions (#GP). For expand-down segments, the
    /// segment limit has the reverse function; the offset can range from the segment
    /// limit to FFFFFFFFH or FFFFH, depending on the setting of the B flag. Offsets
    /// less than the segment limit generate general-protection exceptions. Decreasing
    /// the value in the segment limit field for an expand-down segment allocates new
    /// memory at the bottom of the segment's address space, rather than at the top.
    /// IA-32 architecture stacks always grow downwards, making this mechanism
    /// convenient for expandable stacks.
    unsigned int limit_low : 16;
    /// ***Base address fields***
    ///
    /// Defines the location of byte 0 of the segment within the 4-GByte linear address
    /// space. The processor puts together the three base address fields to form a single
    /// 32-bit value. Segment base addresses should be aligned to 16-byte boundaries.
    /// Although 16-byte alignment is not required, this alignment allows programs to
    /// maximize performance by aligning code and data on 16-byte boundaries.
    unsigned int base_low : 24;

    /// Indicates the segment or gate type and specifies the kinds of access that can be
    /// made to the segment and the direction of growth. The interpretation of this field
    /// depends on whether the descriptor type flag specifies an application (code or
    /// data) descriptor or a system descriptor. The encoding of the type field is
    /// different for code, data, and system descriptors (see Figure 4-1). See Section
    /// 3.4.5.1, “Code- and Data-Segment Descriptor Types”, for a description of how
    /// this field is used to specify code and data-segment types.
    ///
    // segment_type_t descriptor_type;
    unsigned int type : 4;

    // segment_flags_t flags;
    unsigned int flags : 12;

    // base_high: u8 = 0,
    unsigned int base_high : 8;
} __attribute__((packed)) segment_descriptor_t;

static inline segment_descriptor_t segment_descriptor_init(
    uint32_t base, uint32_t limit,
    segment_type_t descriptor_type, segment_flags_t flags)
{
    flags.limit_high = limit >> 16;
    return (segment_descriptor_t){
        .base_low = base,
        .limit_low = (unsigned short)limit,
        .flags = *((unsigned int *)&flags),
        .type = *((unsigned int *)&descriptor_type),
        .base_high = base >> 24};
}

static inline uint32_t segment_descriptor_get_limit(segment_descriptor_t *self)
{
    uint16_t a = self->flags;
    return (((segment_flags_t *)&a)->limit_high << 16) | self->limit_low;
}
static inline uint32_t segment_descriptor_get_base(segment_descriptor_t *self)
{
    return (self->base_high << 24) | self->base_low;
}
static inline void segment_descriptor_set_limit(segment_descriptor_t *self, uint32_t value)
{
    uint16_t a = self->flags;
    ((segment_flags_t *)&a)->limit_high = value >> 16;
    self->flags = a;
    self->limit_low = value;
}
static inline void segment_descriptor_set_base(segment_descriptor_t *self, uint32_t value)
{
    self->base_high = value >> 24;
    self->base_low = value;
}
static inline segment_descriptor_t segment_descriptor_zero()
{
    uint64_t a = 0;
    return *((segment_descriptor_t *)&a);
}

/// The system flags and IOPL field of the EFLAGS register control I/O, maskable hardware inter-
/// rupts, debugging, task switching, and the virtual-8086 mode (see Figure 2-4). Only privileged
/// code (typically operating system or executive code) should be allowed to modify these bits.
typedef struct
{
    unsigned int cf : 1;
    unsigned int reserved1 : 1;
    unsigned int pf : 1;
    unsigned int reserved2 : 1;
    unsigned int af : 1;
    unsigned int reserved3 : 1;
    unsigned int zf : 1;
    unsigned int sf : 1;
    unsigned int tf : 1;

    /// Interrupt enable (bit 9) — Controls the response of the processor to maskable hard-
    /// ware interrupt requests (see also: Section 5.3.2, “Maskable Hardware Interrupts”). The
    /// flag is set to respond to maskable hardware interrupts; cleared to inhibit maskable hard-
    /// ware interrupts. The IF flag does not affect the generation of exceptions or
    /// nonmaskable interrupts (NMI interrupts). The CPL, IOPL, and the state of the VME
    /// flag in control register CR4 determine whether the IF flag can be modified by the CLI,
    /// STI, POPF, POPFD, and IRET.
    unsigned int interrupts_enable : 1;

    unsigned int df : 1;
    unsigned int of : 1;

    /// I/O privilege level field (bits 12 and 13) — Indicates the I/O privilege level (IOPL)
    /// of the currently running program or task. The CPL of the currently running program
    /// or task must be less than or equal to the IOPL to access the I/O address space. This
    /// field can only be modified by the POPF and IRET instructions when operating at a
    /// CPL of 0.
    unsigned int io_previlege_level : 2;

    /// Nested task (bit 14) — Controls the chaining of interrupted and called tasks. The
    /// processor sets this flag on calls to a task initiated with a CALL instruction, an interrupt,
    /// or an exception. It examines and modifies this flag on returns from a task initiated with
    /// the IRET instruction. The flag can be explicitly set or cleared with the POPF/POPFD
    /// instructions; however, changing to the state of this flag can generate unexpected excep-
    /// tions in application programs.
    /// See also: Section 6.4, “Task Linking”.
    unsigned int nested_task : 1;

    unsigned int reserved4 : 1;

    /// Resume (bit 16) — Controls the processor’s response to instruction-breakpoint condi
    /// tions. When set, this flag temporarily disables debug exceptions (#DB) from being
    /// generated for instruction breakpoints (although other exception conditions can
    /// cause an exception to be generated). When clear, instruction breakpoints will
    /// generate debug exceptions.
    /// The primary function of the RF flag is to allow the restarting of an instruction following
    /// a debug exception that was caused by an instruction breakpoint condition. Here, debug
    /// software must set this flag in the EFLAGS image on the stack just prior to returning to
    /// the interrupted program with IRETD (to prevent the instruction breakpoint from
    /// causing another debug exception). The processor then automatically clears this flag
    /// after the instruction returned to has been successfully executed, enabling instruction
    /// breakpoint faults again.
    /// See also: Section 15.3.1.1, “Instruction-Breakpoint Exception Condition”.
    unsigned int resumeflag : 1;

    /// Virtual-8086 mode (bit 17) — Set to enable virtual-8086 mode; clear to return to
    /// protected mode.
    /// See also: Section 16.2.1, “Enabling Virtual-8086 Mode”.
    unsigned int vitual_8086_mode : 1;

    /// Alignment check (bit 18) — Set this flag and the AM flag in control register CR0 to
    /// enable alignment checking of memory references; clear the AC flag and/or the AM flag
    /// to disable alignment checking. An alignment-check exception is generated when refer-
    /// ence is made to an unaligned operand, such as a word at an odd byte address or a
    /// doubleword at an address which is not an integral multiple of four. Alignment-check
    /// exceptions are generated only in user mode (privilege level 3). Memory references that
    /// default to privilege level 0, such as segment descriptor loads, do not generate this
    /// exception even when caused by instructions executed in user-mode.
    /// The alignment-check exception can be used to check alignment of data. This is useful
    /// when exchanging data with processors which require all data to be aligned. The align-
    /// ment-check exception can also be used by interpreters to flag some pointers as special
    /// by misaligning the pointer. This eliminates overhead of checking each pointer and only
    /// handles the special pointer when used.
    unsigned int alignment_check : 1;

    /// Virtual Interrupt (bit 19) — Contains a virtual image of the IF flag. This flag is used
    /// in conjunction with the VIP flag. The processor only recognizes the VIF flag when
    /// either the VME flag or the PVI flag in control register CR4 is set and the IOPL is less
    /// than 3. (The VME flag enables the virtual-8086 mode extensions; the PVI flag enables
    /// the protected-mode virtual interrupts.)
    /// See also: Section 16.3.3.5, “Method 6: Software Interrupt Handling” and Section 16.4,
    /// “Protected-Mode Virtual Interrupts”.
    unsigned int vrtual_interrupt : 1;

    /// Virtual interrupt pending (bit 20) — Set by software to indicate that an interrupt is
    /// pending; cleared to indicate that no interrupt is pending. This flag is used in conjunction
    /// with the VIF flag. The processor reads this flag but never modifies it. The processor
    /// only recognizes the VIP flag when either the VME flag or the PVI flag in control
    /// register CR4 is set and the IOPL is less than 3. The VME flag enables the virtual-8086
    /// mode extensions; the PVI flag enables the protected-mode virtual interrupts.
    /// See Section 16.3.3.5, “Method 6: Software Interrupt Handling” and Section 16.4,
    /// “Protected-Mode Virtual Interrupts”.
    unsigned int virtual_interrupt_pending : 1;

    /// Identification (bit 21). — The ability of a program or procedure to set or clear this flag
    /// indicates support for the CPUID instruction.
    unsigned int identification : 1;

    unsigned int reserved5 : 10;
}
__attribute__((packed)) eflags_t;

// pub const Self = @This();

/// read eflags register
static inline eflags_t eflags_read()
{
    eflags_t res;

    asm volatile(
        "pushfl\n"
        "pop %0\n"
        : "=r"(res)::"memory");

    return res;
}

static inline void write(eflags_t self)
{
    asm volatile(
        "pushl %0\n"
        "popfl\n"
        :
        : "r"(self)
        : "memory");
}

/// The GDT pointer structure that contains the pointer to the beginning of the GDT and the number
/// of the table (minus 1). Used to load the GDT with LGDT instruction.
///
/// When the GDTR register is stored (using the SGDT instruction), a 48-bit “pseudo-descriptor”
/// is stored in memory (see top diagram in Figure 3-11). To avoid alignment check faults in user
/// mode (privilege level 3), the pseudo-descriptor should be located at an odd word address (that
/// is, address MOD 4 is equal to 2). This causes the processor to store an aligned word, followed
/// by an aligned doubleword. User-mode programs normally do not store pseudo-descriptors, but
/// the possibility of generating an alignment check fault can be avoided by aligning pseudo-
/// descriptors in this way. The same alignment should be used when storing the IDTR register
/// using the SIDT instruction. When storing the LDTR or task register (using the SLTR or STR
/// instruction, respectively), the pseudo-descriptor should be located at a doubleword address (that
/// is, address MOD 4 is equal to 0).
typedef struct
{
    /// 16bit entry for the number of entries (minus 1).
    unsigned int limit : 16;

    /// 32bit entry for the base address for the GDT.
    unsigned int base : 32;
}
__attribute__((packed))
pseudo_descriptor_t;

/// The GDTR register holds the base address (32 bits in protected mode; 64 bits in IA-32e mode)
/// and the 16-bit table limit for the GDT. The base address specifies the linear address of byte 0 of
/// the GDT; the table limit specifies the number of bytes in the table.
/// The LGDT and SGDT instructions load and store the GDTR register, respectively. On power up
/// or reset of the processor, the base address is set to the default value of 0 and the limit is set to
/// 0FFFFH. A new base address must be loaded into the GDTR as part of the processor initializa-
/// tion process for protected-mode operation.
/// See also: Section 3.5.1, “Segment Descriptor Tables”.
static inline pseudo_descriptor_t gdtr_read()
{
    pseudo_descriptor_t res = (pseudo_descriptor_t){.limit = 0, .base = 0};
    asm volatile(
        "sgdt %0\n"
        : "=m"(res)::"memory");
    return res;
}

typedef enum
{
    gdt = 0,
    ldt = 1
} table_indicator_t;
/// A segment selector is a 16-bit identifier for a segment (see Figure 3-6). It does not point directly
/// to the segment, but instead points to the segment descriptor that defines the segment. A segment
/// selector contains the following items:
typedef struct
{
    /// **Requested Privilege Level (RPL)**
    /// (Bits 0 and 1) — Specifies the privilege level of the selector. The privilege level
    /// can range from 0 to 3, with 0 being the most privileged level. See Section 4.5,
    /// “Privilege Levels”, for a description of the relationship of the RPL to the CPL
    /// of the executing program (or task) and the descriptor privilege level (DPL) of
    /// the descriptor the segment selector points to.
    unsigned requested_privilege_level : 2;
    /// **TI (table indicator) flag**
    /// (Bit 2) — Specifies the descriptor table to use: clearing this flag selects the
    /// GDT; setting this flag selects the current LDT.
    unsigned int table_indicator : 1;
    /// **Index**
    /// (Bits 3 through 15) — Selects one of 8192 descriptors in the GDT or LDT. The
    /// processor multiplies the index value by 8 (the number of bytes in a segment
    /// descriptor) and adds the result to the base address of the GDT or LDT (from
    /// the GDTR or LDTR register, respectively).
    unsigned index : 13;
}
__attribute__((packed))
segment_selector_t;

static inline void gdtr_write(pseudo_descriptor_t pd,
                              segment_selector_t kdata,
                              segment_selector_t kcode)
{
    asm volatile(
        "lgdt %0\n"
        :
        : "m"(pd)
        : "memory");

    asm volatile(
        "mov %%esp, %%ebx\n"
        "sub $6, %%esp\n"
        "movw %0, 4(%%esp)\n"
        "movl $mukwenjele, (%%esp)\n"
        "ljmp *(%%esp)\n"
        "mukwenjele:\n"
        "add $6, %%esp\n" ::
            "r"(kcode) : "memory", "ebx");

    asm volatile(
        "movw %0, %%bx\n"
        "movw %%bx, %%ds\n"
        "movw %%bx, %%es\n"
        "movw %%bx, %%fs\n"
        "movw %%bx, %%gs\n"
        "movw %%bx, %%ss\n" ::
            "m"(kdata) : "memory", "ebx");
}

/// The LDTR register holds the 16-bit segment selector, base address (32 bits in protected mode;
/// 64 bits in IA-32e mode), segment limit, and descriptor attributes for the LDT. The base address
/// specifies the linear address of byte 0 of the LDT segment; the segment limit specifies the number
/// of bytes in the segment. See also: Section 3.5.1, “Segment Descriptor Tables”.
/// The LLDT and SLDT instructions load and store the segment selector part of the LDTR register,
/// respectively. The segment that contains the LDT must have a segment descriptor in the GDT.
/// When the LLDT instruction loads a segment selector in the LDTR: the base address, limit, and
/// descriptor attributes from the LDT descriptor are automatically loaded in the LDTR.
/// When a task switch occurs, the LDTR is automatically loaded with the segment selector and
/// descriptor for the LDT for the new task. The contents of the LDTR are not automatically saved
/// prior to writing the new LDT information into the register.
/// On power up or reset of the processor, the segment selector and base address are set to the default
/// value of 0 and the limit is set to 0FFFFH.
static inline pseudo_descriptor_t ldt_read()
{
    pseudo_descriptor_t res = (pseudo_descriptor_t){.limit = 0, .base = 0};
    asm volatile(
        "sldt %0\n"
        : "=m"(res)::"memory");
    return res;
}

static inline void ldt_write(pseudo_descriptor_t pd)
{
    asm volatile(
        "lldt (%0)\n"
        :
        : "m"(pd) : "memory");
}

/// The IDTR register holds the base address (32 bits in protected mode; 64 bits in IA-32e mode)
/// and 16-bit table limit for the IDT. The base address specifies the linear address of byte 0 of the
/// IDT; the table limit specifies the number of bytes in the table. The LIDT and SIDT instructions
/// load and store the IDTR register, respectively. On power up or reset of the processor, the base
/// address is set to the default value of 0 and the limit is set to 0FFFFH. The base address and limit
/// in the register can then be changed as part of the processor initialization process.
/// See also: Section 5.10, “Interrupt Descriptor Table (IDT)”.

static inline pseudo_descriptor_t idtr_read()
{
    pseudo_descriptor_t res = (pseudo_descriptor_t){.limit = 0, .base = 0};
    asm volatile("sidt %0\n"
                 : "=m"(res)::"memory");
    return res;
}

static inline void idtr_write(pseudo_descriptor_t pd)
{
    asm volatile(
        "lidt %0\n"
        :
        : "m"(pd) : "memory");
}

/// The task register holds the 16-bit segment selector, base address (32 bits in protected mode; 64
/// bits in IA-32e mode), segment limit, and descriptor attributes for the TSS of the current task.
/// The selector references the TSS descriptor in the GDT. The base address specifies the linear
/// address of byte 0 of the TSS; the segment limit specifies the number of bytes in the TSS. See
/// also: Section 6.2.4, “Task Register”.
/// The LTR and STR instructions load and store the segment selector part of the task register,
/// respectively. When the LTR instruction loads a segment selector in the task register, the base
/// address, limit, and descriptor attributes from the TSS descriptor are automatically loaded into
/// the task register. On power up or reset of the processor, the base address is set to the default value
/// of 0 and the limit is set to 0FFFFH.
/// When a task switch occurs, the task register is automatically loaded with the segment selector
/// and descriptor for the TSS for the new task. The contents of the task register are not automati-
/// cally saved prior to writing the new TSS information into the register.
///
/// The task register holds the 16-bit segment selector and the entire segment descriptor (32-bit base
/// address, 16-bit segment limit, and descriptor attributes) for the TSS of the current task (see
/// Figure 2-5). This information is copied from the TSS descriptor in the GDT for the current task.
/// Figure 6-5 shows the path the processor uses to access the TSS (using the information in the task
/// register).
/// The task register has a visible part (that can be read and changed by software) and an invisible
/// part (maintained by the processor and is inaccessible by software). The segment selector in the
/// visible portion points to a TSS descriptor in the GDT. The processor uses the invisible portion
/// of the task register to cache the segment descriptor for the TSS. Caching these values in a
/// register makes execution of the task more efficient. The LTR (load task register) and STR (store
/// task register) instructions load and read the visible portion of the task register:
/// The LTR instruction loads a segment selector (source operand) into the task register that points
/// to a TSS descriptor in the GDT. It then loads the invisible portion of the task register with infor-
/// mation from the TSS descriptor. LTR is a privileged instruction that may be executed only when
/// the CPL is 0. It’s used during system initialization to put an initial value in the task register.
/// Afterwards, the contents of the task register are changed implicitly when a task switch occurs.
/// The STR (store task register) instruction stores the visible portion of the task register in a
/// general-purpose register or memory. This instruction can be executed by code running at any
/// privilege level in order to identify the currently running task. However, it is normally used only
/// by operating system software.
/// On power up or reset of the processor, segment selector and base address are set to the default
/// value of 0; the limit is set to FFFFH.
static inline segment_selector_t tr_read()
{
    segment_selector_t res;
    asm volatile("str %0\n"
                 : "=m"(res)::"memory");
    return res;
}

static inline void tr_write(segment_selector_t ss)
{
    asm volatile(
        "ltr %0\n"
        :
        : "r"(ss) : "memory");
}

/// The processor state information needed to restore a task is saved in a system segment called the
/// task-state segment (TSS).
typedef struct
{
    /// Previous task link field — Contains the segment selector for the TSS of the previous task
    /// (updated on a task switch that was initiated by a call, interrupt, or exception). This field
    /// (which is sometimes called the back link field) permits a task switch back to the previous
    /// task by using the IRET instruction.
    uint16_t previous_task_link;
    uint16_t reserved0;

    /// **Privilege level 0,stack pointer** — This stack pointer consist of a
    /// logical address made up of the segment selector for the stack segment (SS0)
    /// and an offset into the stack (ESP0). Note that the values in this field
    /// is static for a particular task; whereas, the SS and ESP values will change if stack
    /// switching occurs within the task.
    uint32_t esp0;
    segment_selector_t ss0;
    uint16_t reserved1;
    /// **Privilege level 1,stack pointer** — This stack pointer consist of a
    /// logical address made up of the segment selector for the stack segment (SS1)
    /// and an offset into the stack (ESP1). Note that the values in this field
    /// is static for a particular task; whereas, the SS and ESP values will change if stack
    /// switching occurs within the task.
    uint32_t esp1;
    segment_selector_t ss1;
    uint16_t reserved2;
    /// **Privilege level 2,stack pointer** — This stack pointer consist of a
    /// logical address made up of the segment selector for the stack segment (SS2)
    /// and an offset into the stack (ESP2). Note that the values in this field
    /// is static for a particular task; whereas, the SS and ESP values will change if stack
    /// switching occurs within the task.
    uint32_t esp2;
    segment_selector_t ss2;
    uint16_t reserved3;

    /// **CR3 control register field** — Contains the base physical address of the page directory to
    /// be used by the task. Control register CR3 is also known as the page-directory base register
    /// (PDBR).
    cr3_t cr3;
    /// EIP (instruction pointer) field — State of the EIP register prior to the task switch.
    uint32_t eip;
    /// EFLAGS register field — State of the EFAGS register prior to the task switch.
    eflags_t eflags;

    // General-purpose register fields — State of the EAX, ECX, EDX, EBX, ESP, EBP, ESI,
    // and EDI registers prior to the task switch.
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    // Segment selector fields — Segment selectors stored in the ES, CS, SS, DS, FS, and GS
    // registers prior to the task switch.
    segment_selector_t es;
    uint16_t reserved4;

    segment_selector_t cs;
    uint16_t reserved5;

    segment_selector_t ss;
    uint16_t reserved6;

    segment_selector_t ds;
    uint16_t reserved7;

    segment_selector_t fs;
    uint16_t reserved8;

    segment_selector_t gs;
    uint16_t reserved9;
    /// LDT segment selector field — Contains the segment selector for the task's LDT.
    segment_selector_t ldt_segment_selector;
    uint16_t reserved10;
    /// **T (debug trap) flag (byte 100, bit 0)** — When set, the T flag causes the processor to raise
    /// a debug exception when a task switch to this task occurs (see Section 15.3.1.5, “Task-
    /// Switch Exception Condition”).
    // T: bool = false,
    uint16_t reserved11;
    /// **I/O map base address field** — Contains a 16-bit offset from the base of the TSS to the I/O
    /// permission bit map and interrupt redirection bitmap. When present, these maps are stored
    /// in the TSS at higher addresses. The I/O map base address points to the beginning of the I/O
    /// permission bit map and the end of the interrupt redirection bit map. See Chapter 13,
    /// Input/Output, in the IA-32 Intel Architecture Software Developer’s Manual, Volume 1,
    /// for more information about the I/O permission bit map. See Section 16.3, “Interrupt and
    /// Exception Handling in Virtual-8086 Mode”, for a detailed description of the interrupt
    /// redirection bit map.
    uint16_t io_map_base_address;
} task_state_segment_t;

static inline void io_wait()
{
    port_byte_out(0x80, 0);
}

/// Interrupt and trap gates are very similar to call gates (see Section 4.8.3, “Call Gates”). They
/// contain a far pointer (segment selector and offset) that the processor uses to transfer program
/// execution to a handler procedure in an exception- or interrupt-handler code segment. These gates
/// differ in the way the processor handles the IF flag in the EFLAGS register (see Section 5.12.1.2,
/// “Flag Usage By Exception- or Interrupt-Handler Procedure”).
typedef struct
{
    /// Offset to procedure entry point
    unsigned int offset_low : 16;
    /// Segment Selector for destination code segment
    ///  The segment selector for the gate
    /// points to a segment descriptor for an executable code segment in either the GDT or the current
    /// LDT
    segment_selector_t segment_selector;
    unsigned int reserved0 : 5;
    unsigned int b0 : 3;
    unsigned int b1 : 3;
    // 0b110,
    unsigned int size : 1;
    unsigned int b2 : 1;
    unsigned int dpl : 2;
    unsigned int present : 1;
    unsigned int offset_high : 16;
}
__attribute__((packed))
interrupt_gate_t;

static inline interrupt_gate_t interrupt_gate_init(
    uint32_t proc_entry_point,
    segment_selector_t segment_selector,
    uint8_t previlege)
{
    return (interrupt_gate_t){
        .offset_low = proc_entry_point,
        .segment_selector = segment_selector,
        .dpl = previlege,
        .offset_high = proc_entry_point >> 16,
        .size = 1,
        .present = 1,
        .b0 = 0,
        .b1 = 0b110,
        .b2 = 0,
        .reserved0 = 0,
    };
}

static inline uint32_t interrupt_gate_get_offset(interrupt_gate_t *self)
{
    return (self->offset_high << 16) | self->offset_low;
}

static inline void interrupt_gate_set_offset(interrupt_gate_t *self, uint32_t value)
{
    self->offset_high = value >> 16;
    self->offset_low = value;
}

typedef struct
{
    segment_selector_t selector;
    unsigned int ass : 16;
} __attribute__((packed))
pushed_selector_t;

typedef struct
{
    // Page directory
    cr3_t cr3;

    // Extra segments
    pushed_selector_t gs;
    pushed_selector_t fs;
    pushed_selector_t es;
    pushed_selector_t ds;

    // Destination, source, base pointer
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;

    // General registers
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // Interrupt number and error code
    uint32_t interrupt_number;

    // defined
    uint32_t error_code;
    // if (std.mem.eql(u8, "#PF", interrupt.mnemonic)) PageFaultErrorCode else ErrorCode,
    uint32_t eip;
    pushed_selector_t cs;
    // cs_high: u16,
    eflags_t eflags;
    // if privilege changes
    uint32_t user_esp;
    uint32_t user_ss;
} cpu_context_t;

static inline void invalidate_page(uint32_t page_ptr) {
    asm volatile("invlpg (%0)"::"r" (page_ptr):"memory");
}