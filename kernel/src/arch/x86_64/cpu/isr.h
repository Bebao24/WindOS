#pragma once
#include <cpu.h>

#define PIC_REMAP_OFFSET 0x20

typedef void (*ISRHandler)(cpu_registers_t* registers);

extern void* isr_stub_table[];

void InitializeISR();
void ISR_RegisterHandler(int interrupt, ISRHandler handler);
