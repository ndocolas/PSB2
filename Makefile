CC=gcc
CFLAGS=-Wall #ativa todos os warnigs

all: exe

exe:
	$(CC) $(CFLAGS) -o exe main.c mymemory.c

clear:
	rm -f *.o exe

.PHONY: all clear