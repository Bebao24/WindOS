#include "logging.h"
#include <system.h>
#include <printf.h>

void debugc(char c)
{
    x64_outb(QEMU_DEBUGCON_PORT, c);
}

void debugs(const char* string)
{
    while (*string)
    {
        debugc(*string);
        string++;
    }
}

void debugf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf_internal(debugc, fmt, args);
    va_end(args);
}


