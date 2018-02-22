#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../error/errors_sp.h"

typedef struct __node_t {
	void *data;
	struct __node_t	*next;
} node_t;

typedef struct __queue_t {
	node_t *head;
	node_t *tail;
	pthread_mutex_t head_lock;
	pthread_mutex_t tail_lock;
} queue_t;


//TODO: peek() - gets the element at the front of the queue without removing it
//TODO: isfull() - checks if the queue is full
//TODO: isempty() - Checks if the queue is empty

queue_t * queue_init(int *error);
int queue_destroy(queue_t *q, int *error);

int queue_enqueue(queue_t *q, void *data, int *error);
void * queue_dequeue(queue_t *q, int *error);

