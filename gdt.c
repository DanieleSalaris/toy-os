#include "gdt.h"
#include <stdint.h>
#define GDT_N_ENTRY 3

gdt_t gdt_ref;
gdt_entry_t gdt[GDT_N_ENTRY]; 

// External assembly function to load GDT
extern void gdt_flush(uint32_t gdt_ptr_addr);

// A function that populates gdt entry
// base: base address 32bit filed that point the beginning of a memory segment
// limit: 20 bit field that indicates the size from the memory segment
// access: 8 bit field
// flag: 4 bit field
gdt_entry_t gdt_format_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    gdt_entry_t e;
    e.access = access;
    e.base_low = (uint16_t) base;
    e.base_middle = (uint8_t) (base >> 16);
    e.base_high = (uint8_t) (base >> 24);
    e.limit_low = (uint16_t) limit;
    e.granularity = (uint8_t) ((limit >> 16) & 0x0f);
    e.granularity |= ((flags << 4) & 0xf0);
    return e;
}

// Access field
// 7 | 6 5 | 4 | 3 | 2  | 1  | 0
// P | DPL | S | E | DC | RW | A
// P: Present bit, should only be 1
// DPL: ring level Kernel 00, User 11
// S: 0 system segment, 1 Data/Code segment
// E: 0 Data segment, 1 Code segment 
// DC
// RW: for code segment 1 enable read (write never allowed)
//     for data segment 1 enable write (read always allowed)
// A: put to 1 by CPU when memory is accessed

// flags
// 3 | 2  | 1 | 0
// G | DB | L | Reserved
// G: 0 byte granularity, 1 page granularity (4KiB block)
// DB: 0 16-bit, 1 32-bit
// L: 1 

void gdt_init(void) {
    gdt_ref.base = (uint32_t) (&gdt);
    gdt_ref.limit = sizeof(gdt_entry_t) * GDT_N_ENTRY - 1;

    // Null entry (required from x86 specifications)
    gdt[0] = gdt_format_entry(0, 0, 0, 0);

    // Kernel code segment
    gdt[1] = gdt_format_entry(0, 0xfffff, 0x9a, 0xc);

    // Kernel data segment
    gdt[2] = gdt_format_entry(0, 0xfffff, 0x92, 0xc);

    gdt_flush((uint32_t) (&gdt_ref));
}
