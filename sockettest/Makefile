CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: server_test client_test
default: server_test client_test

server_test.o: server_test.c
	$(CC) $(CFLAGS) -c server_test.c

server_test : server_test.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) server_test.o -o server_test


client_test.o: client_test.c
	$(CC) $(CFLAGS) -c client_test.c

client_test : client_test.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) client_test.o -o client_test

clean :
	rm -f server_test client_test *.o
