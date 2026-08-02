#include <stdint.h>
#include "vga.h"

char keyboard_pressed_keys[88] = {
    '*', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '-', '=', '*', '*', 'q', 'w', 'e', 'r', 't',
    'y', 'o', 'p', '[', ']', '*', '*', 's', 'a', 'd',
    'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '*',
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',
    '/'
};

void idt_keyboard_input_handler(uint8_t scancode) {
    //if (scancode > 41) return;
    // terminal_putchar(keyboard_pressed_keys[scancode - 1]);
    terminal_put_hex(scancode);
    terminal_putchar(' ');
}
