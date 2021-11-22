CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: server client_test
default: server client_test

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

server : server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) server.o -o server


client_test.o: client_test.c
	$(CC) $(CFLAGS) -c client_test.c

client_test : client_test.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) client_test.o -o client_test

clean :
	rm -f server client_test *.o
