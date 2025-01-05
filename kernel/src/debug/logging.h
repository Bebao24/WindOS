#pragma once

#define QEMU_DEBUGCON_PORT 0xE9

void debugc(char c);
void debugs(const char* string);
void debugf(const char* fmt, ...);

