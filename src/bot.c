#include <assert.h>
#include <stdlib.h>
#include "bot.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

State* init_root(Game* g)
{
	State*	s;
	s = malloc(sizeof(State));
	s->best_move = NULL;
	s->eval = 0;
	s->g = g;
	s->parent = NULL;
	s->move_col = -1;
	s->move_row = -1;
	s->turn = g->turn;

	s->field = clone_field(g, g->field);
	s->children = possible_moves(s);
	return s;
}

int value(int p_opp, int p_me)
{
	if (p_opp == 0 && p_me == 0) {
		return 0;
	}
	if (p_opp > 0 && p_me == 1) {
		return value(0, p_opp);
	} 
	if (p_opp == 0) {
		if (p_me == 1) {
			return 1;
		} else if (p_me == 2) {
			return 4;
		} else if (p_me == 3) {
			return 12;
		} else if (p_me == 4) {
			return 999999;
		}
	}

	return 0;
}

int eval_rows(State* s)
{
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_opp;
	int	p_me;

	sum = 0;
	floor = max(0, s->move_col - 3);
	roof = min(s->g->cols, s->move_col + 4) - 3;
	for (i = floor; i <= roof; i++) {
		p_opp = 0;
		p_me = 0;
		for (j = 0; j < 4; j++) {
			if (s->field[s->move_row][i + j] == -s->turn) {
				p_me++;
			} else if (s->field[s->move_row][i + j] == s->turn) {
				p_opp++;
			}
		}
		sum += value(p_opp, p_me);
	}

	return sum;
}

int eval_cols(State* s)
{
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_opp;
	int	p_me;

	sum = 0;
	floor = max(0, s->move_row - 3);
	roof = min(s->g->rows, s->move_row + 4) - 3;
	for (i = floor; i <= roof; i++) {
		p_opp = 0;
		p_me = 0;
		for (j = 0; j < 4; j++) {
			if (s->field[i + j][s->move_col] == -s->turn) {
				p_me++;
			} else if (s->field[i + j][s->move_col] == s->turn) {
				p_opp++;
			}
		}
		sum += value(p_opp, p_me);
	}
	return sum;
}

int eval_diags(State* s)
{
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_opp;
	int	p_me;

	sum = 0;

	/* Diagonally down */
	floor = -min(3, max(s->move_col, s->move_row));
	roof = min(s->g->rows, max(s->move_col + 3, s->move_row + 3)) - 3;
	for (i = floor; i <= roof; i++) {
		p_opp = 0;
		p_me = 0;
		for (j = 0; j < 4; j++) {
			if (s->field[s->move_row + i + j][s->move_col + i + j] == -s->turn) {
				p_me++;
			} else if (s->field[s->move_row + i + j][s->move_col + i + j] == s->turn) {
				p_opp++;
			}
		}
		sum += value(p_opp, p_me);
	}

	/* Diagonally up */
	floor = -min(3, max(s->move_col, s->g->rows - s->move_row - 1));
	roof = min(s->g->rows, max(s->move_col + 3, s->g->rows - s->move_row + 2)) - 3;
	for (i = floor; i <= roof; i++) {
		p_opp = 0;
		p_me = 0;
		for (j = 0; j < 4; j++) {
			if (s->field[s->move_row - i - j][s->move_col + i + j] == -s->turn) {
				p_me++;
			} else if (s->field[s->move_row - i - j][s->move_col + i + j] == s->turn) {
				p_opp++;
			}
		}
		sum += value(p_opp, p_me);
	}
	return sum;
}

void eval_state(State* s)
{
	s->eval += s->parent->turn * (eval_rows(s) + eval_cols(s) + eval_diags(s));
}

void eval_children(List* work, State* s)
{
	Node*	child;

	child = s->children->first;
	while (s != NULL) {
		eval_state(child->state);
		l_append(work, child->state);
		child = child->next;
	}
}

int get_best_move(Game* g)
{
	List*	work;
	State*	root;
	int     out;

	root = init_root(g);
	print_state(root);

	work = init_list();
	l_append(work, root);
	free_list(work);

	out = rand() % g->cols;

	free_state(root);

	return out;
}
