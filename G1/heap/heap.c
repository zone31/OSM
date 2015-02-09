#ifndef HEAP_C
#define HEAP_C

#include <stdio.h> /* REMOVE. */
#include <stdlib.h>
#include <assert.h>
#include "heap.h"

void heap_initialize(heap *heap)
{
    size_t alloc_size = sizeof(node) * 100;
    node *root = malloc(sizeof(node) * 100); /* Make space for 100 things. */

    assert(root != NULL);

    heap->root = root;
    heap->alloc_size = alloc_size;
    heap->size = 0;
}

void heap_clear(heap *heap)
{
    free(heap->root);
}

size_t heap_size(heap *heap)
{
    return heap->size;
}

/* Return the top of the heap, a NULL pointer is returned if the heap is
 * empty. */
void * heap_top(heap *heap)
{
    return heap->size == 0 ? NULL : heap->root->thing;
}

void heap_insert(heap *heap, void *thing, int priority)
{
    node new_node = {thing, priority};
    int pos = heap->size;

    /* Test if the heap has space for one more element. */
    if (heap->alloc_size / sizeof(node) == heap->size) {
        heap_increase_size(heap);
    }

    heap->size += 1;
    heap->root[pos] = new_node;

    while (pos > 0 && heap->root[PARENT(pos)].priority <
        heap->root[pos].priority) {
        exchange(heap->root, pos, PARENT(pos));
        pos = PARENT(pos);
    }
}

/* Help functions. */
void heap_increase_size(heap *heap)
{
    heap->alloc_size *= 2;

    assert(realloc(heap->root, heap->alloc_size) != NULL);
}

void exchange(node *arr, int index1, int index2)
{
    node temp = arr[index1];

    arr[index1] = arr[index2];
    arr[index2] = temp;
}

// void heap_print(heap *heap)
// {
//     int i;

//     for (i = 0; i < heap->size; i++)
//         printf("%d ", heap->root[i].priority);

//     puts("");
// }

#endif
