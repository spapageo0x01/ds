#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "doubly_linked_list.h"

// Input: void
// Return: A newly allocted doubly linked list.
// Time complexity: O(1)
dlist_t * dlist_init(void)
{
	dlist_t *new_list;

	new_list = malloc(sizeof(dlist_t));
	if (new_list == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	new_list->head = NULL;
	pthread_mutex_init(&new_list->lock, NULL);

	return new_list;
}

// Input: A list
// Description: Frees all nodes and the list itself
// Returns: 1 if operation succeeds. Otherwise returns 0.
// Time complexity: O(N) worst case
int dlist_destroy(dlist_t *L)
{
	dnode_t * dest_node;
	dnode_t * temp_node;
	if (L == NULL) {
		return 1;
	}

	if (L->head == NULL) {
		goto free_out;
	}

	pthread_mutex_unlock(&L->lock);

	temp_node = L->head;
	dest_node = NULL;

	while(temp_node->next) {
		dest_node = temp_node;
		temp_node = temp_node->next;
		temp_node->prev = NULL;
		L->head = temp_node;
		free(dest_node);
	}
	pthread_mutex_unlock(&L->lock);
free_out:
	free(temp_node);
	pthread_mutex_destroy(&L->lock);
	free(L);
	return 1;
}

// Input: a list
// Returns 1 if the list is empty. Otherwise returns 0.
// Time complexity: O(1)
int dlist_is_empty(dlist_t *L)
{
	return (L->head == NULL) ? 1 : 0;
}

// Input: a list and the data to be inserted
// Return: void
// Description: Allocates a new node and inserts it a the head of the list
// Time complexity: O(1)
void dlist_insert(dlist_t *L, int x)
{
	dnode_t *new_node;

	if (L == NULL) {
		fprintf(stderr, "List given is NULL\n");
		//printf("List given is NULL\n");
		return;
	}

	new_node = malloc(sizeof(dnode_t));
	if (new_node == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		return;
	}

	new_node->key = x;
	new_node->prev = NULL;

	pthread_mutex_lock(&L->lock);
	new_node->next = L->head;
	if (L->head != NULL) {
		L->head->prev = new_node;
	}
	L->head = new_node;
	pthread_mutex_unlock(&L->lock);
}

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
void dlist_delete(dlist_t *L, int key) {
	int steps = 0;
	dnode_t *temp = NULL;

	if (L == NULL) {
		fprintf(stderr, "List given is NULL\n");
		return;
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


int main() {
	int i;
	dlist_t *L;

	L = dlist_init();
	if (!L) {printf("Death\n"); return 1;}

	dlist_insert(L, 10);
	dlist_insert(L, 5);
	dlist_insert(L, 12);

	dlist_print(L);

	dlist_delete(L, 5);
	dlist_print(L);


	dlist_destroy(L);
}



