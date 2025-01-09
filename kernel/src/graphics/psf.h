#pragma once
#include <stdint.h>
#include <stdbool.h>

#define PSF1_MAGIC_0 0x36
#define PSF1_MAGIC_1 0x04

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charSize;
} PSF1_FONT;

extern PSF1_FONT* psf;

bool psf_LoadFont();
uint8_t* psf_GetGlyphBuffer(uint8_t symbolNumber);
