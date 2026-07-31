#include "idt.h"
#include "gdt.h"
#include "vga.h"
#define IDT_N_ENTRY 256
// Flags
// 7 | 6 5 | 4 | 3 2 1 0 |
// P | RL  | 0 | GT      |
// P: Present bit (must be 1)
// RL: Ring level
// GT: Gate type 0b1110 32b interrupt, 0b1111 32b trqp
#define IDT_F_P 0x80
#define IDT_F_RLK 0x00
#define IDT_F_RLU 0x60
#define IDT_F_GTI 0x0E
#define IDT_F_GTT 0x0F
#define IDT_F_KI IDT_F_P | IDT_F_RLK | IDT_F_GTI
#define IDT_F_KT IDT_F_P | IDT_F_RLK | IDT_F_GTT

extern void asm_isr_division_by_zero(void);
extern void asm_isr_keyboard_input(void);


idt_descriptor_t idt_descriptor;
idt_entry_t idt[IDT_N_ENTRY];

// External assembly function to load IDT 
extern void idt_flush(uint32_t idt_descriptor_addr);

void kernel_panic(const char *msg) {
    terminal_printstring("KERNEL PANIC: ");
    terminal_printstring(msg);
    __asm__ volatile("cli; hlt");
}

void idt_generic_handler() {
    kernel_panic(""); 
}

void idt_division_by_zero_handler() {
    kernel_panic("Division by zero");        
}

void idt_keyboard_input_handler(uint8_t scancode) {

}

idt_entry_t idt_format_entry(uint32_t offset, uint16_t segment_selector, uint8_t flags) {
    idt_entry_t e;
    e.offset_low = (uint16_t) offset;
    e.offset_high = (uint16_t) (offset >> 16);
    e.segment_selector = segment_selector;
    e.flags = flags;
    e.reserved = 0;
    return e;
}

void idt_init() {
    idt_descriptor.base = (uint32_t) (&idt);
    idt_descriptor.limit = sizeof(idt_entry_t) * IDT_N_ENTRY - 1;
    idt[0] = idt_format_entry((uint32_t) asm_isr_division_by_zero, GDT_KERNEL_CODE_REF, IDT_F_KT);
    /*
    for (int i=1; i<IDT_N_ENTRY; i++) {
        idt[i] = idt_format_entry((uint32_t) idt_generic_handler, GDT_KERNEL_CODE_REF, 0);
    }
    */
    idt[33] = idt_format_entry((uint32_t) asm_isr_keyboard_input, GDT_KERNEL_CODE_REF, IDT_F_KI);
    idt_flush((uint32_t) (&idt_descriptor));
}
