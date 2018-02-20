#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define NUMBER_OF_THREADS 4

struct thread_info {
	int tid;
};


void * do_work(void *data)
{
	struct thread_info *tinfo = (struct thread_info *) data;

	printf("%d\n", tinfo->tid);

	pthread_exit(NULL);
}

int main()
{
	int i = 0;
	pthread_t worker_threads[NUMBER_OF_THREADS];
	struct thread_info tinfo[NUMBER_OF_THREADS];

	// Spawn threads
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		tinfo[i].tid = i;

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


        return EXIT_SUCCESS;
}
