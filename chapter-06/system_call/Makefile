CC = gcc

all: system_call 

system_call: system_call.o
	$(CC) system_call.o -o system_call

system_call.o: system_call.c
	$(CC) -c system_call.c -o system_call.o

clean:
	rm -rf system_call.o system_call