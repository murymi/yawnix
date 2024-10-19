#pragma once

#include "i386.h"

static inline 
void pic_write_data_master(unsigned char data){
    port_byte_out(0x21, data);
}

static inline
void pic_write_data_slave(unsigned char data) {
    port_byte_out(0xa1, data);
}

static inline 
void pic_write_cmd_master(unsigned char data){
    port_byte_out(0x20, data);
}

static inline
void pic_write_cmd_slave(unsigned char data) {
    port_byte_out(0xa0, data);
}

static inline 
unsigned char pic_read_data_master(){
    return port_byte_in(0x21);
}

static inline
unsigned char pic_read_data_slave() {
    return port_byte_in(0xa1);
}

static inline 
void pic_write_eoi(unsigned char idx) {
    if(idx > 8){
        pic_write_cmd_slave(0x20);
    }
    pic_write_cmd_master(0x20);
}

static inline 
unsigned char pic_read_master_irr(){
    pic_write_cmd_master(0x08 | 0x02 | 0x0);
    return port_byte_in(0x20);
}

static inline 
unsigned char pic_read_slave_irr(){
    pic_write_cmd_slave(0x08 | 0x02 | 0x0);
    return port_byte_in(0xa0);
}


static inline 
unsigned char pic_read_master_isr(){
    pic_write_cmd_master(0x08 | 0x02 | 0x1);
    return port_byte_in(0x20);
}

static inline 
unsigned char pic_read_slave_isr(){
    pic_write_cmd_slave(0x08 | 0x02 | 0x1);
    return port_byte_in(0xa0);
}

void pic_clear_mask(unsigned char idx);

void pic_remap();

int pic_spurious(unsigned char idx);