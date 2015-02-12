#include <stdio.h>
#include "heap.h"

int main(int argc, char const *argv[])
{
    heap heap;

    int charsize = 8;
    char* chararray[charsize];
    chararray[0] = "zero";
    chararray[1] = "one";
    chararray[2] = "two";
    chararray[3] = "three";
    chararray[4] = "four";
    chararray[5] = "five";
    chararray[6] = "six";
    chararray[7] = "seven";

    int priarray[charsize];
    priarray[0] = 5;
    priarray[1] = 2;
    priarray[2] = 3;
    priarray[3] = 6;
    priarray[4] = 7;
    priarray[5] = 3;
    priarray[6] = 3;
    priarray[7] = 10;


    heap_initialize(&heap);
    printf("Creating heap##################################################\n");
    for (int i = 0; i < charsize; i++){
      heap_insert(&heap, (void *) chararray[i], priarray[i]);
      printf("inserted \"%s\" into heap, with priority %u\n",chararray[i],priarray[i]);
      printf("top is now \"%s\"\n", (char *) heap_top(&heap));
      printf("size of heap: %zu\n", heap_size(&heap));
      printf("assigned mem for heap %zu\n",heap.alloc_size/sizeof(node));
      printf("Printing heap (:x: is array pos)\n");
      heap_print_char(&heap,0,0);
      printf("\n");
    }
    printf("Creating heap ENDED--------------------------------------------\n");
    printf("Poping heap####################################################\n");
    for (int i = 0; i < charsize; i++){
      printf("Returned popped top is %s\n", (char*) heap_pop(&heap));
      printf("top is now %s\n", (char*) heap_top(&heap));
      printf("size of heap: %zu\n", heap_size(&heap));
      printf("assigned mem for heap %zu\n",heap.alloc_size/sizeof(node));
      heap_print_char(&heap,0,0);
      printf("\n");
    }
    printf("Poping heap ENDED----------------------------------------------\n");
    heap_clear(&heap);

    return 0;
}
