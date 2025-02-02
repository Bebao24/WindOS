#include "paging.h"
#include <stdint.h>
#include <stddef.h>
#include <boot.h>
#include <logging.h>
#include <system.h>
#include <pmm.h>
#include <memory.h>

uint64_t* g_PageDir = NULL;

void InitializePaging()
{
    // I will just use the limine's page directory
    uint64_t pdPhysicalAddr = 0;
    asm volatile("movq %%cr3,%0" : "=r"(pdPhysicalAddr)); // Get the physical address from the CR3 register

    // Check if we got the page directory's physical address
    if (!pdPhysicalAddr)
    {
        debugf("Failed to get page directory's physical address!\n");
        panic();
    }

    uint64_t pdVirtualAddr = pdPhysicalAddr + g_BootInfo.hhdmOffset;
    g_PageDir = (uint64_t*)pdVirtualAddr;
}

uint64_t paging_PhysicalAllocate()
{
    uint64_t physicalAddr = pmm_AllocatePage();

    void* virtualAddr = (void*)(physicalAddr + g_BootInfo.hhdmOffset);
    memset(virtualAddr, 0, PAGE_SIZE);

    return physicalAddr;
}

void invalidate(uint64_t virtualAddr)
{
    asm volatile("invlpg %0" ::"m"(virtualAddr));
}

void paging_MapMemory(void* virtualAddr, void* physicalAddr, uint64_t flags)
{
    // For convenient, we will cast the addresses into a uint64_t
    uint64_t virtualAddress = (uint64_t)virtualAddr;
    uint64_t physicalAddress = (uint64_t)physicalAddr;

    // Turn the virtual address into a 48 bits address (by stripping out the upper 16 bits)
    virtualAddress = AMD64_MM_STRIPSX(virtualAddress);

    uint64_t pml4_index = PML4E(virtualAddress);
    uint64_t pdp_index = PDPTE(virtualAddress);
    uint64_t pd_index = PDE(virtualAddress);
    uint64_t pt_index = PTE(virtualAddress);

    if (!(g_PageDir[pml4_index] & PF_PRESENT))
    {
        size_t targetAddr = paging_PhysicalAllocate();
        g_PageDir[pml4_index] = targetAddr | PF_PRESENT | PF_RW | PF_USER;
    }

    uint64_t* pdp = (uint64_t*)(PTE_GET_ADDR(g_PageDir[pml4_index]) + g_BootInfo.hhdmOffset);

    if (!(pdp[pdp_index] & PF_PRESENT))
    {
        size_t targetAddr = paging_PhysicalAllocate();
        pdp[pdp_index] = targetAddr | PF_PRESENT | PF_RW | PF_USER;
    }

    uint64_t* pd = (uint64_t*)(PTE_GET_ADDR(pdp[pdp_index]) + g_BootInfo.hhdmOffset);

    if (!(pd[pd_index] & PF_PRESENT))
    {
        size_t targetAddr = paging_PhysicalAllocate();
        pd[pd_index] = targetAddr | PF_PRESENT | PF_RW | PF_USER;
    }

    uint64_t* pt = (uint64_t*)(PTE_GET_ADDR(pd[pd_index]) + g_BootInfo.hhdmOffset);
    
    pt[pt_index] = (P_PHYS_ADDR(physicalAddress)) | PF_PRESENT | flags;
    invalidate(virtualAddress);
}

void* paging_VirtualToPhysical(void* virtualAddr)
{
    uint64_t virtualAddress = (uint64_t)virtualAddr;

    if (virtualAddress >= g_BootInfo.hhdmOffset && virtualAddress <= (g_BootInfo.hhdmOffset + g_BootInfo.totalMemory))
    {
        return (void*)(virtualAddress - g_BootInfo.hhdmOffset);
    }

    // Save the initial virtual address
    uint64_t virtualAddr_init = virtualAddress;

    virtualAddress &= ~(0xFFF);
    virtualAddress = AMD64_MM_STRIPSX(virtualAddress);

    uint64_t pml4_index = PML4E(virtualAddress);
    uint64_t pdp_index = PDPTE(virtualAddress);
    uint64_t pd_index = PDE(virtualAddress);
    uint64_t pt_index = PTE(virtualAddress);

    if (!(g_PageDir[pml4_index] & PF_PRESENT))
    {
        goto error;
    }

    uint64_t* pdp = (uint64_t*)(PTE_GET_ADDR(g_PageDir[pml4_index]) + g_BootInfo.hhdmOffset);

    if (!(pdp[pdp_index] & PF_PRESENT))
    {
        goto error;
    }

    uint64_t* pd = (uint64_t*)(PTE_GET_ADDR(pdp[pdp_index]) + g_BootInfo.hhdmOffset);

    if (!(pd[pd_index]) & PF_PRESENT)
    {
        goto error;
    }

    uint64_t* pt = (uint64_t*)(PTE_GET_ADDR(pd[pd_index]) + g_BootInfo.hhdmOffset);

    if (pt[pt_index] & PF_PRESENT)
    {
        return (void*)(PTE_GET_ADDR(pt[pt_index]) + ((uint64_t)virtualAddr_init & 0xFFF));
    }

    // Otherwise, it will go to the error label

error:
    return 0;


}


