#include <limine.h>
#include <boot.h>
#include <stdbool.h>
#include <system.h>
#include <logging.h>
#include <framebuffer.h>
#include <console.h>
#include <bitmap.h>
#include <memory.h>

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

    // Test bitmap
    uint8_t test[20];
    memset(test, 0, sizeof(test));

    Bitmap testBitmap;
    testBitmap.bitmapBuffer = &test[0];
    Bitmap_Set(&testBitmap, 1, true);
    Bitmap_Set(&testBitmap, 4, true);
    Bitmap_Set(&testBitmap, 5, true);
    Bitmap_Set(&testBitmap, 6, true);
    Bitmap_Set(&testBitmap, 11, true);
    Bitmap_Set(&testBitmap, 12, true);

    for (int i = 0; i < 15; i++)
    {
        printf(Bitmap_Get(&testBitmap, i) ? "true" : "false");
        putc('\n');
    }

    while (true)
    {
        asm volatile("hlt");
    }
}

