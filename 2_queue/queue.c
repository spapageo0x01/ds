#include "queue.h"

//TODO: isfull() - checks if the queue is full
//TODO: isempty() - Checks if the queue is empty

queue_t * queue_init(int *error)
{
	queue_t *q;
	node_t *tmp;

	q = malloc(sizeof(queue_t));
	if (q == NULL) {
		*error = ERROR_ALLOC;
		return NULL;
	}

	tmp = malloc(sizeof(node_t));
	if (tmp == NULL) {
		*error = ERROR_ALLOC;
		return NULL;
	}

	tmp->data = NULL;
	tmp->next = NULL;
	q->head = q->tail = tmp;

	pthread_mutex_init(&q->head_lock, NULL);
	pthread_mutex_init(&q->tail_lock, NULL);

	return q;
}


int queue_destroy(queue_t *q, int *error)
{
	node_t *tmp, *prev;

	if (q == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	pthread_mutex_lock(&q->head_lock);
	pthread_mutex_lock(&q->tail_lock);
	tmp = q->head;
	while (tmp) {
		prev = tmp;
		tmp = tmp->next;
		free(prev);
	}
	pthread_mutex_unlock(&q->tail_lock);
	pthread_mutex_unlock(&q->head_lock);

	pthread_mutex_destroy(&q->tail_lock);
	pthread_mutex_destroy(&q->head_lock);

	return EXIT_SUCCESS;
}


int queue_enqueue(queue_t *q, void *data, int *error)
{
	node_t *new_node;

	if (q == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (new_node == NULL) {
		*error = ERROR_ALLOC;
		return EXIT_FAILURE;
	}

	new_node->data = data;
	new_node->next = NULL;

	pthread_mutex_lock(&q->tail_lock);
	q->tail->next = new_node;
	q->tail = new_node;
	pthread_mutex_unlock(&q->tail_lock);

	return EXIT_SUCCESS;
}



void * queue_dequeue(queue_t *q, int *error)
{
	void *data;
	node_t *tmp = NULL;
	node_t *new_head = NULL;

	if (q == NULL) {
		*error = ERROR_NULL;
		return NULL;
	}


	pthread_mutex_lock(&q->head_lock);
	tmp = q->head;
	new_head = tmp->next;
	if (new_head == NULL) {
		pthread_mutex_unlock(&q->head_lock);
		return NULL; // queue was empty
	}
	data = new_head->data;
	q->head = new_head;
	pthread_mutex_unlock(&q->head_lock);

	free(tmp);
	return data;
}



void * queue_peek(queue_t *q, int *error)
{
	void *data;

	if (q == NULL) {
		*error = ERROR_NULL;
		return NULL;
	}

	pthread_mutex_lock(&q->head_lock);
	data = q->head->data;
	pthread_mutex_unlock(&q->head_lock);

	return data;
}

//--------------------------------------------------------------------------------------------------