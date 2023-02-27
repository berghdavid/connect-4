#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
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
	s->children = NULL;
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
	roof = min(s->g->cols, s->move_col + 4) - 4;
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
	roof = min(s->g->rows, s->move_row + 4) - 4;
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
	floor = -min(3, min(s->move_row, s->move_col));
	roof = max(-3, min(s->g->rows - s->move_row, s->g->cols - s->move_col) - 4);
	for (i = floor; i < roof; i++) {
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
	floor = -min(3, min(s->g->rows - s->move_row - 1, s->move_col));
	roof = max(-3, min(s->move_row, s->g->cols - s->move_col) - 4);
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

void reevaluate(State* parent)
{
	Node*	n;
	int	eval_parent;

	if (parent == NULL) {
		return;
	}
	n = parent->children->first;
	eval_parent = 0;
	while (n != NULL) {
		if (parent->turn * n->state->eval > parent->turn * parent->eval) {
			parent->eval = n->state->eval;
			parent->best_move = n->state;
			eval_parent = 1;
		}
		n = n->next;
	}
	if (eval_parent == 1) {
		reevaluate(parent->parent);
	}
}

void eval_children(List* work, State* s)
{
	Node*	child;

	s->children = possible_moves(s);
	child = s->children->first;
	while (child != NULL) {
		eval_state(child->state);
		l_append(work, child->state);
		child = child->next;
	}
	reevaluate(s->parent);
}

int get_best_move(Game* g, time_t seconds)
{
	List*	work;
	State*	root;
	State*	s;
	time_t	stamp;
	int	batch;
	int	done;
	int	best;

	stamp = time(NULL);
	work = init_list();
	root = init_root(g);
	l_append(work, root);

	done = 0;
	while (!done && time(NULL) < stamp + seconds) {
		for (batch = 0; batch < 10000; batch++) {
			s = l_pop_first(work);
			if (s == NULL) {
				done = 1;
				break;
			}
			eval_children(work, s);
		}
		printf("Size: %d\n", work->size);
	}

	print_state(root);
	best = root->best_move->move_col;
	free_list(work);
	free_state(root);
	return best;
}
