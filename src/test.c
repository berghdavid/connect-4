#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "bot.h"


int test()
{
	Game*	g;
        State*  s;

	g = init_game(7, 6);

        make_move(g, 3);
        make_move(g, 4);
        make_move(g, 4);
        make_move(g, 4);
        make_move(g, 2);
        make_move(g, 4);
        make_move(g, 1);
        make_move(g, 4);
        s = init_root(g);
	print_game(g, g->field);
        printf("Eval: %d\n", s->eval);
	
	free_state(s);
	free_game(g);
	return 0;
}

int main()
{
	test();
        return 0;
}
