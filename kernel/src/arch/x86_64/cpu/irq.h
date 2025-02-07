#pragma once
#include <cpu.h>

typedef void (*IRQhandler)(cpu_registers_t* registers);

void InitializeIRQ();
void IRQ_RegisterHandler(int irq, IRQhandler handler);

