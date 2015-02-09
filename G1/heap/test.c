#include <stdio.h>
#include "heap.h"

int main(int argc, char const *argv[])
{
    heap heap;
    char *str1 = "one";
    char *str2 = "two";
    char *str3 = "three";
    char *str4 = "four";
    char *str5 = "five";
    char *str6 = "six";
    char *str7 = "seven";

    heap_initialize(&heap);

    heap_insert(&heap, (void *) str1, 10);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    heap_insert(&heap, (void *) str2, 0);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    heap_insert(&heap, (void *) str3, 5);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    heap_insert(&heap, (void *) str4, 1);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    heap_insert(&heap, (void *) str5, 107);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    heap_insert(&heap, (void *) str6, 4);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    heap_insert(&heap, (void *) str7, 7);
    printf("top is \"%s\"\n", (char *) heap_top(&heap));

    printf("size of heap: %zu\n", heap_size(&heap));

    heap_clear(&heap);

    return 0;
}
