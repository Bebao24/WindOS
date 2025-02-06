#include "idt.h"
#include <stdint.h>

typedef struct
{
    uint16_t baseLow;
    uint16_t segmentSelector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t baseMid;
    uint32_t baseHigh;
    uint32_t reserved;
} __attribute__((packed)) IDTEntry;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IDTR;

IDTEntry idtEntries[256] = { 0 };
IDTR default_idtr;

void IDT_SetGate(int interrupt, uint64_t handler, uint8_t flags)
{
    idtEntries[interrupt].baseLow = (uint16_t)handler;
    idtEntries[interrupt].segmentSelector = 0x08; // Kernel code
    idtEntries[interrupt].ist = 0;
    idtEntries[interrupt].reserved = 0;
    idtEntries[interrupt].attributes = flags;
    idtEntries[interrupt].baseMid = (uint16_t)(handler >> 16);
    idtEntries[interrupt].baseHigh = (uint32_t)(handler >> 32);
}

void InitializeIDT()
{
    default_idtr.limit = sizeof(idtEntries) - 1;
    default_idtr.base = (uint64_t)&idtEntries;
    asm volatile ("lidt %0" : : "m"(default_idtr) : "memory"); // Load the IDT
}


