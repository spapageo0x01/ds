all:
	gcc -c -Wall -Werror -fPIC -g -lpthread doubly_linked_list.c
	gcc -shared -o libdllsp.so doubly_linked_list.o

install:
	cp libdllsp.so /usr/local/lib
	ldconfig -n /usr/local/lib
clean:
	rm -f doubly_linked_list.o libdllsp.so /usr/local/lib/libdllsp.so
