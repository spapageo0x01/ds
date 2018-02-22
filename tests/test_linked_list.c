#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
	//printf("%d\n", tinfo->tid);

	pthread_exit(NULL);
}

int isequal(const void *data_1, const void *data_2)
{
	int val_1 = (intptr_t) data_1;
	int val_2 = (intptr_t) data_2;

	if (val_1 == val_2) {
		return 1;
	}

	return 0;
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


	// 1. Single threaded tests
	ret = list_insert_head(test_list, (void *) 5, &error);
	assert(ret == EXIT_SUCCESS);
	assert(error == NO_ERROR);

	ret = list_length(test_list, &error);
	assert(ret == 1);
	assert(error == NO_ERROR);

	ret = list_lookup(test_list, (void *) 5, isequal, &error);
	assert(ret == 1);
	assert(error == NO_ERROR);


	// 2. Multi-threaded tests
	// Spawn threads
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		tinfo[i].tid = i;
		tinfo[i].list = test_list;

		if (pthread_create(&worker_threads[i], NULL, do_work, (void *) &tinfo[i])) {
			printf("Error creating thread %d\n", i);
			return EXIT_FAILURE;
		}
	}

	// 3. Wait for threads to finish
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		if (pthread_join(worker_threads[i], NULL)) {
			printf("Error joining thread %d\n", i);
		}
	}

	// 4. Destroy data structure
	ret = list_destroy(test_list, &error);
	assert(ret == EXIT_SUCCESS);
	assert(error == NO_ERROR);

	return EXIT_SUCCESS;
}