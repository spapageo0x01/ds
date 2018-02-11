#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 64

typedef struct stack {
	int A[MAX_ELEMENTS];
	int idx_top;
} stack_t;

stack_t * init_stack(void)
{
	int i;
	stack_t *new_stack;

	new_stack =  malloc(sizeof(stack_t));
	if (new_stack == NULL) {
		printf("Error allocating stack memory\n");
		return NULL;
	}

	new_stack->idx_top = 0;

	return new_stack;
}

void free_stack(stack_t *stack)
{
	free(stack);
}

//isempty
int is_empty(stack_t *stack) {
	return (stack->idx_top == 0) ? 1 : 0;
}

//push
void push(stack_t *stack, int x) {
	if (stack->idx_top == MAX_ELEMENTS) {
		printf("Max number of elements in stack reached!\n");
		return;
	}

	stack->A[stack->idx_top++] = x;
	return;
}

//pop
int pop(stack_t *stack) {
	if (stack->idx_top == 0) {
		printf("underflow\n");
		return -1;
	}

	return stack->A[--stack->idx_top];
}

int main(void) {
	stack_t *stack;
	int ret, i;

	stack = init_stack();
	if (stack == NULL) {
		return -1;
	}

	printf("is_empty = %d\n", is_empty(stack));

	push(stack, 55);
	printf("is_empty = %d\n", is_empty(stack));

	ret = pop(stack);
	printf("popped: %d\n", ret);
	printf("is_empty = %d\n", is_empty(stack));

	ret = pop(stack);
	printf("popped: %d\n", ret);

	free_stack(stack);
}