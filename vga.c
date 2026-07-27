#include <stdint.h>
#include <stddef.h>
#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CHAR_COLOR 0x07

volatile uint16_t* vga = (volatile uint16_t*) 0XB8000;

size_t vga_row = 0;
size_t vga_col = 0;

struct vga_cell {
    uint8_t c;
    uint8_t color;
};

void terminal_putchar(char c) {
    if (vga_row > VGA_HEIGHT || vga_col > VGA_WIDTH) {
        return;
    }
    struct vga_cell *position = (struct vga_cell*) (vga + (vga_col + vga_row * VGA_WIDTH));
    position->c = (uint8_t) c;
    position->color = VGA_CHAR_COLOR;
    vga_col++;
    if(vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
    }
}

   //terminal_printstring(const char *s);
void terminal_printstring(const char *s) {
    while (*s != '\0') {
        terminal_putchar(*s);
        s++;
    }
}
