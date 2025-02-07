#include "isr.h"
#include <idt.h>
#include <console.h>
#include <stddef.h>
#include <system.h>

ISRHandler g_ISRHandlers[256];

static const char* const g_Exceptions[] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void InitializeISR()
{
    for (int i = 0; i < 32; i++)
    {
        IDT_SetGate(i, (uint64_t)isr_stub_table[i], 0x8E);
    }
}

void interrupt_handler(cpu_registers_t* cpu)
{
    if (g_ISRHandlers[cpu->interrupt_number] != NULL)
    {
        g_ISRHandlers[cpu->interrupt_number](cpu);
    }
    else if (cpu->interrupt_number >= 32)
    {
        printf("Unhandled interrupt: %d\n", cpu->interrupt_number);
    }
    else
    {
        // It is an exception by the CPU
        printf("Exception: %s\n", g_Exceptions[cpu->interrupt_number]);
        printf("KERNEL PANIC!\n");
        panic();
    }
}

void ISR_RegisterHandler(int interrupt, ISRHandler handler)
{
    g_ISRHandlers[interrupt] = handler;
}


