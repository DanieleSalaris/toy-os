section .text
global idt_flush
global asm_isr_division_by_zero
extern idt_division_by_zero_handler

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
