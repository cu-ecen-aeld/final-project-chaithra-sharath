CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

ifeq ($(LDFLAGS),)
	LDFLAGS= -lgpiod
endif

all: gpioreadtest gpiowritetest
default: gpioreadtest gpiowritetest

test_gpio_read.o: test_gpio_read.c
	$(CC) $(CFLAGS) -c test_gpio_read.c

gpioreadtest : test_gpio_read.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) test_gpio_read.o -o gpioreadtest -lgpiod


test_gpio_write.o: test_gpio_write.c
	$(CC) $(CFLAGS) -c test_gpio_write.c

gpiowritetest : test_gpio_write.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) test_gpio_write.o -o gpiowritetest -lgpiod

clean :
	rm -f gpioreadtest gpiowritetest *.o
