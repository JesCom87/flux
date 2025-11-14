CC = gcc
CFLAGS = -Iinclude -Wall

SRC = src/letterscribe.c src/crossscribe.c src/numberscribe.c src/scribe_main.c
TARGET = scribe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
