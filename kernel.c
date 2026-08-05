#include <stdint.h>
#include <stddef.h>
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "string.h"

uint8_t buf[0x10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
extern void pic_remap(void);

void printbuf() {
    // terminal_printstring("| ");
    for (int i=0; i<0x10; i++) {
        terminal_puthex(buf[i]);
        terminal_putchar(' ');
    }
}

void kernel_main() {
    const char* msg = "Hello from my OS! ";
    gdt_init();
    idt_init();
    pic_remap();
    __asm__ volatile("sti");

    terminal_printstring(msg);

    /*

    volatile char a, b, c;
    a = 1;
    b = 0;
    c = a / b;
    terminal_putchar('c');
    terminal_putchar(':');
    terminal_putchar(c);
    */
    while (1) {
        __asm__ volatile ("hlt");
    }
}
