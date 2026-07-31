section .text
global idt_flush
global asm_isr_division_by_zero
global asm_isr_keyboard_input
extern idt_division_by_zero_handler
extern idt_keyboard_input_handler

idt_flush:
    ; init base pointer
    push ebp
    mov ebp, esp
    ; loading idt
    mov eax, [ebp+8]
    lidt [eax]
    ; restoring base pointer
    pop ebp
    ret
asm_isr_division_by_zero:
    pushad
    cld
    call idt_division_by_zero_handler 
    popad
    iret

asm_isr_keyboard_input:
    pushad
    in al, 0x60         ; Read keyboard input
    movzx eax, al
    push eax            ; Push input to stack

    cld
    call idt_keyboard_input_handler ; Call C handler
    pop eax

    mov al, 0x20        ;Send End Of Interrupt (EOI)
    out 0x20, al

    popad
    iret

