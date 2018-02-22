#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include "../0_linked_list/linked_list.h"

#define NUMBER_OF_THREADS 4

struct thread_info {
	int tid;
	list_t *list;
};


void * do_work(void *data)
{
	struct thread_info *tinfo = (struct thread_info *) data;

	// Do work
	printf("%d\n", tinfo->tid);

	pthread_exit(NULL);
}

int main()
{
	int i = 0;
	int ret = 0;
	int error = NO_ERROR;
	list_t * test_list;
	pthread_t worker_threads[NUMBER_OF_THREADS];
	struct thread_info tinfo[NUMBER_OF_THREADS];

	// 0. Init data structure
	test_list = list_init(&error);
	assert(test_list);
	assert(error == NO_ERROR);

	// Spawn threads
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		tinfo[i].tid = i;
		tinfo[i].list = test_list;

		if (pthread_create(&worker_threads[i], NULL, do_work, (void *) &tinfo[i])) {
			printf("Error creating thread %d\n", i);
			return EXIT_FAILURE;
		}
	}

	// Wait for threads to finish
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		if (pthread_join(worker_threads[i], NULL)) {
			printf("Error joining thread %d\n", i);
		}
	}

	// Destroy data structure
	ret = list_destroy(test_list, &error);
	assert(ret == EXIT_SUCCESS);
	assert(error == NO_ERROR);

	return EXIT_SUCCESS;
}