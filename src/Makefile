CC  = gcc
CFLAGS = -Wall -Wpedantic -Wextra

debug: CFLAGS += -ggdb3

.PHONY: all

PROGS = connect-4

all: $(PROGS)

run: connect-4
	./connect-4 0

run-bb: connect-4
	./connect-4 0

run-pb: connect-4
	./connect-4 1

run-pp: connect-4
	./connect-4 2

debug: connect-4
	valgrind --leak-check=full --track-origins=yes ./connect-4 2

connect-4: connect-4.o game.o bot.o
test: test.o game.o bot.o

clean:
	rm -f *.o $(PROGS)

SRC = $(wildcard *.c)
