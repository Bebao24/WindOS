#include "gdt.h"
#include <stdint.h>

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTR;

uint64_t gdt_entries[5];

GDTR default_gdtr;

// From gdt.asm
extern void LoadGDT(GDTR* gdtr);

void InitializeGDT()
{
    // NULL descriptor
    gdt_entries[0] = 0;

    // Kernel code
    uint64_t kernel_code = 0;
    kernel_code |= 0b1011 << 8; // Type of selector
    kernel_code |= 1 << 12; // Not a system descriptor
    kernel_code |= 0 << 13; // Ring 0
    kernel_code |= 1 << 15; // Present
    kernel_code |= 1 << 21; // Long mode segment
    gdt_entries[1] = kernel_code << 32;

    // Kernel data
    uint64_t kernel_data = 0;
    kernel_data |= 0b0011 << 8; // Type of selector
    kernel_data |= 1 << 12; // Not a system descriptor
    kernel_data |= 0 << 13; // Ring 0
    kernel_data |= 1 << 15; // Present
    kernel_data |= 1 << 21; // Long mode segment
    gdt_entries[2] = kernel_data << 32;

    // User code
    uint64_t user_code = kernel_code | (3 << 13); // Ring 3
    gdt_entries[3] = user_code;

    // User data
    uint64_t user_data = kernel_data | (3 << 13); // Ring 3
    gdt_entries[4] = user_data;

    // Setup the GDTR
    default_gdtr.limit = sizeof(gdt_entries) - 1;
    default_gdtr.base = (uint64_t)gdt_entries;

    // Load and flush the GDT
    LoadGDT(&default_gdtr);
}

