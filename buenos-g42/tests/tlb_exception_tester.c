#include "tests/lib.h"

int main(void)
{
    /* heap_init(); */
    int foo[8000];
    unsigned int i;

    for (i = 0; i < sizeof(foo)/sizeof(int); i++) {
        printf("i: %d\n\n", i);
        /* foo[i] = i; */
    }
    return 0;
}

/* int *a = malloc(252); */

/* if (a == NULL){ */
/*     printf("a giver fejl, Magnus\n\n"); */
/* } */

/* int *b = malloc(1); */

/* if (b == NULL){ */
/*     printf("b giver fejl, ik' os' Magnus\n\n"); */
/* } */

/* int i; */
/* const int n = 200; */
/* int bytes = 4096; */
/* // int *py_ptr; */
/* int *arr[200]; */

/* // printf("%p\n", arr[0]); */

/* for (i = 0; i < n; i++) { */
/*     if (i % 1000 == 0) */
/*         printf("Allocating %d bytes, counter %d\n", bytes, i); */

/*     // py_ptr = malloc(bytes); */
/*     arr[i] = malloc(bytes); */
/* } */

/* for (i = 0; i < n; i++) { */
/*     printf("Accessing block %p\n", arr[i]); */
/*     *arr[i] = i; */
/* } */
