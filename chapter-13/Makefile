CC = gcc

memory-user: memory-user.o
	$(CC) memory-user.o -o memory-user

memory-user.o: memory-user.c
	$(CC) -c memory-user.c -o memory-user.o

clean:
	rm -rf memory-user.o memory-user