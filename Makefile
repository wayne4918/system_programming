CC = gcc
CFLAGS = -Wall -g

all: main

main: main.o file_operations.o ipc_operations.o
	$(CC) $(CFLAGS) -o main main.o file_operations.o ipc_operations.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

file_operations.o: file_operations.c file_operations.h
	$(CC) $(CFLAGS) -c file_operations.c

ipc_operations.o: ipc_operations.c ipc_operations.h
	$(CC) $(CFLAGS) -c ipc_operations.c

clean:
	rm -f main *.o
