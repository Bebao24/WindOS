#include <limine.h>
#include <boot.h>
#include <stdbool.h>
#include <system.h>
#include <logging.h>

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

    debugf("Hello World!\nTest: 0x%x\n", 0x123);

    while (true)
    {
        asm volatile("hlt");
    }
}

