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
    pthread_exit(0);
}
void* get(void* n)
{
    pthread_exit(queue_get(&q));
}

int main(int argc, char const *argv[])
{
    int i = 0;
    const int n = 1000;
    void *ret;
    int_struct node_arr[n];
    pthread_t putNget_thr[n];
    pthread_t put_thr[n];
    pthread_t get_thr[n];

    queue_init(&q);

    printf("Creating treads with function that adds and deletes instant\n");
    for(i=0; i < n; i++) {
        node_arr[i].a = i;
        pthread_create(&(putNget_thr[i]), NULL, putNget, &node_arr[i]);
    }

    printf("Now joining treads, that added and deleted instant\n");
    for(i=0; i < n; i++) {
        pthread_join(putNget_thr[i],&ret);
        printf("Output from thead %d, value %d\n",i ,((int_struct*) ret)->a);
    }

    printf("Now deleting and joining in different treads\n");


    for(i = 0; i < n; i++) {
        node_arr[i].a = i;
        pthread_create(&(put_thr[i]), NULL, put, &node_arr[i]);
        pthread_create(&(get_thr[i]), NULL, get, &node_arr[i]);
    }
    for(i = 0; i < n; i++) {
        pthread_join(put_thr[i],&ret);
    }
    for(i = 0; i < n; i++) {
        pthread_join(get_thr[i],&ret);
        if(ret == NULL){
            printf("Output from thead %d, value NULL\n", (i));
        }else{
            printf("Output from thead %d, value %d\n",i,((int_struct*) ret)->a);
        }
    }
    return 0;
}
