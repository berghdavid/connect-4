#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "bot.h"


int test()
{
	Game*	g;
        State*  s;

	g = init_game(7, 6);

        make_move(g, 0);
        s = init_root(g);
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
