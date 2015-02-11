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

    int intsize = 5;
    size_t intarray[intsize];
    intarray[0] = 123;
    intarray[1] = 123234;
    intarray[2] = 11111113;
    intarray[3] = 87654;
    intarray[4] = 42;

    int priarray[charsize+intsize];
    priarray[0] = 5;
    priarray[1] = 2;
    priarray[2] = 3;
    priarray[3] = 6;
    priarray[4] = 7;
    priarray[5] = 3;
    priarray[6] = 3;
    priarray[7] = 10;
    //ints from here
    priarray[8] = 12;
    priarray[9] = 7;
    priarray[10] = 16;
    priarray[11] = 1;
    priarray[12] = 19;

    heap_initialize(&heap);
    for (int i = 0; i < charsize; i++){
      heap_insert(&heap, (void *) chararray[i], priarray[i]);
      printf("inserted \"%s\" into heap, with priority %u\n",chararray[i],priarray[i]);
      printf("top is now \"%s\"\n", (char *) heap_top(&heap));
      printf("size of heap: %zu\n", heap_size(&heap));
      printf("assigned mem for heap %zu\n",heap.alloc_size/sizeof(node));
      printf("\n");
    }
    printf("-------------------------------------------------------------\n");
    printf("Test For other type (intengers)\n");
    printf("-------------------------------------------------------------\n\n");

    for (int i = 0; i < intsize; i++){
      heap_insert(&heap, (void *) intarray[i], priarray[i+charsize]);
      printf("inserted %zu into heap, with priority %u\n",intarray[i],priarray[i+charsize]);
      printf("top is now %zu\n", (size_t) heap_top(&heap));
      printf("size of heap: %zu\n", heap_size(&heap));
      printf("assigned mem for heap %zu\n",heap.alloc_size/sizeof(node));
      printf("\n");
    }

    printf("Returning and removeing top element\n\n");

    printf("Returned top is %zu\n", (size_t) heap_pop(&heap));

    printf("top is now %zu\n", (size_t) heap_top(&heap));
    printf("size of heap: %zu\n", heap_size(&heap));
    printf("assigned mem for heap %zu\n",heap.alloc_size/sizeof(node));
    printf("\n");


    heap_clear(&heap);

    return 0;
}
