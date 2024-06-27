CC=gcc
CFLAGS=-Wall #ativa todos os warnigs

all: clear exe clean

exe: 
	$(CC) -o exe main.c mymemory.c

clear: 
	rm -f *.o exe

clean:
	clear

run:
	./exe

.PHONY: all clear clean