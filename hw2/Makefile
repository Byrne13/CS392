CC=gcc
CFLAGS=-I.

DEPS = insertion.h utils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: main.o insertion.o utils.o
	$(CC) -o a.out main.o insertion.o utils.o $(CFLAGS)
