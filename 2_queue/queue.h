#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct __node_t {
	int 			value;
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

void queue_init(queue_t *q);
void queue_enqueue(queue_t *q, int value);
int queue_dequeue(queue_t *q, int *value);

//TODO: complete this!
void queue_destroy(queue_t *q);
