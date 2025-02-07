#pragma once
#include <stdint.h>

void PIC_Remap(uint8_t offsetPIC1, uint8_t offsetPIC2);
void PIC_SendEOI(int irq);
void PIC_MaskIRQ(int irq);
void PIC_UnmaskIRQ(int irq);

