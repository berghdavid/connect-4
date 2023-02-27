#include <assert.h>
#include <stdlib.h>
#include "bot.h"
#include "state.h"

State* init_root(Game* g)
{
	State*	s;
	s = malloc(sizeof(State));
	s->best_move = NULL;
	s->eval = 0;
	s->g = g;
	s->parent = NULL;
	s->prev_move = -1;
	s->turn = g->turn;

	s->field = clone_field(g, g->field);
	s->children = possible_moves(s);
	return s;
}

int get_best_move(Game* g)
{
	State*	root;
	int     out;

	root = init_root(g);
	print_state(root);

	out = rand() % g->cols;

	free_state(root);

	return out;
}
