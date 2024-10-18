 #pragma once

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
typedef struct {
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



//inline cr3_t read_cr3();
__attribute__((section(".boot.text")))
static inline cr3_t read_cr3_boot() {
    //alignof(unsigned int);
    cr3_t result;
    asm volatile (
        "mov %%cr3, %0"
        : "=r" (result)
    );
    return result;
}

__attribute__((section(".boot.text")))
static inline void write_cr3_boot(cr3_t new) {
    asm volatile (
        "mov %0, %%cr3"
        : : "r" (new)
    );
}


/* 
CR0 — Contains system control flags that control operating mode and states of the
 processor. 
*/
typedef struct {
    /// Protection Enable (bit 0 of CR0) — Enables protected mode when set; enables real-
    /// address mode when clear. This flag does not enable paging directly. It only enables
    /// segment-level protection. To enable paging, both the PE and PG flags must be set.
    /// See also: Section 9.9, “Mode Switching”.
    unsigned int protection: 1;
    /// Monitor Coprocessor (bit 1 of CR0). — Controls the interaction of the WAIT (or
    /// FWAIT) instruction with the TS flag (bit 3 of CR0). If the MP flag is set, a WAIT
    /// instruction generates a device-not-available exception (#NM) if the TS flag is also set.
    /// If the MP flag is clear, the WAIT instruction ignores the setting of the TS flag. Table 9-2
    /// shows the recommended setting of this flag, depending on the IA-32 processor and x87
    /// FPU or math coprocessor present in the system. Table 2-1 shows the interaction of the
    /// MP, EM, and TS flags.
    unsigned int monitor_coprocessor: 1;
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
    unsigned int emulation: 1;
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
    unsigned int task_switched: 1;
    /// Extension Type (bit 4 of CR0) — Reserved in the Pentium 4, Intel Xeon, P6 family,
    /// and Pentium processors. In the Pentium 4, Intel Xeon, and P6 family processors, this
    /// flag is hardcoded to 1. In the Intel386 and Intel486 processors, this flag indicates
    /// support of Intel 387 DX math coprocessor instructions when set.
    unsigned int extension_type: 1;
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
    unsigned int numeric_error: 1;
    unsigned int reserved0: 10;
    /// Write Protect (bit 16 of CR0) — Inhibits supervisor-level procedures from writing
    /// into user-level read-only pages when set; allows supervisor-level procedures to write
    /// into user-level read-only pages when clear. This flag facilitates implementation of the
    /// copy-on-write method of creating a new process (forking) used by operating systems
    /// such as UNIX*.
    unsigned int write_protect: 1;
    unsigned int reserved1: 1;
    /// Alignment Mask (bit 18 of CR0) — Enables automatic alignment checking when set;
    /// disables alignment checking when clear. Alignment checking is performed only when
    /// the AM flag is set, the AC flag in the EFLAGS register is set, CPL is 3, and the
    /// processor is operating in either protected or virtual-8086 mode.
    unsigned int alignment: 1;

    unsigned int reserved2: 10;
    /// Not Write-through (bit 29 of CR0) — When the NW and CD flags are clear, write-
    /// back (for Pentium 4, Intel Xeon, P6 family, and Pentium processors) or write-through
    /// (for Intel486 processors) is enabled for writes that hit the cache and invalidation cycles
    /// are enabled. See Table 10-5 for detailed information about the affect of the NW flag on
    /// caching for other settings of the CD and NW flags.
    unsigned int not_write_through: 1;
    /// Cache Disable (bit 30 of CR0) — When the CD and NW flags are clear, caching of
    /// memory locations for the whole of physical memory in the processor’s internal (and
    /// external) caches is enabled. When the CD flag is set, caching is restricted as described
    /// in Table 10-5. To prevent the processor from accessing and updating its caches, the CD
    /// flag must be set and the caches must be invalidated so that no cache hits can occur.
    /// See also: Section 10.5.3, “Preventing Caching” and Section 10.5, “Cache Control”.
    unsigned int cache_disable: 1;
    /// Paging (bit 31 of CR0) — Enables paging when set; disables paging when clear.
    /// When paging is disabled, all linear addresses are treated as physical addresses. The PG
    /// flag has no effect if the PE flag (bit 0 of register CR0) is not also set; setting the PG
    /// flag when the PE flag is clear causes a general-protection exception (#GP). See also:
    /// Section 3.6, “Paging (Virtual Memory) Overview”.
    /// On IA-32 processors that support Intel® EM64T, enabling and disabling IA-32e mode
    /// operation also requires modifying CR0.PG.
    unsigned int paging: 1;
} 
__attribute__((packed))
__attribute__((aligned(4)))
cr0_t;

//inline cr3_t read_cr3();
__attribute__((section(".boot.text")))
static inline cr0_t read_cr0_boot() {
    cr0_t result;
    asm volatile (
        "mov %%cr3, %0"
        : "=r" (result)
    );
    return result;
}

__attribute__((section(".boot.text")))
static inline void write_cr0_boot(cr0_t new) {
    asm volatile (
        "mov %0, %%cr0"
        : : "r" (new)
    );
}

/// CR4 — Contains a group of flags that enable several architectural extensions, and indicate
/// operating system or executive support for specific processor capabilities. The control
/// registers can be read and loaded (or modified) using the move-to-or-from-control-registers
/// forms of the MOV instruction. In protected mode, the MOV instructions allow the control
/// registers to be read or loaded (at privilege level 0 only). This restriction means that
/// application programs or operating-system procedures (running at privilege levels 1, 2, or
/// 3) are prevented from reading or loading the control registers.
typedef struct {
    /// Virtual-8086 Mode Extensions (bit 0 of CR4) — Enables interrupt- and exception-
    /// handling extensions in virtual-8086 mode when set; disables the extensions when clear.
    /// Use of the virtual mode extensions can improve the performance of virtual-8086 appli-
    /// cations by eliminating the overhead of calling the virtual-8086 monitor to handle inter-
    /// rupts and exceptions that occur while executing an 8086 program and, instead,
    /// redirecting the interrupts and exceptions back to the 8086 program’s handlers. It also
    /// provides hardware support for a virtual interrupt flag (VIF) to improve reliability of
    /// running 8086 programs in multitasking and multiple-processor environments.
    /// See also: Section 16.3, “Interrupt and Exception Handling in Virtual-8086 Mode”.
    unsigned int vitual8086_mode_extensions: 1;
    /// Protected-Mode Virtual Interrupts (bit 1 of CR4) — Enables hardware support for
    /// a virtual interrupt flag (VIF) in protected mode when set; disables the VIF flag in
    /// protected mode when clear.
    /// See also: Section 16.4, “Protected-Mode Virtual Interrupts”.
    unsigned int protected_mode_virtual_interrupts: 1;
    /// Time Stamp Disable (bit 2 of CR4) — Restricts the execution of the RDTSC instruc-
    /// tion to procedures running at privilege level 0 when set; allows RDTSC instruction to
    /// be executed at any privilege level when clear.
    unsigned int timestamp_disable: 1;
    /// Debugging Extensions (bit 3 of CR4) — References to debug registers DR4 and DR5
    /// cause an undefined opcode (#UD) exception to be generated when set; when clear,
    /// processor aliases references to registers DR4 and DR5 for compatibility with software
    /// written to run on earlier IA-32 processors.
    /// See also: Section 15.2.2, “Debug Registers DR4 and DR5”.
    unsigned int debugging_extensions: 1;
    /// Page Size Extensions (bit 4 of CR4) — Enables 4-MByte pages when set; restricts
    /// pages to 4 KBytes when clear.
    /// See also: Section 3.6.1, “Paging Options”.
    unsigned int page_size_extensions: 1;
    /// Physical Address Extension (bit 5 of CR4) — When set, enables paging mechanism
    /// to reference greater-or-equal-than-36-bit physical addresses. When clear, restricts
    /// physical addresses to 32 bits. PAE must be enabled to enable IA-32e mode operation.
    /// Enabling and disabling IA-32e mode operation also requires modifying CR4.PAE.
    /// See also: Section 3.8, “36-Bit Physical Addressing Using the PAE Paging
    /// Mechanism”.
    unsigned int physical_address_extension: 1;
    /// Machine-Check Enable (bit 6 of CR4) — Enables the machine-check exception
    /// when set; disables the machine-check exception when clear.
    /// See also: Chapter 14, Machine-Check Architecture.
    unsigned int machine_check_enable: 1;
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
    unsigned int page_global_enable: 1;
    /// Performance-Monitoring Counter Enable (bit 8 of CR4) — Enables execution of
    /// the RDPMC instruction for programs or procedures running at any protection level
    /// when set; RDPMC instruction can be executed only at protection level 0 when clear.
    unsigned int performance_monitoring_counter_enable: 1;
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
    unsigned int osfxsr: 1;
    /// Operating System Support for Unmasked SIMD Floating-Point Exceptions (bit 10
    /// of CR4) — When set, indicates that the operating system supports the handling of
    /// unmasked SIMD floating-point exceptions through an exception handler that is invoked
    /// when a SIMD floating-point exception (#XF) is generated. SIMD floating-point excep-
    /// tions are only generated by SSE/SSE2/SSE3 SIMD floating-point instructions.
    ///
    /// The operating system or executive must explicitly set this flag. If this flag is not set, the
    /// processor will generate an invalid opcode exception (#UD) whenever it detects an
    /// unmasked SIMD floating-point exception.
    unsigned int osxmmexcpt: 1;

    unsigned int res0: 21;
} 
__attribute__((packed))
__attribute__((aligned(4)))
 cr4_t;


__attribute__((section(".boot.text")))
static inline cr4_t read_cr4_boot() {
    cr4_t result;
    asm volatile (
        "mov %%cr4, %0"
        : "=r" (result)
    );
    return result;
}

__attribute__((section(".boot.text")))
static inline void write_cr4_boot(cr4_t new) {
    asm volatile (
        "mov %0, %%cr4"
        : : "r" (new)
    );
}

/// Linear Address Translation (4-KByte Pages)
typedef struct {
    /// **Page-offset**
    /// Bits 0 through 11 provides an offset to a physical address in the page.
    unsigned int page_offset: 12;
    /// **Page-table entry** — Bits 12 through 21 of the linear address provide an offset to an entry
    /// in the selected page table. This entry provides the base physical address of a page in
    /// physical memory.
    unsigned int page_table_entry: 10;
    /// **Page-directory entry** — Bits 22 through 31 provide an offset to an entry in the page
    /// directory. The selected entry provides the base physical address of a page table.
    unsigned int page_directory_entry: 10;
} 
__attribute__((packed)) 
__attribute__((aligned(4)))
linear_address_4k_t;


/// Linear Address Translation (4-MByte Pages)
typedef struct {
    /// Page offset—Bits 0 through 21 provides an offset to a physical address in the page.
    unsigned int page_offset: 22;
    /// **Page directory entry** —Bits 22 through 31 provide an offset to an entry in the page
    /// directory. The selected entry provides the base physical address of a 4-MByte page.
    unsigned int page_directory_entry: 10;
} __attribute__((packed)) 
__attribute__((aligned(4)))
linear_address_4m_t;

/// Format of Page-Directory and Page-Table Entries for 4-KByte Pages
/// and 32-Bit Physical Addresses
typedef struct {
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
    int present: 1;
    /// **Read/write (R/W) flag, bit 1**
    /// Specifies the read-write privileges for a page or group of pages (in the case of
    /// a page-directory entry that points to a page table). When this flag is clear, the
    /// page is read only; when the flag is set, the page can be read and written into.
    /// This flag interacts with the U/S flag and the WP flag in register CR0. See
    /// Section 4.11, “Page-Level Protection”, and Table 4-3 for a detailed discussion
    /// of the use of these flags.
    int read_write: 1;
    /// **User/supervisor (U/S) flag, bit 2**
    /// Specifies the user-supervisor privileges for a page or group of pages (in the
    /// case of a page-directory entry that points to a page table). When this flag is
    /// clear, the page is assigned the supervisor privilege level; when the flag is set,
    /// the page is assigned the user privilege level. This flag interacts with the R/W
    /// flag and the WP flag in register CR0. See Section 4.11, “Page-Level Protec-
    /// tion”, and Table 4-3 for a detail discussion of the use of these flags.
    int sepervisor: 1;
    /// **Page-level write-through (PWT) flag, bit 3**
    /// Controls the write-through or write-back caching policy of individual pages or
    /// page tables. When the PWT flag is set, write-through caching is enabled for the
    /// associated page or page table; when the flag is clear, write-back caching is
    /// enabled for the associated page or page table. The processor ignores this flag if
    /// the CD (cache disable) flag in CR0 is set. See Section 10.5, “Cache Control”,
    /// for more information about the use of this flag. See Section 2.5, “Control
    /// Registers”, for a description of a companion PWT flag in control register CR3.
    int page_level_write_through: 1;
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
    int page_level_cache_disable: 1;
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
    int accessed: 1;
    /// **bit 6**
    int reserved: 1;
    /// **Page size (PS) flag, bit 7 page-directory entries for 4-KByte pages**
    /// Determines the page size. When this flag is clear, the page size is 4 KBytes and
    /// the page-directory entry points to a page table. When the flag is set, the page
    /// size is 4 MBytes for normal 32-bit addressing (and 2 MBytes if extended phys-
    /// ical addressing is enabled) and the page-directory entry points to a page. If the
    /// page-directory entry points to a page table, all the pages associated with that
    /// page table will be 4-KByte pages.
    int is_page: 1;
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
    int global: 1;

    int available_bits: 3;

    /// (Page-directory entries for 4-KByte page tables) — Specifies the physical
    /// address of the first byte of a page table. The bits in this field are interpreted as
    /// the 20 most-significant bits of the physical address, which forces page tables to
    /// be aligned on 4-KByte boundaries.
    int page_table_base_address: 20;
}__attribute__((packed)) 
__attribute__((aligned(4)))
page_directory_entry_4k_t;

static inline void pde4k_set_page_table_base_address(page_directory_entry_4k_t * self, unsigned int address) {
    unsigned int *tmp = (unsigned int *)self;
    *tmp |= address;
}

static inline unsigned int pde4k_get_page_table_base_address(page_directory_entry_4k_t *self) {
    unsigned int *tmp = (unsigned int *)self;
    return (*tmp >> 12) << 12;
}

static inline page_directory_entry_4k_t pde4k_zero() {
    unsigned int tmp = 0;
    return *(page_directory_entry_4k_t *)&tmp;
}

/// Format of Page-Directory Entries for 4-MByte Pages and 32-Bit Addresses
typedef struct {
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
    int present: 1;
    /// **Read/write (R/W) flag, bit 1**
    /// Specifies the read-write privileges for a page or group of pages (in the case of
    /// a page-directory entry that points to a page table). When this flag is clear, the
    /// page is read only; when the flag is set, the page can be read and written into.
    /// This flag interacts with the U/S flag and the WP flag in register CR0. See
    /// Section 4.11, “Page-Level Protection”, and Table 4-3 for a detailed discussion
    /// of the use of these flags.
    int read_write: 1;
    /// **User/supervisor (U/S) flag, bit 2**
    /// Specifies the user-supervisor privileges for a page or group of pages (in the
    /// case of a page-directory entry that points to a page table). When this flag is
    /// clear, the page is assigned the supervisor privilege level; when the flag is set,
    /// the page is assigned the user privilege level. This flag interacts with the R/W
    /// flag and the WP flag in register CR0. See Section 4.11, “Page-Level Protec-
    /// tion”, and Table 4-3 for a detail discussion of the use of these flags.
    int sepervisor: 1;
    /// **Page-level write-through (PWT) flag, bit 3**
    /// Controls the write-through or write-back caching policy of individual pages or
    /// page tables. When the PWT flag is set, write-through caching is enabled for the
    /// associated page or page table; when the flag is clear, write-back caching is
    /// enabled for the associated page or page table. The processor ignores this flag if
    /// the CD (cache disable) flag in CR0 is set. See Section 10.5, “Cache Control”,
    /// for more information about the use of this flag. See Section 2.5, “Control
    /// Registers”, for a description of a companion PWT flag in control register CR3.
    int page_level_write_through: 1;
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
    int page_level_cache_disable: 1;
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
    int accsessed: 1;

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
    int dirty: 1;

    /// **Page size (PS) flag, bit 7 page-directory entries for 4-KByte pages**
    /// Determines the page size. When this flag is clear, the page size is 4 KBytes and
    /// the page-directory entry points to a page table. When the flag is set, the page
    /// size is 4 MBytes for normal 32-bit addressing (and 2 MBytes if extended phys-
    /// ical addressing is enabled) and the page-directory entry points to a page. If the
    /// page-directory entry points to a page table, all the pages associated with that
    /// page table will be 4-KByte pages.
    int is_page: 1;
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
    int global: 1;
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
    int available_bits: 3;
    /// **Page attribute table index (PAT) flag, bit 7 in page-table entries for 4-KByte pages and
    /// bit 12 in page-directory entries for 4-MByte pages**
    /// (Introduced in the Pentium III processor) — Selects PAT entry. For processors
    /// that support the page attribute table (PAT), this flag is used along with the
    /// PCD and PWT flags to select an entry in the PAT, which in turn selects the
    /// memory type for the page (see Section 10.12, “Page Attribute Table (PAT)”).
    /// For processors that do not support the PAT, this bit is reserved and should be
    /// set to 0.
    int page_attribute_table_index: 1;

    int reserved: 9;
    /// **(Page-directory entries for 4-MByte pages)** — Specifies the physical address
    /// of the first byte of a 4-MByte page. Only bits 22 through 31 of this field are
    /// used (and bits 12 through 21 are reserved and must be set to 0, for IA-32
    /// processors through the Pentium II processor). The base address bits are inter-
    /// preted as the 10 most-significant bits of the physical address, which forces
    /// 4-MByte pages to be aligned on 4-MByte boundaries.
    int page_base_address: 10;
} __attribute__((packed))
__attribute__((aligned(4)))
page_directory_entry_4m_t;

static inline void pde4m_set_page_base_address(page_directory_entry_4m_t * self, unsigned int address) {
    unsigned int *tmp = (unsigned int *)self;
    *tmp |= address;
}

static inline unsigned int pde4m_get_page_base_address(page_directory_entry_4m_t *self) {
    unsigned int *tmp = (unsigned int *)self;
    return (*tmp >> 22) << 22;
}

static inline page_directory_entry_4m_t pde4m_zero() {
    unsigned int tmp = 0;
    return *(page_directory_entry_4m_t *)&tmp;
}

__attribute__((section(".boot.text")))
static inline page_directory_entry_4m_t pde4m_boot_zero() {
    unsigned int tmp = 0;
    return *(page_directory_entry_4m_t *)&tmp;
}

typedef union {
    page_directory_entry_4m_t m4;
    page_directory_entry_4k_t k4;
} page_directory_entry_t;


static inline unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__(
        "in %%dx, %%al": "=a" (result) : "d" (port)
    );
    return result;
}


static inline void port_byte_out(unsigned short port, unsigned char data) {
    __asm__(
        "out %%al, %%dx" : :"a" (data), "d" (port)
    );
}