#ifndef HEAP_H
#define HEAP_H

#include <stddef.h> // size_t

#define PARENT(i) ((i - 1) / 2) /* Compute the parent of an index i. */
#define LEFT(i)   (2*i + 1)     /* Compute the left child of index i. */
#define RIGHT(i)  (2*i + 2)     /* Compute the right child of index i. */

typedef struct {
    void *thing;
    int priority;
} node;

typedef struct {
    node *root;
    size_t size;
    size_t alloc_size;
} heap;

/* Functions to implement for assignment. */
void heap_initialize(heap *);
void heap_clear(heap *);
size_t heap_size(heap *);
void * heap_top(heap *);
void heap_insert(heap *, void *, int);
void * heap_pop(heap *);

/* Help functions. */
void heap_increase_size(heap *heap);
void exchange(node *arr, int index1, int index2);
void heapify(heap *, int pos);

/*Pretty print heap*/
void heap_print_char(heap *, int, int);

#endif
