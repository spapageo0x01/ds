#include <stdlib.h>
#include <pthread.h>

typedef struct list_node {
	void *data;
	struct list_node *next;
} node_t;

typedef struct list {
	node_t *head;
	pthread_mutex_t lock;
} list_t;


list_t * list_init(int *error);
int list_destroy(list_t *list, int *error);

size_t list_length(list_t *list, int *error);
int list_isempty(list_t *list, int *error);

int list_insert_head(list_t *list, void *data, int *error);
int list_insert_tail(list_t *list, void *data, int *error);
int list_insert_sorted(list_t *list, void *data, int (*compare)(const void *, const void *), int *error);

// 0 if not, 1 if exists
int list_lookup(list_t *list, void *data, int (*isequal)(const void *, const void *), int *error);

void * list_remove_head(list_t *list, int *error);
void * list_remove_tail(list_t *list, int *error);
