#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "vga.h"
#include "asm.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CHAR_COLOR 0x07
#define HEX_BUF_SIZE 8
#define INT_BUF_SIZE 16
#define CLEAR_CELL 0x0720

char hex_mapper[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

volatile uint16_t* vga = (volatile uint16_t*) 0XB8000;

typedef struct {
    uint16_t row;
    uint16_t col;
    uint16_t pos;
} vga_pos_t;

vga_pos_t vga_pos = { 0, 0, 0 };

struct vga_cell {
    uint8_t c;
    uint8_t color;
};


void fill_screen() {
    for (int i=0; i<(VGA_HEIGHT - 1)*VGA_WIDTH; i++) {
        terminal_putchar(hex_mapper[i%16]);
    }
}

// Set position value to hardware registers
// port 0x3D4 point to the register 0x0E contains high value while 0x0F contains low value
// poer 0x3D5 set the selected register value
void terminal_set_cursor_position(uint16_t position) {
    asm_out(0x3D4, 0x0E);
    asm_out(0x3D5, (uint8_t)(position>>8));
    asm_out(0x3D4, 0x0F);
    asm_out(0x3D5, (uint8_t) position);
}

void set_vga_pos(uint16_t row, uint16_t col) {
    vga_pos.row = row;
    vga_pos.col = col;
    vga_pos.pos = row * VGA_WIDTH + col;
    terminal_set_cursor_position(vga_pos.pos);
}

void inc_vga_pos() {
    vga_pos.col++;
    if (vga_pos.col < VGA_WIDTH) {
        vga_pos.pos++;
        terminal_set_cursor_position(vga_pos.pos);
        return;
    }
    terminal_breakline();
}

void terminal_scrolldown() {
    memmove((void*) vga, (void*) (vga+VGA_WIDTH), sizeof(*vga) * (VGA_HEIGHT-1) * VGA_WIDTH);
    size_t position = (VGA_HEIGHT-1) * VGA_WIDTH;
    for (int i=0; i<VGA_WIDTH; i++) {
        vga[position++] = CLEAR_CELL;
    }
}

void terminal_breakline() {
    uint16_t row = vga_pos.row + 1;
    if (row >= VGA_HEIGHT) {
        row --;
        terminal_scrolldown();
    }
    set_vga_pos(row, 0);
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
    if (c == '\n') {
        terminal_breakline();
        return;
    }
    struct vga_cell *position = (struct vga_cell*) (vga + vga_pos.pos);
    position->c = (uint8_t) c;
    position->color = VGA_CHAR_COLOR;
    inc_vga_pos();
}

void terminal_printstring(const char *s) {
    while (*s != '\0') {
        terminal_putchar(*s);
        s++;
    }
}

