#include "gdt.h"
#include <stdint.h>

uint64_t gdt_entries[5];

static GDTEntries gdt;
static GDTR gdtr;

// From gdt.asm
extern void LoadGDT(GDTR* gdtr);

void InitializeGDT()
{
	// NULL descriptor
	gdt.descriptors[0].Limit = 0;
	gdt.descriptors[0].BaseLow = 0;
	gdt.descriptors[0].BaseMid = 0;
	gdt.descriptors[0].Access = 0;
	gdt.descriptors[0].Granularity = 0;
	gdt.descriptors[0].BaseHigh = 0;

	// Kernel code
	gdt.descriptors[1].Limit = 0;
	gdt.descriptors[1].BaseLow = 0;
	gdt.descriptors[1].BaseMid = 0;
	gdt.descriptors[1].Access = 0b10011010;
	gdt.descriptors[1].Granularity = 0b00100000;
	gdt.descriptors[1].BaseHigh = 0;

	// Kernel data
	gdt.descriptors[2].Limit = 0;
	gdt.descriptors[2].BaseLow = 0;
	gdt.descriptors[2].BaseMid = 0;
	gdt.descriptors[2].Access = 0b10010010;
	gdt.descriptors[2].Granularity = 0;
	gdt.descriptors[2].BaseHigh = 0;

	gdtr.Limit = sizeof(GDTEntries) - 1;
	gdtr.Base = (uint64_t)&gdt;

	LoadGDT(&gdtr);

}

