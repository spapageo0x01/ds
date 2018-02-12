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


list_t * list_init(void);
int list_destroy(list_t *list);
int list_insert_head(list_t *list, void *data);
int list_insert_tail(list_t *list, void *data);
size_t list_length(list_t *list);
//-----
int list_insert_sort(list_t *list, void *data, int (*compare)(const void *, const void *));

//int list_delete_element(list_t *list, void *data);

//node_t list_search(list_t *list);
int list_delete_head();	//?
int list_delete_tail();	//?
