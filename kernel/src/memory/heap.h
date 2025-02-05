#pragma once
#include <stddef.h>
#include <stdbool.h>

#define HEAP_ALIGNMENT 0x10

#define COMBINE_RIGHT 0b01
#define COMBINE_LEFT  0b10
#define COMBINE_BOTH  0b11
#define COMBINE_NONE  0b00

// The heap node will just be a linked list
typedef struct HeapNode
{
    size_t size;
    bool free;
    struct HeapNode* next;
    struct HeapNode* last;
} HeapNode;

void InitializeHeap(size_t pagesCount);

void* malloc(size_t size);
void free(void* ptr);


