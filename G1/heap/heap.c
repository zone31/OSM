#ifndef HEAP_C
#define HEAP_C

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"

void heap_initialize(heap *heap)
{
    size_t alloc_size = sizeof(node);
    node *root = malloc(sizeof(node)); /* Make space for 100 things. */

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

    //bottom up method
    while (heap->root[PARENT(pos)].priority < heap->root[pos].priority) {
        exchange(heap->root, pos, PARENT(pos));
        pos = PARENT(pos);
    }
}

void max_heapify(heap *heap, int i)
{
    int size = heap->size;
    int l = LEFT(i);
    int r = RIGHT(i);
    int larg;
    node *root = heap->root;

    larg = (l <= size-1 && root[l].priority > root[i].priority) ? l : i;
    larg = (r <= size-1 && root[r].priority > root[larg].priority) ? r : larg;

    if (larg != i) {
        exchange(root, i, larg);
        max_heapify(heap, larg);
    }
}

void * heap_pop(heap *heap)
{
    void *max;

    if (heap->size < 1)
        return NULL;

    max = heap->root->thing;                  /* Top element in the array. */
    *heap->root = heap->root[heap->size - 1]; /* Move last node to the front. */
    heap->size--;

    max_heapify(heap, 0);

    return max;
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

void heap_print_char(heap* heap, int level, int pos)
{
    int i;

    if (heap->size!=0) {
        for (i = 0; i < level; i++)
            printf("  ");

        printf("%u %s :%u: \n",
        heap->root[pos].priority, (char *) heap->root[pos].thing, pos);

        if (LEFT(pos)<heap->size)
            heap_print_char(heap, level+1, LEFT(pos));

        if (RIGHT(pos)<heap->size)
            heap_print_char(heap, level+1, RIGHT(pos));
    }
}

#endif
