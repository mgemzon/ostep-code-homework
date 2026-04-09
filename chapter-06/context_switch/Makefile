CC = gcc

all: context_switch 

context_switch: context_switch.o
	$(CC) context_switch.o -o context_switch

context_switch.o: context_switch.c
	$(CC) -c context_switch.c -o context_switch.o

clean:
	rm -rf context_switch.o context_switch