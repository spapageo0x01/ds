#include <stdlib.h>
#include <pthread.h>
#include "../error/errors_sp.h"

typedef struct __bst_t {
	void *data;
	struct __bst_t left;
	struct __bst_t right;
} bst_t;

bst_t * bst_init(int *error);
int bst_destroy(bst_t *bst, int *error);

int bst_insert(bst_t *bst, void *data, int (*compare)(const void *, const void *), int *error);
int bst_delete(bst_t *bst, void *data, int (*compare)(const void *, const void *), int *error);
int bst_lookup(bst_t *bst, void *data, int (*compare)(const void *, const void *), int *error);