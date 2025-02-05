#include "heap.h"
#include <stdint.h>
#include <vmm.h>
#include <pmm.h>
#include <logging.h>
#include <util.h>

void* heapStart;
HeapNode* lastNode;
void* heapEnd;

void InitializeHeap(size_t pagesCount)
{
    void* heapAddress = vmm_AllocatePages(pagesCount);
    debugf("Heap starting address: 0x%llx\n", (uint64_t)heapAddress);

    size_t heapSize = pagesCount * PAGE_SIZE;

    heapStart = heapAddress;
    heapEnd = (void*)((size_t)heapStart + heapSize);

    lastNode = (HeapNode*)heapStart;
    lastNode->size = heapSize - sizeof(HeapNode);
    lastNode->next = NULL;
    lastNode->last = NULL;
    lastNode->free = true;
}

size_t align(size_t size)
{
    // return (size / HEAP_ALIGNMENT + 1) * HEAP_ALIGNMENT;
    size_t newSize = size;
    if (newSize % HEAP_ALIGNMENT > 0)
    {
        newSize -= (newSize % HEAP_ALIGNMENT);
        newSize += HEAP_ALIGNMENT;
    }

    return newSize;
}

size_t alignToPage(size_t size)
{
    size_t newSize = size;
    if (newSize % PAGE_SIZE > 0)
    {
        newSize -= (newSize % PAGE_SIZE);
        newSize += PAGE_SIZE;
    }

    return newSize;
}

void SplitNode(HeapNode* node, size_t splitSize)
{
    if (splitSize < HEAP_ALIGNMENT)
    {
        return;
    }

    HeapNode* newNode = (HeapNode*)((void*)node + splitSize + sizeof(HeapNode));

    newNode->size = node->size - splitSize - sizeof(HeapNode);
    newNode->free = true;
    newNode->next = node->next;

    node->size = splitSize;
    node->free = false;
    node->next = newNode;

    if (lastNode == node)
    {
        lastNode = newNode;
    }
}

void CombineForward(HeapNode* node)
{
    if (node->next == NULL)
    {
        return;
    }

    if (!node->next->free)
    {
        return;
    }

    if (node->next == lastNode)
    {
        lastNode = node;
    }

    if (node->next->next != NULL)
    {
        node->next->next->last = node;
    }

    node->size = node->size + node->next->size + sizeof(HeapNode);
    node->next = node->next->next;
}

void CombineBackward(HeapNode* node)
{
    if (node->last != NULL && node->last->free)
    {
        CombineForward(node->last);
    }
}

uint8_t CanCombine(HeapNode* node)
{
    uint8_t can_combine = 0;

    if (node->last != NULL && node->last->free)
    {
        uint64_t prevAddr = (uint64_t)node->last + sizeof(HeapNode) + node->last->size;
        if (prevAddr == (uint64_t)node)
        {
            can_combine |= COMBINE_LEFT;
        }
    }

    if (node->next != NULL && node->next->free)
    {
        uint64_t nextAddr = (uint64_t)node + sizeof(HeapNode) + node->size;
        if (nextAddr == (uint64_t)node->next)
        {
            can_combine |= COMBINE_RIGHT;
        }
    }

    return can_combine;
}

void ExpandHeap(size_t neededSize)
{
    neededSize = alignToPage(neededSize);
    debugf("Expanding heap with size 0x%llx\n", neededSize);

    size_t pagesCount = DivRoundUp(neededSize, PAGE_SIZE);

    // We will just allocate using the vmm
    HeapNode* newNode = (HeapNode*)vmm_AllocatePages(pagesCount);

    debugf("Expand heap address: %llx\n", (uint64_t)newNode);

    newNode->next = NULL;
    newNode->last = lastNode;
    newNode->size = pagesCount * PAGE_SIZE;
    newNode->free = true;

    lastNode->next = newNode;
    lastNode = newNode;

    // Try to combine backward if we can
    // uint8_t can_combine = CanCombine(newNode);
    // if (can_combine & COMBINE_LEFT)
    // {
    //     CombineBackward(newNode);
    // }
}

void* malloc(size_t size)
{
    HeapNode* currentNode = (HeapNode*)heapStart;

    if (size == 0)
    {
        debugf("Can't allocate heap with size 0!\n");
        return NULL;
    }

    // Align it to 0x10
    size = align(size);

    while (currentNode)
    {
        if (currentNode->free)
        {
            if (currentNode->size >= size)
            {
                // We found a node that is large enough
                if (currentNode->size > size)
                {
                    SplitNode(currentNode, size);
                    currentNode->free = false;
                    currentNode->size = size;
                }
                else
                {
                    currentNode->free = false;
                }
                
                return (void*)currentNode + sizeof(HeapNode);
            }
        }

        if (currentNode == lastNode)
        {
            ExpandHeap(size);
            if (currentNode->last != NULL)
            {
                currentNode = currentNode->last;
            }
        }

        currentNode = currentNode->next;
    }

    return NULL;
}


