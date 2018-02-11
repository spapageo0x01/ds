#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct dlist_object {
	int key;									// REPLACE WITH VOID
	struct dlist_object *next;
	struct dlist_object *prev;
} dnode_t;

typedef struct dlist {
	dnode_t *head;
	pthread_mutex_t lock;
} dlist_t;

dlist_t * dlist_init(void);
int dlist_is_empty(dlist_t *L);
void dlist_insert(dlist_t *L, int x);			// REPLACE WITH VOID
dnode_t * dlist_search(dlist_t *L, int key);
void dlist_delete(dlist_t *L, int key);