#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

list_t * list_init(void)
{
	list_t *new_list;

	new_list = malloc(sizeof(list_t));
	if (new_list == NULL) {
		fprintf(stderr, "[%s] - Failed to allocate memory\n", __func__);
		return NULL;
	}

	new_list->head = NULL;
	pthread_mutex_init(&new_list->lock, NULL);

	return new_list;
}


// Note: No threads should be using the list!
int list_destroy(list_t *list)
{
	node_t *tmp, *next;

	// 0. Check if list is NULL
	if (list == NULL) {
		fprintf(stderr,"[%s] - NULL list provided\n", __func__);
		return EXIT_FAILURE;
	}

	// 1. Check if there are any nodes within the list. If so, delete them
	if (list->head != NULL) {
		tmp = list->head;
		do {
			next = tmp->next;
			free(tmp);
			tmp = next;
		} while (tmp != NULL);
	}

	// 2. Free the list descriptor.
	pthread_mutex_destroy(&list->lock);
	free(list);

	return EXIT_SUCCESS;
}





int list_insert_head(list_t *list, void *data)
{
	node_t *new_node;

	if (list == NULL) {
		fprintf(stderr, "[%s] NULL list provided\n", __func__);
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (new_node == NULL) {
		fprintf(stderr, "[%s] Failed to allocate memory\n", __func__);
		return EXIT_FAILURE;
	}
	new_node->data = data;
	new_node->next = NULL;

	pthread_mutex_lock(&list->lock);
	new_node->next = list->head;
	list->head = new_node;
	pthread_mutex_unlock(&list->lock);
	return EXIT_SUCCESS;
}


int list_insert_tail(list_t *list, void *data)
{
	node_t *new_node, *tmp;

	if (list == NULL) {
		fprintf(stderr, "[%s] NULL list provided\n", __func__);
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (new_node == NULL) {
		fprintf(stderr, "[%s] Failed to allocate memory\n", __func__);
		return EXIT_FAILURE;
	}
	new_node->data = data;
	new_node->next = NULL;

	pthread_mutex_lock(&list->lock);
	if (list->head == NULL) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		tmp = list->head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = new_node;
	}
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


int list_insert_sorted(list_t *list, void *data, int (*compare)(const void *, const void *))
{
	node_t *new_node, *prev, *tmp;

	if (list == NULL) {
		fprintf(stderr, "[%s] NULL list provided\n", __func__);
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (new_node == NULL) {
		fprintf(stderr, "[%s] Failed to allocate memory\n", __func__);
		return EXIT_FAILURE;
	}
	new_node->data = data;
	new_node->next = NULL;

	pthread_mutex_lock(&list->lock);
	if (list->head == NULL) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		tmp = prev = list->head;

		while (tmp != NULL) {
			if (compare(data, tmp->data)) {
				prev->next = new_node;
				new_node->next = tmp;
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}

		if (tmp == NULL) {
			prev->next = new_node;
		}
	}
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


void * list_remove_head(list_t *list)
{
	void * data = NULL;
	node_t *node;
	if ((list == NULL) || (list->head == NULL)) {
		return NULL;
	}

	pthread_mutex_lock(&list->lock);
	node = list->head;
	list->head = list->head->next;
	pthread_mutex_unlock(&list->lock);

	data = node->data;
	free(node);

	return data;
}

void * list_remove_tail(list_t *list)
{
	void * data = NULL;
	node_t *node, *prev;
	if ((list == NULL) || (list->head == NULL)) {
		return NULL;
	}

	pthread_mutex_lock(&list->lock);
	node = prev = list->head;
	while (node->next) {
		prev = node;
		node = node->next;
	}
	prev->next = NULL;
	pthread_mutex_unlock(&list->lock);

	data = node->data;
	free(node);

	return data;
}


int list_lookup(list_t *list, void *data, int (*isequal)(const void *, const void *))
{
	int found = 0;
	node_t *tmp;
	if ((list == NULL) || (list->head == NULL)) {
		return 0;
	}

	pthread_mutex_lock(&list->lock);
	tmp = list->head;
	while (tmp) {
		if (isequal(data, tmp->data)) {
			found = 1;
			break;
		}
		tmp = tmp->next;
	}
	pthread_mutex_unlock(&list->lock);

	return found;
}


size_t list_length(list_t *list)
{
	size_t length = 0;
	node_t *tmp;

	if (list == NULL) {
		fprintf(stderr, "[%s] NULL list provided\n", __func__);
		return EXIT_FAILURE;
	}

	pthread_mutex_lock(&list->lock);
	tmp = list->head;
	while (tmp != NULL) {
		length++;
		tmp = tmp->next;
	}
	pthread_mutex_unlock(&list->lock);

	return length;
}




/////////////////////////////////////////
//TODO: make tests multithreaded
int main()
{
	int ret;
	list_t *L = NULL;

	ret = list_destroy(L);
	assert(ret == EXIT_FAILURE);

	L = list_init();
	if (L == NULL) {
		fprintf(stderr, "Empty list.\n");
		return EXIT_FAILURE;
	}

	//check ret
	ret = list_destroy(L);

	return EXIT_SUCCESS;
}
