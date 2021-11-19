CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: solarc
default: solarc

solarc.o: solarc.c
	$(CC) $(CFLAGS) -c solarc.c

solarc: solarc.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) solarc.o -o solarc -lgpiod

clean :
	rm -f solarc *.o
