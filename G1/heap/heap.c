#ifndef HEAP_C
#define HEAP_C

#include <stdio.h> /* REMOVE. */
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
  exchange(heap->root, 0, heap->size - 1);
  int pos = 0;
  int run_heapify = 1;



  while (run_heapify == 1) {
    int scope = heap->root[      pos ].priority;
    int right = heap->root[RIGHT(pos)].priority;
    int left  = heap->root[LEFT (pos)].priority;
      //Testing if left and right node exist
      if(LEFT(pos)<=heap->size && RIGHT(pos)<=heap->size){
          // if both nodes are lower, end iteration
          if(scope>=left && scope>=right){
            run_heapify = 0;
          }
          //if both are bigger, take biggest
          else if(scope<right && scope<left){
              if(left<right){
                  exchange(heap->root,pos,RIGHT(pos));
                  pos = RIGHT(pos);
              }
              else{
                  exchange(heap->root,pos,LEFT(pos));
                  pos = LEFT(pos);
              }
          }
          //take left
          else if(scope<left){
              exchange(heap->root,pos,LEFT(pos));
              pos = LEFT(pos);
          }
          //take right
          else if(scope<right){
              exchange(heap->root,pos,RIGHT(pos));
              pos = RIGHT(pos);
          }

      }
      else if (LEFT(pos)<=heap->size){
          if(scope<left){
              exchange(heap->root,pos,LEFT(pos));
              pos = LEFT(pos);
          }
          else{
            run_heapify = 0;
          }
      }
      else{
        run_heapify = 0;
      }
  }


  return rthing;
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
//
//     for (i = 0; i < heap->size; i++)
//         printf("%d ", heap->root[i].priority);
//
//     puts("");
// }

#endif
