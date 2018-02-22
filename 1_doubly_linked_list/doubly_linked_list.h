#include <stdlib.h>
#include <pthread.h>

typedef struct dlist_object {
	void *data;
	struct dlist_object *next;
	struct dlist_object *prev;
} dnode_t;

typedef struct dlist {
	dnode_t *head;
	size_t length;
	pthread_mutex_t lock;
} dlist_t;


dlist_t * dlist_init(int *error);				// OK
int dlist_destroy(dlist_t *list, int *error);	// OK

int dlist_isempty(dlist_t *list);				// OK
size_t dlist_length(dlist_t *list, int *error);	// OK


int dlist_insert_head(dlist_t *list, void *data, int *error);	//TODO
int dlist_insert_tail(dlist_t *list, void *data, int *error);	//TODO
int dlist_insert_sorted(dlist_t *list, void *data, int (*compare)(const void *, const void *), int *error);	// TODO

int dlist_lookup(dlist_t *list, void *data, int (*isequal)(const void * const void *), int *error);	// TODO

dnode_t * dlist_remove_head(dlist_t *list, int *error);	//TODO
dnode_t * dlist_remove_tail(dlist_t *list, int *error); // TODO