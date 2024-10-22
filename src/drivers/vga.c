#include "vga.h"
#include "../i386.h"
#include "../strings.h"

#include<stdarg.h>
#include<string.h>

volatile vga_cell_t *frame_buffer[25];
mutex_t vga_mutex;

void vga_init(unsigned int address)
{
    vga_cell_t *counter = (vga_cell_t *)address;
    for (int i = 0; i < 25; i++)
    {
        frame_buffer[i] = counter;
        counter += 80;
    }
    vga_clear_screen(black);
    vga_mutex = mutex_init();
}

vga_cell_t vga_cell_init(unsigned char c, vga_color_t fg, vga_color_t bg)
{
    return (vga_cell_t){
        .character = c,
        .attributes = (unsigned char)fg | (unsigned char)bg << 4};
}

void vga_print_char(vga_pos_t pos, char c)
{
    frame_buffer[pos.y][pos.x] = vga_cell_init(c, green, black);
}

void vga_clear_screen(vga_color_t color)
{
    const vga_cell_t cell = vga_cell_init(' ', green, color);
    for (int j = 0; j < 25; j++)
    {
        for (int i = 0; i < 80; i++)
        {
            frame_buffer[j][i] = cell;
        }
    }
    vga_set_cursor((vga_pos_t){.x = 0, .y = 0});
}

void vga_scroll_up()
{
    int current_line = 0;
    //int next_line = 1;

    for (; current_line < 24; current_line += 1)
    {
        for (int j = 0; j < 80; j += 1)
        {
            frame_buffer[current_line][j] = frame_buffer[current_line+1][j];
        }
    }

    vga_cell_t blank = vga_cell_init(' ', green, black);

    for (int j = 0; j < 80; j += 1)
    {
        frame_buffer[24][j] = blank;
    }
}

vga_pos_t vga_get_cursor()
{
    unsigned short result = 0;
    port_byte_out(VGA_SCREEN_CONTROL_REGISTER, VGA_HIGH_OFFSET);
    result |= port_byte_in(VGA_SCREEN_DATA_REGISTER);
    result <<= 8;
    port_byte_out(VGA_SCREEN_CONTROL_REGISTER, VGA_LOW_OFFSET);
    result |= port_byte_in(VGA_SCREEN_DATA_REGISTER);

    return (vga_pos_t){.x = result % 80, .y = result / 80};
}

static inline unsigned short get_offset(vga_pos_t pos)
{
    return ((pos.y * 80) + pos.x);
}

void vga_set_cursor(vga_pos_t pos)
{
    unsigned short offset = get_offset(pos);
    port_byte_out(VGA_SCREEN_CONTROL_REGISTER, VGA_HIGH_OFFSET);
    port_byte_out(VGA_SCREEN_DATA_REGISTER, offset >> 8);
    port_byte_out(VGA_SCREEN_CONTROL_REGISTER, VGA_LOW_OFFSET);
    port_byte_out(VGA_SCREEN_DATA_REGISTER, offset & 0xff);
}

vga_pos_t vga_new_line(vga_pos_t old)
{
    // vga_pos_t new = old;

    if (old.y >= 25)
    {
        vga_scroll_up();
        old.y -= 1;
    }

    if (old.x > 80)
    {
        old.x = 0;
        old.y += 1;

        if (old.y >= 25)
        {
            vga_scroll_up();
            old.y -= 1;
        }
    }

    return old;
}

void vga_write_char(char c) {
    vga_pos_t current_position = vga_get_cursor();

        switch (c)
        {
        case '\n':
            current_position.y += 1;
            current_position.x = 0;
            current_position = vga_new_line(current_position);
            break;

        case '\t':
            current_position.x += 1;
            current_position = vga_new_line(current_position);
            current_position.x += 1;
            current_position = vga_new_line(current_position);
            break;
        
        default:
            vga_print_char(current_position, c);
            current_position.x += 1;
            current_position = vga_new_line(current_position);
            break;
        }
        vga_set_cursor(current_position);
}

void vga_write(char *buf, unsigned int len) {
    vga_pos_t current_position = vga_get_cursor();

    for(unsigned int i = 0; i < len; i += 1) {
        vga_write_char(buf[i]);
    }
}

/// @brief  `%i` signed int
///
///`%u` unsigned int
///
///`%h` signed int hex
///
///`%x` unsigned int hex
///
///`%s` char *
///
///`%c` character
/// @param fmt 
/// @param  
void vga_printf(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vga_vprintf(fmt, args);
    va_end(args);
}

void vga_vprintf(char *fmt, __gnuc_va_list args) {
    mutex_lock(&vga_mutex);
    char buf[50] = {0};

    for(int i = 0;fmt[i]!=0; i++) {

        switch (fmt[i])
        {
        case '%':
            /* code */
            switch (fmt[i+1])
            {
            case 'i':
                int inumber = va_arg(args, int);
                int_to_string(buf, inumber);
                vga_write(buf, strlen(buf));
                i += 1;
                /* code */
                break;
            case 'u':
                unsigned int unumber = va_arg(args, unsigned int);
                uint_to_string(buf, unumber);
                vga_write(buf, strlen(buf));
                i += 1;
                /* code */
                break;
            case 'h':
                int hnumber = va_arg(args, int);
                int_to_string_hex(buf, hnumber);
                vga_write(buf, strlen(buf));
                i += 1;
                /* code */
                break;
            case 'x':
                unsigned int xnumber = va_arg(args, unsigned int);
                uint_to_string_hex(buf, xnumber);
                vga_write(buf, strlen(buf));
                i += 1;
                /* code */
                break;
            case 'c':
                char cnumber = (char)va_arg(args, int);
                vga_write_char(cnumber);
                i += 1;
                /* code */
                break;
            case 's':
                char *str = (char *)va_arg(args, char *);
                vga_write(str, strlen(str));
                i += 1;
                /* code */
                break;
            default:
                //vga_write("%", 1);
                vga_write_char(fmt[i]);
                break;
            }
            break;
        default:
            vga_write_char(fmt[i]);
            break;
        }
    }
    mutex_unlock(&vga_mutex);
}
