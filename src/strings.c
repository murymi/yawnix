#include "./strings.h"

static void string_rev(char *buf, unsigned int len) {
    int half = len/2;
    for(int j  = 0; j < half; j++) {
        char c = buf[j];
        buf[j] = buf[len-j-1];
        buf[len-j-1] = c;
    }
}

void uint_to_string(char *buf, unsigned long value) {
    uint32_t i = 0;
    if(value == 0) {
        buf[i] = '0';
        return;
    }
    while (value > 0)
    {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }
    buf[i] = 0;
    string_rev(buf, i);
}

void int_to_string(char *buf, long value) {
    uint32_t i = 0;
    int is_negative = 0;

    if(value < 0) {
        is_negative = 1;
        value = -value;
    }
    if(value == 0) {
        buf[i] = '0';
        return;
    }
    while (value > 0)
    {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }

    if(is_negative) buf[i++] = '-';
    buf[i] = 0;

    string_rev(buf, i);
}

void uint_to_string_hex(char *buf, unsigned long value) {
    uint32_t i = 0;
    if(value == 0) {
        buf[i] = '0';
        return;
    }
    while (value > 0)
    {
        int tmp = (value % 16);

        if(tmp >= 10) {
            buf[i++] = (tmp - 10) + 'a';
        } else {
            buf[i++] = tmp + '0';
        }
        value /= 16;
    }
    buf[i] = 0;
    string_rev(buf, i);
}

void int_to_string_hex(char *buf, long value) {
    uint32_t i = 0;
    int is_negative = 0;

    if(value < 0) {
        is_negative = 1;
        value = -value;
    }
    if(value == 0) {
        buf[i] = '0';
        return;
    }
    while (value > 0)
    {
        int tmp = (value % 16);
        if(tmp >= 10) {
            buf[i++] = (tmp - 10) + 'a';
        } else {
            buf[i++] = tmp + '0';
        }
        value /= 16;
    }

    if(is_negative) buf[i++] = '-';
    buf[i] = 0;

    string_rev(buf, i);
}