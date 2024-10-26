#include "./gdt.h"
#include "drivers/vga.h"
#include "./debug.h"
#include <string.h>

segment_descriptor_t gdt_entries[6];
task_state_segment_t tss;

void tss_set_stack0(uint32_t stack) {
    tss.esp0 = stack;
}

void gdt_init(uint32_t kstack_top) {

    gdt_entries[0] = segment_descriptor_zero();

    gdt_entries[1] = segment_descriptor_init(
        0, 0xfffff, (segment_type_t){
            .code = (segment_type_code_t){
                .accessed = 0,
                .conforming = 0,
                .is_code = 1,
                .read_enabled = 1
            }
        }, (segment_flags_t){
            .avl = 0,
            .db = 1,
            .descriptor_type = 1,
            .granularity = 1,
            .l = 0,
            .limit_high = 0xf,
            .present = 1,
            .privilege_level = 0
        }
    );

    gdt_entries[2] = segment_descriptor_init(
        0, 0xfffff, (segment_type_t){
            .data = (segment_type_data_t){
                .accessed = 0,
                .expand_down = 0,
                .is_code = 0,
                .write_enabled = 1
            }
        }, (segment_flags_t){
            .avl = 0,
            .db = 1,
            .descriptor_type = 1,
            .granularity = 1,
            .l = 0,
            .limit_high = 0xf,
            .present = 1,
            .privilege_level = 0,    
        }
    );

    gdt_entries[3] = segment_descriptor_init(
        0, 0xfffff, (segment_type_t){
            .code = (segment_type_code_t){
                .accessed = 0,
                .conforming = 0,
                .is_code = 1,
                .read_enabled = 1
            }
        }, (segment_flags_t){
            .avl = 0,
            .db = 1,
            .descriptor_type = 1,
            .granularity = 1,
            .l = 0,
            .limit_high = 0xf,
            .present = 1,
            .privilege_level = 3
        }
    );

    gdt_entries[4] = segment_descriptor_init(
        0, 0xfffff, (segment_type_t){
            .data = (segment_type_data_t){
                .accessed = 0,
                .expand_down = 0,
                .is_code = 0,
                .write_enabled = 1
            }
        }, (segment_flags_t){
            .avl = 0,
            .db = 1,
            .descriptor_type = 1,
            .granularity = 1,
            .l = 0,
            .limit_high = 0xf,
            .present = 1,
            .privilege_level = 3
        }
    );

    gdt_entries[5] = segment_descriptor_init(
        (uint32_t)&tss,
        sizeof(tss)-1,
        (segment_type_t){
            .system = segment_type_system_tss32available()
        },
        (segment_flags_t) {
            .granularity = 0,
            .avl = 0,
            .db = 1,
            .descriptor_type = 0,
            .l = 0,
            .present = 1,
            .privilege_level = 3,
            .limit_high = 0
        }
    );


    pseudo_descriptor_t pd = (pseudo_descriptor_t){
        .base = (uint32_t) &gdt_entries, .limit = sizeof(gdt_entries)-1 
    };
     

    gdtr_write(pd,
    (segment_selector_t){.index = 2, .requested_privilege_level = 0, .table_indicator = 0},
    (segment_selector_t){.index = 1, .requested_privilege_level = 0, .table_indicator = 0}
    );

    pseudo_descriptor_t tmp = gdtr_read();

    assert(tmp.base == (uint32_t) &gdt_entries, "gdt base not set correctly");
    assert(tmp.limit == sizeof(gdt_entries)-1, "gdt limit not set correctly");


    tss.ss0 = (segment_selector_t){.index = 2, .requested_privilege_level = 0, .table_indicator = 0};
    //tss.esp0 = kstack_top;
    tss.cs = (segment_selector_t){.index = 1, .requested_privilege_level = 3, .table_indicator = 0};
    tss.ds = tss.ss = tss.es = tss.fs = tss.gs = (segment_selector_t){.index = 2, .requested_privilege_level = 3, .table_indicator = 0};

    segment_selector_t tsel = 
    (segment_selector_t){.index = 5, .requested_privilege_level = 0, .table_indicator = 0};
    tr_write(tsel);

    segment_selector_t t2 = tr_read();
    assert(*((uint16_t *) &t2) == 0x28, "task register not set correctly");
    
}
