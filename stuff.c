#include "src/i386.h"
#include <stdio.h>
#include <stdint.h>
#include<stdarg.h>



int main(int argc, char const *argv[])
{
    segment_descriptor_t sdt = segment_descriptor_init(
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

    segment_descriptor_t sdt2 = segment_descriptor_init(
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

    //sdt = segment_descriptor_zero();

    printf("a: %lld, b: %lld\n", *((long long *) &sdt), *((long long *) &sdt2));
    return 0;
}
