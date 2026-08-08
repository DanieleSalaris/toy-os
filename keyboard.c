#include <stdint.h>
#include "bool.h"
#include "vga.h"
#include "ctype.h"
#define CS3_PREFIX 0xE0
#define KT_SIZE 0x40
#define UO_LOCK_BIT 1
#define UO_SHIFT_BIT 2
char keyboard_pressed_keys[KT_SIZE] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'',  0,  0,   0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0,   '+',  0,    0,  'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l',  0,   0,  '<', 0,    0,  'z',  'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '-',  0,   0,   0,  ' ',  0,    0,   0,    0,   0,   0
};

bool_t cs3_prefix_on = FALSE;
uint8_t uppercase_on = 0;

void keyboard_handle_special_chars(uint8_t scancode) {
    switch (scancode) {
        case 0x1C: // enter
            terminal_breakline();
            break;
        case 0x3A: // lock click
            uppercase_on ^= UO_LOCK_BIT;
            break;
        case 0x2A: // shift pressed
            uppercase_on |= UO_SHIFT_BIT;
            break;
        case 0xAA: // shift unpressed
            uppercase_on &= ~UO_SHIFT_BIT;
            break;
    }
}

void idt_keyboard_input_handler(uint8_t scancode) {
    if (scancode == CS3_PREFIX) {
        cs3_prefix_on = TRUE;
        return;
    }
    if (cs3_prefix_on && scancode == 0x5C) {
        terminal_putchar(' ');
    }
    cs3_prefix_on = FALSE;
    char c;
    if (scancode >= KT_SIZE) {
        c = 0;
    } else {
        c = keyboard_pressed_keys[scancode];
    }
    if (c == 0) {
        keyboard_handle_special_chars(scancode);
        return;
    }
    if (uppercase_on) {
        c = toupper(c);
    }
    terminal_putchar(c);
}

