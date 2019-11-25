CC = gcc 
CFLAGS = -Wall -lpthread

TARGET = threaded_server

all: $(TARGET)

$(TARGET) :  $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) 

clean:
	rm $(TARGET)

