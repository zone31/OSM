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

void * heap_pop(heap* heap){
  void* rthing = heap_top(heap);
  heap->size -= 1;
  exchange(heap->root, 0, heap->size);
  heapify(heap,0);
  return rthing;
}



/* Help functions. */
void heap_increase_size(heap *heap)
{
    heap->alloc_size *= 2;

    assert(realloc(heap->root, heap->alloc_size) != NULL);
}

void heapify(heap* heap,int this){
    int right   = RIGHT(this);
    int left    = LEFT (this);
    int this_p  = heap->root[this ].priority;
    int right_p = heap->root[right].priority;
    int left_p  = heap->root[left ].priority;
    int mover   = NULL;
    int mover_p = NULL;
    // if left exist, set as move target
    if (left <  heap->size){
      mover_p = left_p;
      mover   = left;
    }
    // if right exist, and is bigger than left priority
    if (right < heap->size && left_p < right_p){
      mover_p = right_p;
      mover   = right;
    }
    // if the childen are bigger than the node in focus, flip biggest
    if (this_p < mover_p){
      exchange(heap->root,this,mover);
      heapify(heap,mover);
    }
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
