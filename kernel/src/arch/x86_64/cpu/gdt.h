#pragma once
#include <stdint.h>

#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10

typedef struct 
{
    uint16_t Limit;
    uint16_t BaseLow;
    uint8_t BaseMid;
    uint8_t Access;
    uint8_t Granularity;
    uint8_t BaseHigh;
} __attribute__((packed)) GDTEntry;

typedef struct
{
    uint16_t Limit; // Basically the size
    uint64_t Base;
} __attribute__((packed)) GDTR;

typedef struct
{
    GDTEntry descriptors[3];
} __attribute__((packed)) GDTEntries;


void InitializeGDT();




