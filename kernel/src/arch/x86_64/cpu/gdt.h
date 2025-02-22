#pragma once
#include <stdint.h>

#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_TSS 0x18

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
    uint16_t Limit;
    uint16_t BaseLow;
    uint8_t BaseMid;
    uint8_t Flags1;
    uint8_t Flags2;
    uint8_t BaseHigh;
    uint32_t BaseUpper;
    uint32_t Reserved;
} __attribute__((packed)) TSSEntry;

typedef struct tss
{
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t reserved2;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t io_bitmap_offset;
}__attribute__((__packed__)) TSSPtr;

typedef struct
{
    GDTEntry descriptors[3];
    TSSEntry tss;
} __attribute__((packed)) GDTEntries;


void InitializeGDT();




