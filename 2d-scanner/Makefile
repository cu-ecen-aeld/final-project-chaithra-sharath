CC = gcc 
CFLAGS = -Wall
LDFLAGS =
OBJFILES = test_gpio_read.o test_gpio_write.o solarc.o server.o
TARGET = server_final
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS) -lrt -lgpiod
clean:
	rm -f $(OBJFILES) $(TARGET) *~
