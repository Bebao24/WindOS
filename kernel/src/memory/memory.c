#include "memory.h"
#include <stdint.h>

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void* memcpy(void* dst, const void* src, size_t num)
{
    uint8_t* u8Dst = (uint8_t*)dst;
    const uint8_t* u8Src = (const uint8_t*)src;

    for (size_t i = 0; i < num; i++)
    {
        u8Dst[i] = u8Src[i];
    }

    return dst;
}

void* memset(void* ptr, int value, size_t num)
{
    uint8_t* u8Ptr = (uint8_t*)ptr;

    for (size_t i = 0; i < num; i++)
    {
        u8Ptr[i] = (uint8_t)value;
    }

    return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
    const uint8_t* u8Ptr1 = (const uint8_t*)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t*)ptr2;

    for (size_t i = 0; i < num; i++)
    {
        if (u8Ptr1[i] != u8Ptr2[i])
        {
            return 1;
        }
    }

    return 0;
}

void *memmove(void* dst, const void* src, size_t num)
{
    uint8_t *u8Dst = (uint8_t *)dst;
    const uint8_t *u8Src = (const uint8_t *)src;

    if (u8Dst < u8Src)
    {
        for (size_t i = 0; i < num; i++)
        {
            u8Dst[i] = u8Src[i];
        }
    }
    else
    {
        for (size_t i = num; i > 0; i--)
        {
            u8Dst[i - 1] = u8Src[i - 1];
        }
    }

    return dst;
}
