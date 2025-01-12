#pragma once
#include <stdint.h>

void panic();

void x64_outb(uint16_t port, uint8_t value);
uint8_t x64_inb(uint16_t port);

// From linker.ld
extern uint64_t kernel_start;
extern uint64_t kernel_end;

