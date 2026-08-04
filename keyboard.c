#include <stdint.h>
#include "bool.h"
#include "vga.h"
#define CS3_PREFIX 0xE0
#define KT_SIZE 0x40
char keyboard_pressed_keys[KT_SIZE] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'',  0,  0,   0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0,   '+',  0,    0,  'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l',  0,   0,  '<', 0,    0,  'z',  'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '-',  0,   0,   0,   0,  0,    0,   0,    0,   0,   0
};

bool_t cs3_prefix_on = FALSE;


void idt_keyboard_input_handler(uint8_t scancode) {
    if (scancode == CS3_PREFIX) {
        cs3_prefix_on = TRUE;
        return;
    }
    if (cs3_prefix_on && scancode == 0x5C) {
        terminal_putchar(' ');
    }
    cs3_prefix_on = FALSE;
    if (scancode >= KT_SIZE) {
        return;
    }
    char c = keyboard_pressed_keys[scancode];
    if (c != 0) {
        terminal_putchar(c);
    }
}

