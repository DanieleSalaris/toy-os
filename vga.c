#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CHAR_COLOR 0x07
#define HEX_BUF_SIZE 8
#define INT_BUF_SIZE 16
#define CLEAR_CELL 0x0720

char hex_mapper[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

volatile uint16_t* vga = (volatile uint16_t*) 0XB8000;

size_t vga_row = 0;
size_t vga_col = 0;

struct vga_cell {
    uint8_t c;
    uint8_t color;
};


void fill_screen() {
    for (int i=0; i<(VGA_HEIGHT - 1)*VGA_WIDTH; i++) {
        terminal_putchar(hex_mapper[i%16]);
    }
}

void terminal_scrolldown() {
    memmove((void*) vga, (void*) (vga+VGA_WIDTH), sizeof(*vga) * (VGA_HEIGHT-1) * VGA_WIDTH);
    size_t position = (VGA_HEIGHT-1) * VGA_WIDTH;
    for (int i=0; i<VGA_WIDTH; i++) {
        vga[position++] = CLEAR_CELL;
    }
}


void terminal_putint(int num) {
    char intbuf[INT_BUF_SIZE];
    int i;
    for (i=0; i<INT_BUF_SIZE; i++) {
        intbuf[i] = hex_mapper[num % 10];
        num /= 10;
    }
    for (i=INT_BUF_SIZE-1; i>0 && intbuf[i]=='0'; i--);
    for (i=i; i>=0; i--) {
        terminal_putchar(intbuf[i]);
    }
}

void terminal_puthex(int num) {
    char hexbuf[HEX_BUF_SIZE]; 
    int i;
    for (i=0; i<HEX_BUF_SIZE; i++) {
       hexbuf[i] = hex_mapper[num % 16];
        num >>= 4; 
    }
    terminal_putchar('0');
    terminal_putchar('x');
    for (i=HEX_BUF_SIZE-1; i>0 && hexbuf[i]=='0'; i--);
    for (i=i; i>=0; i--) {
        terminal_putchar(hexbuf[i]);
    }
}

void terminal_putchar(char c) {
    struct vga_cell *position = (struct vga_cell*) (vga + (vga_col + vga_row * VGA_WIDTH));
    position->c = (uint8_t) c;
    position->color = VGA_CHAR_COLOR;
    vga_col++;
    if (vga_col < VGA_WIDTH) {
        return;
    }
    vga_col = 0;
    vga_row++;
    if (vga_row >= VGA_HEIGHT) {
        vga_row--;
        terminal_scrolldown();
    }
}

void terminal_printstring(const char *s) {
    while (*s != '\0') {
        terminal_putchar(*s);
        s++;
    }
}
