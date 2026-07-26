; boot.asm - Minimal Multiboot Header for Toy OS
MBALIGN     equ  1 << 0                   ; align loaded modules on page boundaries
MEMINFO     equ  1 << 1                   ; provide memory map
MAGIC       equ  0x1BADB002               ; 'magic number' lets bootloader find the header
FLAGS       equ  MBALIGN | MEMINFO        ; this is the Multiboot 'flag' field
CHECKSUM    equ -(MAGIC + FLAGS)          ; checksum of above, to prove we are multiboot

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB stack
stack_top:

section .text
global _start:function (_start.end - _start)
global gdt_flush:function (gdt_flush_end - gdt_flush)
extern kernel_main                        ; Declare external C function

gdt_flush:
    ; init base pointer
    push ebp
    mov ebp, esp
    ; loading gdt
    mov eax, [ebp+8]
    lgdt [eax]
    ; kernel data section is the third entry of the table (relative addr 0x10)
    ; void (0), kernel code (1), kernel data(2)
    ; 0x0       0x08             0x10
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; initing cs to the code section with a far jump
    jmp 0x08:.flush_far_jump
.flush_far_jump
    ; restoring base pointer
    pop ebp
    ret
gdt_flush_end:
_start:
    ; Set up stack pointer
    mov esp, stack_top

    ; Call C kernel main
    call kernel_main

    ; Hang if kernel_main ever returns
    cli
.hang:
    hlt
    jmp .hang
.end:
