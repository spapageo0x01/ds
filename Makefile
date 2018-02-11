all:
	gcc -g -lpthread -o queue 2_queue/queue.c
clean:
	rm -f queue
