#pragma once
#include <stdint.h>

extern uint32_t* fb;
extern uint64_t fbWidth;
extern uint64_t fbHeight;
extern uint64_t fbPitch;

#define COLOR(r, g, b) ((r << 16) | (g << 8) | (b))

void InitializeFramebuffer();

void fb_PutPixel(uint32_t x, uint32_t y, uint32_t color);
void fb_clearScreen(uint32_t color);
void fb_drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

