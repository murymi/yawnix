#pragma once;

#include "i386.h"

static inline 
void pic_write_master(unsigned char data){
    port_byte_out(0x21, data);
}

static inline
void pic_write_slave(unsigned char data) {
    port_byte_out(0xa1, data);
}

void pic_clear_mask(unsigned char idx);

void pic_remap();