CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: schedsim

schedsim: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o schedsim

test: schedsim
	bash tests/test_suite.sh

.PHONY: all clean test
