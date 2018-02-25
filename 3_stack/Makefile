all:
	gcc -c -Wall -Werror -fPIC -g -lpthread stack.c
	gcc -shared -o libstsp.so stack.o

install:
	cp libstsp.so /usr/local/lib
	ldconfig -n /usr/local/lib
clean:
	rm -f stack.o libstsp.so /usr/local/lib/libstsp.so
