CC = gcc
CFLAGS = -Iinclude -Wall

all: scribe

scribe: src/scribe_main.c src/letterscribe.c src/crossscribe.c src/numberscribe.c
	$(CC) $(CFLAGS) -o scribe src/scribe_main.c src/letterscribe.c src/crossscribe.c src/numberscribe.c

clean:
	rm -f scribe output.flux linked.flux
