#pragma once
#include <cpu.h>

typedef void (*ISRHandler)(cpu_registers_t* registers);

extern void* isr_stub_table[];

void InitializeISR();
void ISR_RegisterHandler(int interrupt, ISRHandler handler);
