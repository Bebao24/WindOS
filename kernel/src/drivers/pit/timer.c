#include "timer.h"
#include <irq.h>
#include <system.h>
#include <util.h>

uint64_t ticks;
uint32_t frequncy = 1193;

void InitializeTimer()
{
    IRQ_RegisterHandler(0, timerTick);

    uint32_t divisor = 1193180 / frequncy;

    x64_outb(0x43, 0x36);
    x64_outb(0x40, (uint8_t)(divisor & 0xFF));
    x64_outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

void sleep(uint64_t duration)
{
    uint64_t startTime = ticks;
    while (ticks < startTime + duration)
    {
        asm volatile("hlt");
    }
}

void timerTick(cpu_registers_t* cpu)
{
    USED(cpu);
    ticks++;
}
