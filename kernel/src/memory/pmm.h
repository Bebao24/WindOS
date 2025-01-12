#pragma once
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define INVALID_PAGE ((uint64_t)-1)

void InitializePMM();

void pmm_FreePage(void* address);
void pmm_LockPage(void* address);
void pmm_UnreservePage(void* address);
void pmm_ReservePage(void* address);

void pmm_FreePages(void* address, uint64_t pagesCount);
void pmm_LockPages(void* address, uint64_t pagesCount);
void pmm_UnreservePages(void* address, uint64_t pagesCount);
void pmm_ReservePages(void* address, uint64_t pagesCount);

uint64_t pmm_GetFreeMemory();
uint64_t pmm_GetUsedMemory();
uint64_t pmm_GetReservedMemory();

void* pmm_AllocatePage();
void* pmm_AllocatePages(uint64_t numPages);

