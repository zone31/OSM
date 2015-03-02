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
    printf("put     trying lock\n");
    fflush(stdout);
    pthread_mutex_lock(&q->queue_put_lock);

    printf("put     got lock\n");
    fflush(stdout);

    node_t *new = malloc(sizeof(node_t));
    assert(new != NULL);

  	new->item = item;
  	new->next = NULL;


  	/* Add the new node to the tail */
  	q->tail->next = new;
  	q->tail = new;

    printf("put     unlock\n");
    fflush(stdout);
    pthread_mutex_unlock(&q->queue_put_lock);

}

void * queue_get(queue_t *q) {
    void *item;
    node_t *old;

    printf("    get trying lock\n");
    fflush(stdout);

    pthread_mutex_lock(&q->queue_get_lock);

    printf("    get got lock\n");
    fflush(stdout);

    old = q->head;

  	/* note that the head contains a 'dummy' node. That's why we test
  	 * old->next. */
  	if (old->next == NULL) {
      pthread_mutex_unlock(&q->queue_get_lock);
      return NULL; /* queue was empty */
  	}

  	item = old->next->item;

  	/* update the head and free the old memory */
  	q->head = old->next;


    free(old);
    printf("    get unlock \n");
    fflush(stdout);
    pthread_mutex_unlock(&q->queue_get_lock);



    return item;
}
