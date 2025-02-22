#include "gdt.h"
#include <stdint.h>
#include <memory.h>

uint64_t gdt_entries[5];

static GDTEntries gdt;
static GDTR gdtr;

static TSSPtr tss;

TSSPtr* tssPtr = &tss;

// From gdt.asm
extern void LoadGDT(GDTR* gdtr);

void GDT_LoadTSS(TSSPtr* tss)
{
	uint64_t TSSAddr = (uint64_t)tss;

	gdt.tss.BaseLow = (uint16_t)TSSAddr;
	gdt.tss.BaseMid = (uint8_t)(TSSAddr >> 16);
	gdt.tss.Flags1 = 0b10001001;
	gdt.tss.Flags2 = 0;
	gdt.tss.BaseHigh = (uint8_t)(TSSAddr >> 24);
	gdt.tss.BaseUpper = (uint32_t)(TSSAddr >> 32);
	gdt.tss.Reserved = 0;

	asm volatile("ltr %0" : : "rm"((uint16_t)GDT_TSS) : "memory");
}

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

	// GDT TSS
	gdt.tss.Limit = 104;
	gdt.tss.BaseLow = 0;
	gdt.tss.BaseMid = 0;
	gdt.tss.Flags1 = 0b10001001;
	gdt.tss.Flags2 = 0;
	gdt.tss.BaseHigh = 0;
	gdt.tss.BaseUpper = 0;
	gdt.tss.Reserved = 0;

	gdtr.Limit = sizeof(GDTEntries) - 1;
	gdtr.Base = (uint64_t)&gdt;

	LoadGDT(&gdtr);

	// Prepare for the TSS
	memset(&tss, 0, sizeof(TSSPtr));
	GDT_LoadTSS(&tss);
}

