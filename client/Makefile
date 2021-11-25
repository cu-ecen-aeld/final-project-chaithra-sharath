CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: client

default: client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

client: client.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) client.o -o client -lgpiod

clean :
	rm -f client *.o
