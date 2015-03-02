#include "queue.h"
#include <stdio.h>
#include <pthread.h>

typedef struct {
    int a;
} int_struct;

queue_t q;

void* putNget(void* n)
{
    queue_put(&q,n);
    pthread_exit(queue_get(&q));
}

int main(int argc, char const *argv[])
{
    int i = 0;
    const int n = 2000;
    void *ret;
    int_struct node;
    int_struct node_arr[n];
    int_struct* retur;
    pthread_t thr[n];

    queue_init(&q);

    for(i; i < n; i++) {
        node_arr[i].a = i;
        pthread_create(&(thr[i]), NULL, putNget, &node_arr[i]);
    }

    printf("NOW JOINING\n");
    i = 0;
    for(i; i < n; i++) {
        pthread_join(thr[i],&ret);
        printf("output test %d\n", ((int_struct*) ret)->a);
    }

    return 0;
}
