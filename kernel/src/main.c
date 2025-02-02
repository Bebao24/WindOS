#include <limine.h>
#include <boot.h>
#include <stdbool.h>
#include <system.h>
#include <logging.h>
#include <framebuffer.h>
#include <console.h>
#include <memory.h>
#include <pmm.h>
#include <util.h>
#include <paging.h>

// Set base revision to 3
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

void kmain()
{
    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        panic();
    }

    ParseBootInfo();

    // Limine already identity mapping the framebuffer
    InitializeFramebuffer();
    InitializeConsole();

    clearScreen();

    InitializePMM();
    InitializePaging();

    paging_MapMemory((void*)0x60000000000000000, (void*)0x80000, PF_RW);
    uint64_t* test = (uint64_t*)0x60000000000000000;
    *test = 69;
    printf("%d\n", *test);

    void* physicalAddr = paging_VirtualToPhysical((void*)0x60000000000000000);
    printf("Physical Address: %llx\n", (uint64_t)physicalAddr);

    printf("Hello World!\n");

    while (true)
    {
        asm volatile("hlt");
    }
}

