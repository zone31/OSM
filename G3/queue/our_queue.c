#include "our_queue.h"
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void queue_init(queue_t *q)
{
  node_t *new = malloc(sizeof(node_t));
  assert(new != NULL);

  new->next = NULL;
  q->head = q->tail = new;
}

void queue_put(queue_t *q, void *item)
{
    pthread_mutex_lock(&q->queue_put_lock);

    node_t *new = malloc(sizeof(node_t));
    assert(new != NULL);

  	new->item = item;
  	new->next = NULL;

  	/* add the new node to the tail */
  	q->tail->next = new;
  	q->tail = new;

    pthread_mutex_unlock(&q->queue_put_lock);
}

void * queue_get(queue_t *q) {
    pthread_mutex_lock(&q->queue_get_lock);
    node_t *old = q->head;

  	/* note that the head contains a 'dummy' node. That's why we test
  	 * old->next. */
  	if (old->next == NULL) {
  		return NULL; /* queue was empty */
  	}

  	void *item = old->next->item;

  	/* update the head and free the old memory */
  	q->head = old->next;

    pthread_mutex_unlock(&q->queue_get_lock);

    free(old);

    return item;
}
