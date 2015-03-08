#include "tests/lib.h"

int main(void)
{
    int i;
    const int n = 200;
    int bytes = 4096;
    // int *py_ptr;
    int *arr[200];

    // printf("%p\n", arr[0]);

    for (i = 0; i < n; i++) {
        if (i % 1000 == 0)
            printf("Allocating %d bytes, counter %d\n", bytes, i);

        // py_ptr = malloc(bytes);
        arr[i] = malloc(bytes);
    }

    for (i = 0; i < n; i++) {
        printf("Accessing block %p\n", arr[i]);
        *arr[i] = i;
    }

    return 0;
}
