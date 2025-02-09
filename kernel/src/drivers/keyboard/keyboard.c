#include "keyboard.h"
#include <kb_translation.h>
#include <util.h>
#include <heap.h>
#include <memory.h>
#include <irq.h>
#include <system.h>

key_info_t* g_keyInfo;

void InitializeKeyboard()
{
    g_keyInfo = malloc(sizeof(key_info_t));
    memset(g_keyInfo, 0, sizeof(key_info_t));

    IRQ_RegisterHandler(1, KeyboardIRQ);
}

void KeyboardIRQ(cpu_registers_t* cpu)
{
    USED(cpu);

    uint8_t scancode = x64_inb(0x60);

    switch (scancode)
    {
        case LeftShift:
            g_keyInfo->uppercase = true;
            return;
        case LeftShift + 0x80: // Key released
            g_keyInfo->uppercase = false;
            return;
        case RightShift:
            g_keyInfo->uppercase = true;
            return;
        case RightShift + 0x80:
            g_keyInfo->uppercase = false;;
            return;
        case Enter:
            g_keyInfo->key = '\r';
            return;
        case BackSpace:
            g_keyInfo->key = '\b';
            return;
    }

    char key = TranslateScancode(scancode, g_keyInfo->uppercase);

    if (key)
    {
        g_keyInfo->key = key;
    }
}

char kb_getKey()
{
    while (!g_keyInfo->key)
    {
        asm volatile("hlt");
    }

    char key = g_keyInfo->key;
    g_keyInfo->key = 0;

    return key;
}

