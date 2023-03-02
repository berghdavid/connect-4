#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "bot.h"

void test_1()
{
	Game*	g;
	State*  s;
	int     pass;

	pass = 0;
	g = init_game(7, 6);

	make_move(g, 1);
	make_move(g, 1);
	make_move(g, 2);
	make_move(g, 2);
	make_move(g, 3);
	make_move(g, 3);
	make_move(g, 0);
	s = init_root(g);
	pass = abs(s->eval) > 999;
	
	free_state(s);
	free_game(g);
	printf("test_1: ");
	if (pass) {
		printf("PASSED\n");
	} else {
		printf("FAILED\n");
	}
}

int main()
{
	test_1();
	return 0;
}
