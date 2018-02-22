#include <stdio.h>
#include "doubly_linked_list.h"
#include "../error/errors_sp.h"

// Input: void
// Return: A newly allocted doubly linked list.
// Time complexity: O(1)
dlist_t * dlist_init(int *error)
{
	dlist_t *new_list;
	*error = NO_ERROR;

	new_list = malloc(sizeof(dlist_t));
	if (!new_list) {
		*error = ERROR_NULL;
		return NULL;
	}

	new_list->head = NULL;
	new_list->length = 0;
	pthread_mutex_init(&new_list->lock, NULL);

	return new_list;
}

// Input: A list
// Description: Frees all nodes and the list itself
// Returns: 1 if operation succeeds. Otherwise returns 0.
// Time complexity: O(N) worst case
int dlist_destroy(dlist_t *list, int *error)
{
	dnode_t * dest;
	dnode_t * temp;
	*error = NO_ERROR;

	if (!list) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	if (list->head != NULL) {
		pthread_mutex_lock(&list->lock);
		temp = list->head;
		while (temp->next) {
			dest = temp;
			temp = temp->next;
			temp->prev = NULL;
			free(dest);
		}
		free(temp);
		pthread_mutex_unlock(&list->lock);
	}

	pthread_mutex_destroy(&list->lock);
	free(list);

	return EXIT_SUCCESS;
}

// Returns 1 if the list is empty. Otherwise returns 0.
// Time complexity: O(1)
int dlist_isempty(dlist_t *list)
{
	return (list->head == NULL) ? 1 : 0;
}

size_t dlist_length(dlist_t *list, int *error)
{
	*error = NO_ERROR;
	if (!list) { 
		*error = ERROR_NULL;
		return 0;
	}

	return list->length;
}


// Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
// Description: Allocates a new node and inserts it at the head of the list
// Time complexity: O(1)
int dlist_insert_head(dlist_t *list, void *data, int *error)
{
	dnode_t *new_node;
	*error = NO_ERROR;

	if (list == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(dnode_t));
	if (new_node == NULL) {
		*error = ERROR_ALLOC;
		return EXIT_FAILURE;
	}

	new_node->data = data;
	new_node->prev = NULL;

	pthread_mutex_lock(&list->lock);
	new_node->next = list->head;
	if (list->head != NULL) {
		list->head->prev = new_node;
	}
	list->head = new_node;
	++list->length;
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


// Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
// Description: Allocates a new node and inserts it at the tail of the list
// Time complexity: O(1)
int dlist_insert_tail(dlist_t *list, void *data, int *error)
{
	dnode_t *new_node, *temp;
	*error = NO_ERROR;

	if (list == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	new_node = malloc(sizeof(dnode_t));
	if (new_node == NULL) {
		*error = ERROR_ALLOC;
		return EXIT_FAILURE;
	}

	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;

	pthread_mutex_lock(&list->lock);
	if (list->head == NULL) {
		list->head = new_node;
	} else {
		temp = list->head;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = new_node;
		new_node->prev = temp;
	}
	++list->length;
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


// Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
// Description: Allocates a new node and inserts it sorted within the list (based on the compare 
// function)
// Time complexity: O(1)
int dlist_insert_sorted(dlist_t *list, void *data, int (*compare)(const void *, const void *), int *error)
{
	dnode_t *new_node, *temp, *prev;
	*error = NO_ERROR;

	// Check list provided is not NULL
	if (list == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	// Allocate new node
	new_node = malloc(sizeof(dnode_t));
	if (new_node == NULL) {
		*error = ERROR_ALLOC;
		return EXIT_FAILURE;
	}

	// Initialize new node
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;

	// Acquire list lock
	pthread_mutex_lock(&list->lock);
	if (list->head == NULL) {
		// 0. If list head is NULL, this is the only node. Simply add
		list->head = new_node;
	} else {
		// 1. The list is not empty. Traverse it until compare() returns 1. Insert
		// the new node before the "temp" node.
		temp = list->head;
		do {
			if (compare(data, temp->data)) {
				new_node->next = temp;
				new_node->prev = temp->prev;
				temp->prev = new_node;
				break;
			}
			prev = temp;
			temp = temp->next;
		} while (temp);
		

		// 2. Compare never evaluated as true, therefore simply add node to the tail of the list.
		if (temp == NULL) {
			prev->next = new_node;
			new_node->prev = prev;
		}

	}
	++list->length;
	pthread_mutex_unlock(&list->lock);

	return EXIT_SUCCESS;
}


// Time complexity: O(n)
int dlist_lookup(dlist_t *list, void *data, int (*isequal)(const void *, const void *), int *error)
{
	int found = 0;
	dnode_t *tmp;
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
void * dlist_remove_head(dlist_t *list, int *error)
{
	void * data = NULL;
	dnode_t *node;
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
	list->head->prev = NULL;
	--list->length;
	pthread_mutex_unlock(&list->lock);

	data = node->data;
	free(node);

	return data;
}



// Time complexity: O(n)
void * dlist_remove_tail(dlist_t *list, int *error)
{
	void * data = NULL;
	dnode_t *node;
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
	while (node->next) {
		node = node->next;
	}
	node->prev->next = NULL;
	--list->length;
	pthread_mutex_unlock(&list->lock);

	data = node->data;
	free(node);

	return data;
}



/*
static void __dlist_print(dlist_t *list)
{
	dnode_t *tmp;
	if (list == NULL) {return;}
	if (list->head == NULL) {return;}

	pthread_mutex_lock(&list->lock);
	tmp = list->head;
	while (tmp) {
		printf("%d ", tmp->key);
		tmp = tmp->next;
	}
	printf("\n");
	pthread_mutex_unlock(&list->lock);
}
*/

/*

//Returns pointer to element with key provided, NULL otherwise
//Time complexity O(N) worst case
dnode_t * dlist_search(dlist_t *L, int key) {
	dnode_t *temp = NULL;

	if (L == NULL) {
		fprintf(stderr, "List given is NULL\n");
		return NULL;
	}

	pthread_mutex_lock(&L->lock);
	temp = L->head;
	while (temp != NULL) {
		if (temp->key == key) {
			pthread_mutex_unlock(&L->lock);
			return temp;
		}
		temp = temp->next;
	}
	pthread_mutex_unlock(&L->lock);

	return NULL;
}


// Time complexity O(N) worst case
int dlist_delete(dlist_t *L, int key) {
	int steps = 0;
	dnode_t *temp = NULL;

	if (L == NULL) {
		fprintf(stderr, "List given is NULL\n");
		return EXIT_FAILURE;
	}

	pthread_mutex_lock(&L->lock);
	temp = L->head;
	while (temp != NULL) {
		if (temp->key == key) {
			if (temp->next == NULL) {
				temp->prev->next = NULL;
			} else {
				temp->next->prev = temp->prev;
			}

			if (temp->prev == NULL) {
				// Head of the list
				L->head = temp->next;
			} else {
				temp->prev->next = temp->next;
			}
			free(temp);
			break;
		} else {
			// Key not found - Advance search
			temp = temp->next;
		}
	}
	pthread_mutex_unlock(&L->lock);

	return EXIT_SUCCESS;
}


*/

// int main() {
// 	int i;
// 	dlist_t *L;

// 	L = dlist_init();

// 	if (!L) {
// 		printf("Death\n"); 
// 		return 1;
// 	}

// 	dlist_insert(L, 10);
// 	dlist_insert(L, 5);
// 	dlist_insert(L, 12);

// 	dlist_print(L);

// 	dlist_delete(L, 5);
// 	dlist_print(L);


// 	dlist_destroy(L);
// }