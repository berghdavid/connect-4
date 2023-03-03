#ifndef CONNECT_4_H
#define CONNECT_4_H

#include "game.h"

typedef enum {
	PLAYER,
	BOT
} InputType;

int get_input(Game* g, InputType type, double time);

int play_game(InputType p1, InputType p2, int rows, int cols, double time, int starter);

int main(int argc, char **argv);

#endif
