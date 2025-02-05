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
#include <vmm.h>
#include <heap.h>

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
    InitializeHeap(0x10);

    printf("malloc result: %llx\n", (uint64_t)malloc(0x100));
    printf("malloc result: %llx\n", (uint64_t)malloc(0x8000));

    void* test = vmm_AllocatePage();

    printf("malloc result: %llx\n", (uint64_t)malloc(0x8000));


    printf("Hello World!\n");

    while (true)
    {
        asm volatile("hlt");
    }
}

