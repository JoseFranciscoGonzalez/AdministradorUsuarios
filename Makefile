CC=gcc
CFLAGS=-Wall -ansi -pedantic -O3 -c
LFLAGS=-Wall
DEBUG=-g

all: app

app: net.o user.o list.o array.o
	$(CC) $(LFLAGS) $(DEBUG) -o social_network net.o user.o list.o array.o

net.o: net.c net.h
	$(CC) $(CFLAGS) $(DEBUG) -o net.o net.c

user.o: user.c user.h
	$(CC) $(CFLAGS) $(DEBUG) -o user.o user.c

list.o: list.c list.h 
	$(CC) $(CFLAGS) $(DEBUG) -o list.o list.c

array.o: array.c array.h 
	$(CC) $(CFLAGS) $(DEBUG) -o array.o array.c

clean:
	rm *.o
