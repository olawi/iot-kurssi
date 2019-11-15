CC = gcc 
CFLAGS = -Wall

TARGET = child_signal

all: $(TARGET)

$(TARGET) :  $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

hello: hello.c
	$(CC) $(CFLAGS) -o hello.out hello.c

clean:
	rm $(TARGET) *.out

