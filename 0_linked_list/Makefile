all:
	gcc -c -Wall -Werror -fpic linked_list.c
	gcc -shared -o libllsp.so linked_list.o

install:
	cp libllsp.so /usr/local/lib
#	gcc -g -Wall -lpthread -o list 0_linked_list/linked_list.c
#	gcc -g -Wall -lpthread -o queue 2_queue/queue.c
clean:
	rm -f linked_list.o libllsp.so
