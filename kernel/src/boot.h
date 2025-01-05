#pragma once
#include <stddef.h>
#include <limine.h>

typedef struct
{
    uint64_t hhdmOffset;
    uint64_t kernelVirtBase;
    uint64_t kernelPhysicalBase;

    uint64_t totalMemory;
    uint64_t mmEntriesCount;
    LIMINE_PTR(struct limine_memmap_entry**)mmEntries;
} BootInfo;

extern BootInfo g_BootInfo;

void ParseBootInfo();

