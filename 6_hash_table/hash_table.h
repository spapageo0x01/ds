#include <stdlib.h>
#include <pthread.h>
#include "../0_linked_list/linked_list.h"

#define BUCKETS 101

typedef struct __string_t {
	char *str;
	size_t length;
} string_t;


typedef struct __hash_t {
	list_t lists[BUCKETS];
} hash_t;

hash_t * hash_init(int *error);
void hash_destroy(hash_t *hash_table, int *error);

int hash_lookup(hash_t *hash_table, int key, int *error);

int hash_insert(hash_t *hash_table, int key, int *error);
int hash_delete(hash_t *hash_table, int key, int *error);
