#ifndef __SPAP_STACK_H__
#define __SPAP_STACK_H__

#include <stdlib.h>
#include "../error/errors_sp.h"

#define MAX_ELEMENTS 64

typedef struct stack {
	size_t size;
	size_t capacity;
	void **storage;

	pthread_mutex_t mutex;
	pthread_cond_t empty_condition_variable;
	pthread_cond_t full_condition_variable;
} stack_t;


stack_t * stack_init(size_t capacity, int *error);
int stack_destroy(stack_t *stack, int *error);


int stack_push(stack_t* stack, void* data, int *error);

#endif