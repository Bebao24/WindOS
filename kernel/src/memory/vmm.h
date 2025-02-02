#pragma once
#include <stdint.h>

void* vmm_AllocatePage();
void* vmm_AllocatePages(uint64_t numPages);

void vmm_FreePage(void* ptr);
void vmm_FreePages(void* ptr, uint64_t numPages);

