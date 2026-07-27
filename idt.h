#ifndef IDT_H
#define IDT_H
#include <stdint.h>

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) idt_descriptor_t;

typedef struct {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__ ((packed)) idt_entry_t;

void idt_init();

#endif
