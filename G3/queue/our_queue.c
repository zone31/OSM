#include "queue.h"
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t queue_put_lock ;
pthread_mutex_t queue_get_lock ;

void queue_init(queue_t *q)
{
    q->head = NULL;
    q->tail = NULL;
}

void queue_put(queue_t *q, void *item)
{
    pthread_mutex_lock(&queue_put_lock);
    node_t *new = malloc(sizeof(node_t));

    assert(new != NULL);


    new->item = item;
    new->next = NULL;
    if (q->head == NULL || q->tail == NULL) {

        q->head = new;
        q->tail = new;
    } else {
        /* Insert on the tail. */
        q->tail->next = new;
        q->tail = new;
    }

    pthread_mutex_unlock(&queue_put_lock);
}

void * queue_get(queue_t *q) {
    node_t *old;
    void *item;

    pthread_mutex_lock(&queue_get_lock);

    if (QUEUE_EMPTY(q)){
        return NULL;
    }


    old = q->head;
    item = q->head->item;

    if (q->head->next == NULL) {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = old->next;
    }



    pthread_mutex_unlock(&queue_get_lock);

    free(old);
    
    return item;
}
