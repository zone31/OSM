
#include "tests/lib.h"

int main() {
    heap_init();
    int i;
    int size = 63;
    int *array=(int *)malloc(size*sizeof(int));
    free(array);
    for (i = 0; (size)>i;i++){
      array[i]=i;
    }
    for (i = 0; (size)>i;i++){
      printf("number %d\n",array[i]);
    }
    printf("test %d \n",(int)array);
    syscall_memlimit((void*)5);


    return 0;
}
