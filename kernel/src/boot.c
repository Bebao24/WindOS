#include "boot.h"
#include <system.h>

BootInfo g_BootInfo;

__attribute__((used, section(".limine_requests_start")))
static volatile struct limine_paging_mode_request liminePagingReq = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL
};

__attribute__((used, section(".limine_requests_start")))
static volatile struct limine_kernel_address_request limineKernelAddrReq = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile struct limine_hhdm_request limineHHDMReq = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile struct limine_memmap_request limineMMReq = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void ParseBootInfo()
{
    struct limine_paging_mode_response* liminePagingRes = liminePagingReq.response;
    if (liminePagingRes->mode != LIMINE_PAGING_MODE_X86_64_4LVL)
    {
        panic();
    }

    // Higher half direct map
    struct limine_hhdm_response* limineHHDMRes = limineHHDMReq.response;
    g_BootInfo.hhdmOffset = limineHHDMRes->offset;

    // Kernel address
    struct limine_kernel_address_response* limineKernelAddrRes = limineKernelAddrReq.response;
    g_BootInfo.kernelPhysicalBase = limineKernelAddrRes->physical_base;
    g_BootInfo.kernelVirtBase = limineKernelAddrRes->virtual_base;

    // Memory map
    struct limine_memmap_response* limineMMRes = limineMMReq.response;
    g_BootInfo.mmEntriesCount = limineMMRes->entry_count;
    g_BootInfo.mmEntries = limineMMRes->entries;

    g_BootInfo.totalMemory = 0;
    for (uint64_t i = 0; i < limineMMRes->entry_count; i++)
    {
        struct limine_memmap_entry* entry = limineMMRes->entries[i];
        
        if (entry->type != LIMINE_MEMMAP_RESERVED)
        {
            g_BootInfo.totalMemory += entry->length;
        }
    }
}


