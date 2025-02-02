#include <vmm.h>
#include <pmm.h>
#include <boot.h>
#include <paging.h>
#include <system.h>
#include <logging.h>

void* vmm_AllocatePage()
{
    return vmm_AllocatePages(1);
}

void* vmm_AllocatePages(uint64_t numPages)
{
    uint64_t physicalAddr = (uint64_t)pmm_AllocatePages(numPages);

    uint64_t virtualAddress = physicalAddr + g_BootInfo.hhdmOffset;
    return (void*)virtualAddress;
}

void vmm_FreePage(void* ptr)
{
    vmm_FreePages(ptr, 1);
}

void vmm_FreePages(void* ptr, uint64_t numPages)
{
    uint64_t physicalAddr = paging_VirtualToPhysical(ptr);
    if (!physicalAddr)
    {
        debugf("Can't find the physical address of %llx!\n", (uint64_t)ptr);
        panic();
    }

    pmm_FreePages(physicalAddr, numPages);
}




