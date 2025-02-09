#include "irq.h"
#include <isr.h>
#include <logging.h>
#include <stddef.h>
#include <pic.h>

IRQhandler g_IRQHandlers[16];

void IRQ_Handler(cpu_registers_t* cpu)
{
    int irq = cpu->interrupt_number - PIC_REMAP_OFFSET;

    if (g_IRQHandlers[irq] != NULL)
    {
        g_IRQHandlers[irq](cpu);
    }
    else
    {
        debugf("Unhandled IRQ: %d\n", irq);
    }

    // Send EOI
    PIC_SendEOI(irq);
}

void InitializeIRQ()
{
    // We've already remapped the PIC

    for (int i = 0; i < 16; i++)
    {
        ISR_RegisterHandler(PIC_REMAP_OFFSET + i, IRQ_Handler);    
    }
}

void IRQ_RegisterHandler(int irq, IRQhandler handler)
{
    g_IRQHandlers[irq] = handler;
}
