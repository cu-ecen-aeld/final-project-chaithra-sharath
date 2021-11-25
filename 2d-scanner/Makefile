CC = gcc 
CFLAGS = -Wall
LDFLAGS = -lrt -lgpiod -pthread
OBJFILES = test_gpio_read.o test_gpio_write.o solarc.o server.o
TARGET = server_final
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
