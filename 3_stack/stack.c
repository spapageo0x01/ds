#include "stack.h"
#include <pthread.h>


#define MAX(A,B) (((A) > (B)) ? (A) : (B))

static const size_t MINIMUM_CAPACITY = 10;


stack_t * stack_init(size_t capacity, int *error)
{
	stack_t *new_stack;
	*error = NO_ERROR;

	new_stack = malloc(sizeof(stack_t));
	if (new_stack == NULL) {
		*error = ERROR_ALLOC;
		return NULL;
	}

	new_stack->size = 0;
	new_stack->capacity = MAX(capacity, MINIMUM_CAPACITY);
	new_stack->storage = malloc(sizeof(void*) * new_stack->capacity);

	pthread_mutex_init(&new_stack->mutex, NULL);
	pthread_cond_init(&new_stack->empty_condition_variable, NULL);
	pthread_cond_init(&new_stack->full_condition_variable, NULL);

	return new_stack;
}


int stack_destroy(stack_t *stack, int *error)
{
	*error = NO_ERROR;

	if (stack == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

	free(stack->storage);
	pthread_mutex_destroy(&stack->mutex);
	pthread_cond_destroy(&stack->empty_condition_variable);
	pthread_cond_destroy(&stack->full_condition_variable);

	free(stack);

	return EXIT_SUCCESS;
}



int stack_push(stack_t* stack, void* data, int *error)
{
	*error = NO_ERROR;

	if (stack == NULL) {
		*error = ERROR_NULL;
		return EXIT_FAILURE;
	}

    pthread_mutex_lock(&stack->mutex);

    while (stack->size == stack->capacity)
    {
        pthread_cond_wait(&stack->full_condition_variable, &stack->mutex);
    }

    stack->storage[stack->size++] = data;

    pthread_cond_signal(&stack->empty_condition_variable);
    pthread_mutex_unlock(&stack->mutex);

    return EXIT_SUCCESS;
}



void * stack_pop(stack_t* stack, int *error)
{
	void *data = NULL;
	*error = NO_ERROR;

	if (stack == NULL) {
		*error = ERROR_NULL;
		return NULL;
	}

    pthread_mutex_lock(&stack->mutex);

    while (stack->size == 0)
    {
        pthread_cond_wait(&stack->empty_condition_variable, &stack->mutex);
    }

    data = stack->storage[stack->size - 1];
    stack->size--;

    pthread_cond_signal(&stack->full_condition_variable);
    pthread_mutex_unlock(&stack->mutex);

    return data;
}

//-----

void * stack_top(stack_t* stack, int *error)
{
    void* data;
    *error = NO_ERROR;

	if (stack == NULL) {
		*error = ERROR_NULL;
		return NULL;
	}

    pthread_mutex_lock(&stack->mutex);

    while (stack->size == 0)
    {
        pthread_cond_wait(&stack->empty_condition_variable, &stack->mutex);
    }

    data = stack->storage[stack->size - 1];

    pthread_cond_signal(&stack->full_condition_variable);
    pthread_mutex_unlock(&stack->mutex);

    return data;
}

size_t stack_size(stack_t* stack, int *error)
{
	size_t size;
	*error = NO_ERROR;

	if (stack == NULL) {
		*error = ERROR_NULL;
		return 0;
	}

	pthread_mutex_lock(&stack->mutex);
	size = stack->size;
	pthread_mutex_unlock(&stack->mutex);

	return size;
}

size_t stack_capacity(stack_t* stack, int *error)
{
	*error = NO_ERROR;

	if (stack == NULL) {
		*error = ERROR_NULL;
		return 0;
	}

	return stack->capacity;
}
