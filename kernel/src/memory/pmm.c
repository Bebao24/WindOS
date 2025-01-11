#include "pmm.h"
#include <bitmap.h>
#include <logging.h>
#include <boot.h>
#include <system.h>
#include <util.h>
#include <memory.h>

uint64_t freeMemory;
uint64_t usedMemory;
uint64_t reservedMemory;

Bitmap g_PhysicalBitmap;

void pmm_InitializeBitmap(size_t bitmapSize, void* bitmapAddress);

void InitializePMM()
{
    void* largestFreeMemorySeg = NULL;
    size_t largestFreeMemorySegSize = 0;

    for (uint64_t i = 0; i < g_BootInfo.mmEntriesCount; i++)
    {
        struct limine_memmap_entry* entry = g_BootInfo.mmEntries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            if (entry->length > largestFreeMemorySegSize)
            {
                largestFreeMemorySeg = (void*)entry->base;
                largestFreeMemorySegSize = entry->length;
            }
        }
    }

    if (largestFreeMemorySeg == NULL)
    {
        debugf("Can't find free region for pmm bitmap!\n");
        panic();
    }

    debugf("pmm bitmap start: 0x%llx\n", (uint64_t)largestFreeMemorySeg);

    freeMemory = g_BootInfo.totalMemory;
    uint64_t bitmapSize = g_BootInfo.totalMemory / PAGE_SIZE / 8 + 1;
    pmm_InitializeBitmap(bitmapSize, largestFreeMemorySeg);

    pmm_ReservePages(0, g_BootInfo.totalMemory / PAGE_SIZE + 1);
    for (uint64_t i = 0; i < g_BootInfo.mmEntriesCount; i++)
    {
        struct limine_memmap_entry* entry = g_BootInfo.mmEntries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            // We should always shrink the free region
            pmm_UnreservePages((void*)entry->base, entry->length / PAGE_SIZE);
        }
    }

    pmm_ReservePages(0, 0x10);
    // The bitmap buffer in the g_PhysicalBitmap is virtual memory
    pmm_LockPages(largestFreeMemorySeg, g_PhysicalBitmap.bitmapSize / PAGE_SIZE + 1);
}

void pmm_InitializeBitmap(size_t bitmapSize, void* bitmapAddress)
{
    g_PhysicalBitmap.bitmapSize = bitmapSize;
    // The memory is physical memory
    g_PhysicalBitmap.bitmapBuffer = (uint8_t*)(bitmapAddress + g_BootInfo.hhdmOffset);
    memset(g_PhysicalBitmap.bitmapBuffer, 0, bitmapSize);
}

// 0 = free block
// 1 = occupied block

void pmm_FreePage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (Bitmap_Get(&g_PhysicalBitmap, index) == false)
    {
        return;
    }

    Bitmap_Set(&g_PhysicalBitmap, index, false);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void pmm_LockPage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (Bitmap_Get(&g_PhysicalBitmap, index))
    {
        return;
    }
    
    Bitmap_Set(&g_PhysicalBitmap, index, true);
    freeMemory -= 4096;
    usedMemory += 4096;
}

void pmm_UnreservePage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (Bitmap_Get(&g_PhysicalBitmap, index) == false)
    {
        return;
    }

    Bitmap_Set(&g_PhysicalBitmap, index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void pmm_ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (Bitmap_Get(&g_PhysicalBitmap, index))
    {
        return;
    }
    
    Bitmap_Set(&g_PhysicalBitmap, index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void pmm_FreePages(void* address, uint64_t pagesCount)
{
    for (uint64_t i = 0; i < pagesCount; i++)
    {
        pmm_FreePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void pmm_LockPages(void* address, uint64_t pagesCount)
{
    for (uint64_t i = 0; i < pagesCount; i++)
    {
        pmm_LockPage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void pmm_UnreservePages(void* address, uint64_t pagesCount)
{
    for (uint64_t i = 0; i < pagesCount; i++)
    {
        pmm_UnreservePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void pmm_ReservePages(void* address, uint64_t pagesCount)
{
    for (uint64_t i = 0; i < pagesCount; i++)
    {
        pmm_ReservePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}


uint64_t pmm_GetFreeMemory()
{
    return freeMemory;
}

uint64_t pmm_GetUsedMemory()
{
    return usedMemory;
}

uint64_t pmm_GetReservedMemory()
{
    return reservedMemory;
}

