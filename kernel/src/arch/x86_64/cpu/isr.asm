[bits 64]
extern interrupt_handler


%macro ISR_ERROR_CODE 1
global isr%1
isr%1:
    ; The error code is already on the stack
    push %1 ; Push the interrupt number
    jmp isr_common
%endmacro

%macro ISR_NO_ERROR_CODE 1
global isr%1
isr%1:
    push 0 ; Push a dummy error code
    push %1 ; Push the interrupt number
    jmp isr_common
%endmacro

%include "src/arch/x86_64/cpu/isr.inc"

%macro save_context 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro restore_context 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

global isr_common
isr_common:
    save_context
    mov rdi, rsp ; Pass the registers to the C function
    call interrupt_handler
    restore_context
    add rsp, 16
    iretq

section .data
global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    48
    dq isr%+i
%assign i i+1 
%endrep

