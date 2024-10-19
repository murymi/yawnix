#include "pic.h"

void pic_clear_mask(unsigned char idx)
{
    unsigned char port = 0x21;
    if(idx >= 8) port = 0xa1;
    int shift = idx % 8;
    int value = port_byte_in(port) & ~(1 << shift);
    port_byte_out(port, value);
}

void pic_remap() {
    pic_write_cmd_master(0x10 | 0x01);
    io_wait();
    pic_write_cmd_slave(0x10 | 0x01);
    io_wait();

    pic_write_data_master(0x20);
    io_wait();
    pic_write_data_slave(0x28);
    io_wait();

    pic_write_data_master(0x04);
    io_wait();
    pic_write_data_slave(0x02);
    io_wait();

    pic_write_data_master(0x01);
    io_wait();
    pic_write_data_slave(0x01);
    io_wait();

    pic_write_data_master(0xff);
    io_wait();
    pic_write_data_slave(0xff);
    io_wait();

    pic_clear_mask(0x02);
}

int pic_spurious(unsigned char idx) {
    if(idx == 7) {
        if(!(pic_read_master_isr() & 0x80)) return 1;
    } else if(idx == 15) {
        if(!(pic_read_slave_isr() & 0x80)) {
            pic_write_cmd_master(0x20);
            return 1;
        }
    }
    return 0;
}
