#include "psf.h"
#include <zap-light.h>
#include <logging.h>

PSF1_FONT* psf;

bool psf_LoadFont()
{
    void* fontData = &zap_light_psf[0];
    PSF1_FONT* fontHeader = (PSF1_FONT*)fontData;

    if (fontHeader->magic[0] != PSF1_MAGIC_0 || fontHeader->magic[1] != PSF1_MAGIC_1)
    {
        debugf("Invalid PSF font!\n");
        return false;
    }

    psf = fontData;
    debugf("PSF font char size: %d\n", psf->charSize);
    return true;
}

uint8_t* psf_GetGlyphBuffer(uint8_t symbolNumber)
{
    return (uint8_t*)psf + sizeof(PSF1_FONT) + (symbolNumber * psf->charSize);
}


