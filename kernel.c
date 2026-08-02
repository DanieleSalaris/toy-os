#include <stdint.h>
#include <stddef.h>
#include "gdt.h"
#include "idt.h"
#include "vga.h"

extern void pic_remap(void);

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
