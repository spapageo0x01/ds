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
	dnode_t * dest, temp;
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

//------------------------------------------------------------


// Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
// Description: Allocates a new node and inserts it a the head of the list
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



void dlist_print(dlist_t *L)
{
	dnode_t *tmp;
	if (L == NULL) {return;}
	if (L->head == NULL) {return;}

	pthread_mutex_lock(&L->lock);
	tmp = L->head;
	while(tmp) {
		printf("%d ", tmp->key);
		tmp = tmp->next;
	}
	printf("\n");
	pthread_mutex_unlock(&L->lock);
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