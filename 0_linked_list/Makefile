all:
	gcc -c -Wall -Werror -fpic linked_list.c
	gcc -shared -o lib_linkedlist_spap.so linked_list.o

#	gcc -g -Wall -lpthread -o list 0_linked_list/linked_list.c
#	gcc -g -Wall -lpthread -o queue 2_queue/queue.c
clean:
	rm -f linked_list.o lib_linkedlist_spap.so
