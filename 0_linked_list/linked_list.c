#include <stdio.h>
#include "linked_list.h"
#include "../error/errors_sp.h"


// Time Complexity: O(1)
list_t * list_init(int *error)
{
	list_t *new_list;
	*error = NO_ERROR;

	new_list = malloc(sizeof(list_t));
	if (!new_list) {
		*error = ERROR_NULL;
		return NULL;
	}

	new_list->head = NULL;
	new_list->length = 0;
	pthread_mutex_init(&new_list->lock, NULL);

	return new_list;
}


// Time Complexity: O(n) [worst], O(1) [best]
// Note: No threads should be using the list!
int list_destroy(list_t *list, int *error)
{
	node_t *tmp, *next;
	*error = NO_ERROR;

	// 0. Check if list is NULL
	if (!list) {
		*error = ERROR_NULL;
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




// Time complexity: O(1)
int list_insert_head(list_t *list, void *data, int *error)
{
	node_t *new_node;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (!new_node) {
		*error = ERROR_ALLOC;
		return EXIT_FAILURE;
	}
	new_node->data = data;
	new_node->next = NULL;

	pthread_mutex_lock(&list->lock);
	new_node->next = list->head;
	list->head = new_node;
	++list->length;
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}

// Time complexity: O(n)
int list_insert_tail(list_t *list, void *data, int *error)
{
	node_t *new_node, *tmp;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (!new_node) {
		*error = ERROR_ALLOC;
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
	++list->length;
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


// Time complexity: O(n)
int list_insert_sorted(list_t *list, void *data, int (*compare)(const void *, const void *), int *error)
{
	node_t *new_node, *prev, *tmp;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(node_t));
	if (!new_node) {
		*error = ERROR_ALLOC;
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
	++list->length;
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


// Time complexity: O(1)
void * list_remove_head(list_t *list, int *error)
{
	void * data = NULL;
	node_t *node;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return NULL;
	}

	if (!list->head) {
		return NULL;
	}

	pthread_mutex_lock(&list->lock);
	node = list->head;
	list->head = list->head->next;
	--list->length;
	pthread_mutex_unlock(&list->lock);

	data = node->data;
	free(node);

	return data;
}


// Time complexity: O(n)
void * list_remove_tail(list_t *list, int *error)
{
	void * data = NULL;
	node_t *node, *prev;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return NULL;
	}

	if (!list->head) {
		return NULL;
	}

	pthread_mutex_lock(&list->lock);
	node = prev = list->head;
	while (node->next) {
		prev = node;
		node = node->next;
	}
	prev->next = NULL;
	--list->length;
	pthread_mutex_unlock(&list->lock);

	data = node->data;
	free(node);

	return data;
}


// Time complexity: O(n)
int list_lookup(list_t *list, void *data, int (*isequal)(const void *, const void *), int *error)
{
	int found = 0;
	node_t *tmp;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
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

// Time complexity: O(1)
size_t list_length(list_t *list, int *error)
{
	size_t *tmp;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return 0;
	}

	pthread_mutex_lock(&list->lock);
	tmp = list->length;
	pthread_mutex_unlock(&list->lock);

	return tmp;
}