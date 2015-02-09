#include <stddef.h> // size_t

typedef struct {
  void* thing;
  int priority;
} node;

typedef struct {
  node* root;
  size_t size;
  /* You may want to allocate more
     space than strictly needed. */
  size_t alloc_size;
} heap;

void heap_initialize(heap*);
void heap_clear(heap*);
size_t heap_size(heap*);
void* heap_top(heap*);
void heap_insert(heap*, void*, int);
void *heap_pop(heap*);
