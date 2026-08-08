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
global pic_remap
global asm_out
global asm_in
global gdt_flush:function (gdt_flush_end - gdt_flush)
extern kernel_main                        ; Declare external C function

pic_remap:
    ; --- Master PIC (0x20 / 0x21) ---
    mov al, 0x11
    out 0x20, al        ; ICW1 al master
    out 0x80, al         ; delay

    mov al, 0x20
    out 0x21, al        ; ICW2 master: offset vettore 0x20 (IRQ0->0x20 ... IRQ7->0x27)
    out 0x80, al

    mov al, 0x04
    out 0x21, al        ; ICW3 master: bitmask, bit2=1 -> slave agganciato su IRQ2
    out 0x80, al

    mov al, 0x01
    out 0x21, al        ; ICW4 master: modalità 8086
    out 0x80, al

    ; --- Slave PIC (0xA0 / 0xA1) ---
    mov al, 0x11
    out 0xA0, al        ; ICW1 allo slave
    out 0x80, al

    mov al, 0x28
    out 0xA1, al        ; ICW2 slave: offset vettore 0x28 (IRQ8->0x28 ... IRQ15->0x2F)
    out 0x80, al

    mov al, 0x02
    out 0xA1, al        ; ICW3 slave: numero semplice, 2 -> agganciato a IRQ2 del master
    out 0x80, al

    mov al, 0x01
    out 0xA1, al        ; ICW4 slave: modalità 8086
    out 0x80, al

    mov al, 0xFD
    out 0x21, al        ; Masking IRQ lines except keyboard for master
    out 0x80, al

    mov al, 0xFF
    out 0xA1, al        ; Masking All IRQ lines for slave
    out 0x80, al
    ret
asm_out:
    mov eax, [esp+8]    ; value
    mov edx, [esp+4]    ; port
    out dx, al
    ret
asm_in:
    mov edx, [esp+4]    ;port
    in al, dx
    movzx eax, al
    ret
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
.flush_far_jump:
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
