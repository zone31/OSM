#include <stdio.h>
#include "heap.h"

int main(int argc, char const *argv[])
{
    heap heap;
    int i;
    int charsize = 14;
    int priarray[] = {5, 2, 2, 3, 6, 7, 3, 3, 10, 15, 4, 1, 5, 6, 8};
    char* chararray[] = {"zero", "one", "two", "three", "four", "five",
        "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen"};

    heap_initialize(&heap);

    printf("Creating heap##################################################\n");
    for (i = 0; i < charsize; i++) {
        heap_insert(&heap, (void *) chararray[i], priarray[i]);

        printf("inserted '%s' into heap, with priority %u\n", chararray[i],
            priarray[i]);
        printf("top is now '%s'\n", (char *) heap_top(&heap));
        printf("size of heap: %zu\n", heap_size(&heap));
        printf("assigned mem for heap %zu\n", heap.alloc_size);
        printf("Printing heap (:x: is array pos)\n");
        heap_print_char(&heap, 0, 0);
        printf("\n");
    }
    printf("Creating heap ENDED--------------------------------------------\n");

    printf("Poping heap####################################################\n");
    for (i = 0; i < charsize; i++) {
        printf("Returned popped top is '%s'\n", (char *) heap_pop(&heap));
        printf("top is now '%s'\n", (char *) heap_top(&heap));
        printf("size of heap: %zu\n", heap_size(&heap));
        printf("assigned mem for heap %zu\n",heap.alloc_size);
        heap_print_char(&heap, 0, 0);
        printf("\n");
    }
    printf("Poping heap ENDED----------------------------------------------\n");

    heap_clear(&heap);

    return 0;
}
