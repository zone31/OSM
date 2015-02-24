#include "queue.h"
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

pthread_mutex_t queue_put_lock;
pthread_mutex_t queue_get_lock;

void queue_init(queue_t *q)
{
    q->head = NULL;
    q->tail = NULL;
}

void queue_put(queue_t *q, void *item)
{
    node_t *new = malloc(sizeof(node_t));

    assert(new != NULL);

    pthread_mutex_lock(&queue_put_lock);

    if (QUEUE_EMPTY(q)) {
        q->head = new;
        q->tail = new;
    } else {
        new->item = item;
        new->next = NULL;

        /* Insert on the tail. */
        q->tail->next = new;
        q->tail = new;
    }

    pthread_mutex_unlock(&queue_put_lock);
}

void * queue_get(queue_t *q) {
    node_t *old;
    void *item;

    if (QUEUE_EMPTY(q))
        return NULL;

    pthread_mutex_lock(&queue_get_lock);

    old = q->head;
    item = q->head->item;

    if (q->head->next == NULL) {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = old->next;
    }

    free(old);

    pthread_mutex_unlock(&queue_get_lock);

    return item;
}
