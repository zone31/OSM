#include "queue.h"
#include <stdio.h>
#include <pthread.h>

typedef struct {
    int a;
} int_struct;

queue_t q;

void* putNget(void* n){
  queue_put(&q,n);
  pthread_exit(queue_get(&q));

}

int main(int argc, char const *argv[])
{
    int i = 0;
    const int n = 50;

    queue_init(&q);

    int_struct node;
    int_struct node_arr[n];
    int_struct* retur;
    pthread_t thr[n];
    void* ret;

    for(i;i<n;i++){
      node_arr[i].a = i;
      pthread_create(&(thr[i]), NULL, putNget, &node_arr[i]);
      //queue_put(&q,&node_arr[i]);
    }

    for(i;i>=0;i--){
      pthread_join(thr[i],&ret);
      printf("output test %d\n",((int_struct*)ret)->a);
    }


    // printf("input test%p\n",(void*)&b);
    //
    //
    //
    // int_struct* retur = (int_struct*) queue_get(&q);
    // retur = (int_struct*) queue_get(&q);
    //
    // printf("output test%p\n",(void*)retur);
    // printf("Validatig\n");
    // if (retur->a == c.a){
    //   printf("Worked!!\n");
    // }

    return 0;
}
