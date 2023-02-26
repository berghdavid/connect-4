#ifndef CONNECT_4_H
#define CONNECT_4_H

#include "game.h"

typedef enum {
	PLAYER,
	BOT
} InputType;

int get_input(Game* g, InputType type);

int play_game(InputType p1, InputType p2);

int main(int argc, char **argv);

#endif
