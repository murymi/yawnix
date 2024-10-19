#include "./gdt.h"
#include "drivers/vga.h"
#include "./debug.h"

segment_descriptor_t gdt_entries[6];


void gdt_init() {
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

    gdt_entries[1] = segment_descriptor_init(
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
            .privilege_level = 0
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

    pseudo_descriptor_t pd = (pseudo_descriptor_t){
        .base = (uint32_t) &gdt_entries,.limit = sizeof(gdt_entries)-1 
    };
    gdtr_write(pd);
    pseudo_descriptor_t tmp = gdtr_read();

    assert(tmp.base == (uint32_t) &gdt_entries, "gdt base not set correctly");
    assert(tmp.limit == sizeof(gdt_entries)-1, "gdt limit not set correctly");
}
