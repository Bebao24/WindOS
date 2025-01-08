#include "framebuffer.h"
#include <limine.h>
#include <system.h>
#include <stddef.h>
#include <logging.h>

uint32_t* fb = NULL;
uint64_t fbWidth;
uint64_t fbHeight;
uint64_t fbPitch;

__attribute__((used, section(".limine_requests_start")))
static volatile struct limine_framebuffer_request limineFbReq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void InitializeFramebuffer()
{
    // Check if we got a framebuffer
    if (limineFbReq.response == NULL || limineFbReq.response->framebuffer_count < 1)
    {
        panic();
    }

    struct limine_framebuffer* limineFb = limineFbReq.response->framebuffers[0];
    fb = limineFb->address;
    fbWidth = limineFb->width;
    fbHeight = limineFb->height;
    fbPitch = limineFb->pitch;
    debugf("Framebuffer resolution: %dx%d, pitch: %d\n", fbWidth, fbHeight, fbPitch);
}

void fb_PutPixel(uint32_t x, uint32_t y, uint32_t color)
{
    uint32_t offset = (y * fbPitch) + (x * 4);
    fb[offset / 4] = color;
}

void fb_clearScreen(uint32_t color)
{
    for (uint32_t y = 0; y < fbHeight; y++)
    {
        for (uint32_t x = 0; x < fbWidth; x++)
        {
            uint32_t offset = (y * fbPitch) + (x * 4);
            fb[offset / 4] = color;
        }
    }
}

void fb_drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    for (uint32_t yy = 0; yy < h; yy++)
    {
        for (uint32_t xx = 0; xx < w; xx++)
        {
            uint32_t offset = ((y + yy) * fbPitch) + ((x + xx) * 4);
            fb[offset / 4] = color;
        }
    }
}


