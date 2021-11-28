CC = gcc 
CFLAGS = -Wall
LDFLAGS = -lrt -pthread
OBJFILES = nokia5110.o armbianio.o client.o
TARGET = client
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
