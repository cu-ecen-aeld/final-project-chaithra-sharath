CC = gcc
CFLAGS = -Wall
LDFLAGS = -lrt -pthread
OBJFILES = nokia5110.o armbianio.o main.o
TARGET = test
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
