#include "console.h"
#include <printf.h>

int g_ScreenX;
int g_ScreenY;

void InitializeConsole()
{
    g_ScreenX = 0;
    g_ScreenY = 0;
}

void clearScreen()
{
    g_ScreenX = 0;
    g_ScreenY = 0;
    fb_clearScreen(BACKGROUND_COLOR);
}

void putc(char c)
{
    switch (c)
    {
        case '\r':
            g_ScreenX = 0;
            break;
        case '\n':
            g_ScreenX = 0;
            g_ScreenY += CHARACTER_HEIGHT;
            break;
        case '\t':
            for (int i = 0; i < 4 - (g_ScreenX % 4); i++)
            {
                putc(' ');
            }
            break;
        default:
            fb_drawChar(g_ScreenX, g_ScreenY, FOREGROUND_COLOR, c);
            g_ScreenX += CHARACTER_WIDTH;
            break;
    }

    if (g_ScreenX >= fbWidth)
    {
        g_ScreenX = 0;
        g_ScreenY += CHARACTER_HEIGHT;
    }

    if (g_ScreenY >= fbHeight)
    {
        // We need to implement scroll back later
        clearScreen();
    }
}

void puts(const char* string)
{
    while (*string)
    {
        putc(*string);
        string++;
    }
}

void printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf_internal(putc, fmt, args);
    va_end(args);
}

