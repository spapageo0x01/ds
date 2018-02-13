#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

//TODO: peek() - gets the element at the front of the queue without removing it
//TODO: isfull() - checks if the queue is full
//TODO: isempty() - Checks if the queue is empty

void queue_init(queue_t *q)
{
	//TODO: error checks? Retry?
	node_t *tmp = malloc(sizeof(node_t));
	assert(tmp);

	tmp->next = NULL;
	q->head = q->tail = tmp;
	//TODO: return value checks?
	pthread_mutex_init(&q->head_lock, NULL);
	pthread_mutex_init(&q->tail_lock, NULL);
}

void queue_enqueue(queue_t *q, int value) {
	node_t *tmp = malloc(sizeof(node_t));
	assert(tmp != NULL);

	tmp->value = value;
	tmp->next = NULL;


	pthread_mutex_lock(&q->tail_lock);
	q->tail->next = tmp;
	q->tail = tmp;
	pthread_mutex_unlock(&q->tail_lock);
}

int queue_dequeue(queue_t *q, int *value) {
	node_t *tmp = NULL;
	node_t *newHead = NULL;
	pthread_mutex_lock(&q->head_lock);
	tmp = q->head;
	newHead = tmp->next;
	if (newHead == NULL) {
		pthread_mutex_unlock(&q->head_lock);
		return -1; // queue was empty
	}
	*value = newHead->value;
	q->head = newHead;
	pthread_mutex_unlock(&q->head_lock);
	free(tmp);
	return 0;
}

//TODO: complete this!
void queue_destroy(queue_t *q)
{
	//free nodes?
	pthread_mutex_destroy(&q->tail_lock);
	pthread_mutex_destroy(&q->head_lock);
}



int main(void)
{
	int ret;
	queue_t queue;
	queue_init(&queue);
	assert(queue);

	queue_enqueue(&queue, 10);
	queue_enqueue(&queue, 5);
	queue_enqueue(&queue, 12);

	queue_dequeue(&queue, &ret);
	printf("%d\n", ret);
	queue_dequeue(&queue, &ret);
	printf("%d\n", ret);
	queue_dequeue(&queue, &ret);
	printf("%d\n", ret);
}
