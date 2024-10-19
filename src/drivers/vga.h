#pragma once

#include<stdarg.h>

#define VGA_SCREEN_CONTROL_REGISTER     0x3d4
#define VGA_SCREEN_DATA_REGISTER        0x3d5
#define VGA_HIGH_OFFSET                 0x0e
#define VGA_LOW_OFFSET                  0x0f

typedef enum {
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    magenta = 5,
    brown = 6,
    nightGray = 7,
    darkGray = 8,
    nightBlue = 9,
    nightGreen = 10,
    nightCyan = 11,
    nightRed = 12,
    nightMagenta = 13,
    nightBrown = 14,
    white = 15,
} vga_color_t;


typedef struct {
    unsigned char character;
    unsigned char attributes;
} __attribute__((packed))
__attribute__((aligned(2)))
vga_cell_t;

typedef struct {
    unsigned char x;
    unsigned char y;
} vga_pos_t;


void vga_init(unsigned int address);
void vga_print_char(vga_pos_t pos, char c);
vga_cell_t vga_cell_init(unsigned char c, vga_color_t fg, vga_color_t bg);
void vga_clear_screen(vga_color_t color);
void vga_scroll_up();
vga_pos_t vga_get_cursor();
void vga_set_cursor(vga_pos_t pos);
vga_pos_t vga_new_line(vga_pos_t old);
void vga_write(char *buf, unsigned int len);
void vga_printf(char *fmt, ...);
void vga_write_char(char c);
void vga_vprintf(char *fmt, __gnuc_va_list args);