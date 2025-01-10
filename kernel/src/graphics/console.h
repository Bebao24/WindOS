#pragma once
#include <framebuffer.h>

#define CHARACTER_HEIGHT 16
#define CHARACTER_WIDTH 8

#define BACKGROUND_COLOR COLOR(0, 0, 255)
#define FOREGROUND_COLOR COLOR(255, 255, 255) 

void InitializeConsole();
void clearScreen();

void putc(char c);
void puts(const char* string);
void printf(const char* fmt, ...);
