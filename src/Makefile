# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CC  = gcc
CFLAGS = -Wall -Wpedantic -Wextra

debug: CFLAGS += -ggdb3

.PHONY: all

PROGS = connect-4

all: $(PROGS)

run: connect-4
	./connect-4

debug: connect-4
	valgrind --leak-check=full --track-origins=yes ./connect-4

connect-4: connect-4.o game.o

clean:
	rm -f *.o $(PROGS)

SRC = $(wildcard *.c)
