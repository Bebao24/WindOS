#include <limine.h>
#include <boot.h>
#include <stdbool.h>
#include <system.h>
#include <logging.h>
#include <framebuffer.h>
#include <console.h>
#include <bitmap.h>
#include <memory.h>
#include <pmm.h>
#include <util.h>

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

    printf("Free memory: %d KB\n", DivRoundUp(pmm_GetFreeMemory(), 1024));
    printf("Used memory: %d KB\n", DivRoundUp(pmm_GetUsedMemory(), 1024));
    printf("Reserved memory: %d KB\n", DivRoundUp(pmm_GetReservedMemory(), 1024));

    printf("Hello World!\n");

    while (true)
    {
        asm volatile("hlt");
    }
}

