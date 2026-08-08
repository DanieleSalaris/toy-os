#ifndef ASM_H
#define ASM_H

#include <stdint.h>
extern void asm_out(uint16_t port, uint8_t value);
extern void asm_in(uint16_t port);

#endif
