#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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


void queue_init(queue_t *q)
{
	node_t *tmp = malloc(sizeof(node_t));
	assert(tmp);

	tmp->next = NULL;
	q->head = q->tail = tmp;
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


int main(void)
{
	int ret;
	queue_t queue;
	queue_init(&queue);
	assert(&queue);

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
