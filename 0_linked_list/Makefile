all:
	gcc -c -Wall -Werror -fPIC -g -lpthread linked_list.c
	gcc -shared -o libllsp.so linked_list.o

install:
	cp libllsp.so /usr/local/lib
	ldconfig -n /usr/local/lib
clean:
	rm -f linked_list.o libllsp.so /usr/local/lib/libllsp.so
