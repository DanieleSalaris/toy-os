#ifndef GDT_H
#define GDT_H
#include <stdint.h>

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity; // flags (4) + limit_high(4)
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t limit; // size of GDT -1;
    uint32_t base; // ptr to the first entry of the table
} __attribute__((packed)) gdt_t;

void gdt_init(void);
#endif
