CC  = gcc
CFLAGS = -Wall -Wpedantic -Wextra
PROGS = connect-4

.PHONY: setup
debug: CFLAGS += -ggdb3

.PHONY: all
all: $(PROGS)

params: connect-4 # Display customizable parameters
	./connect-4

run-bb: connect-4 # Play the bot against itself
	./connect-4 0

run-pb: connect-4 # Play against the bot
	./connect-4 1

run-pp: connect-4 # Play against yourself or a friend
	./connect-4 2

debug: connect-4 # Run a valgrind leak check
	valgrind --leak-check=full --track-origins=yes ./connect-4 0

connect-4: connect-4.o game.o bot.o
test: test.o game.o bot.o

help: # Show help for each of the Makefile recipes.
	@grep -E '^[a-zA-Z0-9 -]+:.*#'  Makefile | sort | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:$$(echo $$l | cut -f 2- -d'#')\n"; done

clean: # Delete executables
	rm -f *.o $(PROGS)

SRC = $(wildcard *.c)
