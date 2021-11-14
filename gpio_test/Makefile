CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

#ifeq ($(LDFLAGS),)
	LDFLAGS= -lgpiod
#endif

all: gpioreadtest gpiowritetest
default: gpioreadtest gpiowritetest

gpioreadtest : test_gpio_read.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o gpioreadtest test_gpio_read.o $(LDFLAGS)

gpiowritetest : test_gpio_write.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o gpiowritetest test_gpio_write.o $(LDFLAGS)

test_gpio_read.o : test_gpio_read.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c test_gpio_read.c $(LDFLAGS)

test_gpio_write.o : test_gpio_write.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -c test_gpio_write.c $(LDFLAGS)

clean :
	rm -f gpioreadtest gpiowritetest *.o
