CC=gcc
CFLAGS=-Wall #ativa todos os warnigs

all: clear exe

exe:
	$(CC) -o exe main.c mymemory.c

clear:
	rm -f *.o exe

.PHONY: all clear