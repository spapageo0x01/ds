all:
	gcc -c -Wall -Werror -fPIC -g -lpthread queue.c
	gcc -shared -o libqsp.so queue.o

install:
	cp libqsp.so /usr/local/lib
	ldconfig -n /usr/local/lib
clean:
	rm -f queue.o libqsp.so /usr/local/lib/libqsp.so
