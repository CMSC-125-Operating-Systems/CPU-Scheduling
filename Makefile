# all: Compile the shell
# clean: Remove binaries and object files

CC=gcc
CFLAGS= -Wall


_OBJ = src/main.c
OBJ = $(_OBJ)

%.o: %.c 
	@$(CC) -c -o $@ $< $(CFLAGS)

all: schedsim

schedsim: $(OBJ)
	@$(CC) -o $@ $^

.PHONY: clean

clean: 
	@rm -f src/*.o schedsim
	@rm -f core *~
# 	@rm -f test_out.txt

test:
