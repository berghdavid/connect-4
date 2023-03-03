#ifndef CONNECT_4_H
#define CONNECT_4_H

#include "game.h"

typedef struct Player Player;
typedef enum {
	PLAYER,
	BOT
} InputType;

struct Player {
	InputType	input;
	double		time;
	int		limit;
	int		offset;
};

Player* init_player();

int get_input(Game* g, Player* p);

int play_game(Player* p1, Player* p2, int rows, int cols, int starter);

int main(int argc, char **argv);

#endif
