#include "our_queue.h"
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
void* put(void* n)
{
    queue_put(&q,n);
}
void* get(void* n)
{
    pthread_exit(queue_get(&q));
}

int main(int argc, char const *argv[])
{
    int i = 0;
    const int n = 10;
    void *ret;
    int_struct node;
    int_struct node_arr[n];
    int_struct* retur;
    pthread_t putNget_thr[n];
    pthread_t put_thr[n];
    pthread_t get_thr[n];

    queue_init(&q);

    // for(i; i < n; i++) {
    //     node_arr[i].a = i;
    //     pthread_create(&(putNget_thr[i]), NULL, putNget, &node_arr[i]);
    // }
    //
    // printf("NOW JOINING\n");
    // i = 0;
    // for(i; i < n; i++) {
    //     pthread_join(putNget_thr[i],&ret);
    //     printf("output test %d\n", ((int_struct*) ret)->a);
    // }

    printf("Now deleting and joining at the same time!\n");

    i = 0;
    for(i; i < n; i++) {
        node_arr[i].a = i;
        pthread_create(&(put_thr[i]), NULL, put, &node_arr[i]);
        pthread_create(&(get_thr[i]), NULL, get, &node_arr[i]);
    }
    i = 0;
    for(i; i < n; i++) {
        pthread_join(put_thr[i],&ret);
    }
    i = 0;
    for(i; i < n; i++) {
        pthread_join(get_thr[i],&ret);
        if(ret == NULL){
            printf("Was null\n");
        }else{
            printf("output test %d\n", ((int_struct*) ret)->a);
        }
    }
    return 0;
}
