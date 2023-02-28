#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "bot.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

int eval_field(Game* g, int** field)
{
	int	sum;
	int	i;
	int	j;

	sum = 0;
	for (i = 0; i < g->rows; i++) {
		for (j = 0; j < g->cols; j++) {
			sum += eval_square(g, field, i, j);
		}
	}

	return sum;
}

State* init_root(Game* g)
{
	State*	s;
	s = malloc(sizeof(State));
	s->depth = 0;
	s->g = g;
	s->parent = NULL;
	s->move_col = -1;
	s->move_row = -1;
	s->turn = g->turn;
	s->children = NULL;

	s->field = clone_field(g, g->field);
	s->eval = eval_field(g, s->field);
	return s;
}

int value(int p_1, int p_2)
{
	if ((p_1 > 0 && p_2 > 0) || (p_1 == 0 && p_2 == 0)) {
		return 0;
	}
	if (p_1 > 0) {
		if (p_1 == 1) {
			return 1;
		} else if (p_1 == 2) {
			return 4;
		} else if (p_1 == 3) {
			return 50;
		} else {
			return 999999;
		}
	}
	if (p_2 == 1) {
		return -1;
	} else if (p_2 == 2) {
		return -4;
	} else if (p_2 == 3) {
		return -20;
	}
	return -999999;
}

int eval_rows(Game* g, int** field, int row, int col)
{
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_1;
	int	p_2;

	sum = 0;
	floor = max(0, col - 3);
	roof = min(g->cols, col + 4) - 4;
	for (i = floor; i <= roof; i++) {
		p_1 = 0;
		p_2 = 0;
		for (j = 0; j < 4; j++) {
			if (field[row][i + j] == 1) {
				p_1++;
			} else if (field[row][i + j] == -1) {
				p_2++;
			}
		}
		sum += value(p_1, p_2);
	}

	return sum;
}

int eval_cols(Game* g, int** field, int row, int col)
{
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_1;
	int	p_2;

	sum = 0;
	floor = max(0, row - 3);
	roof = min(g->rows, row + 4) - 4;
	for (i = floor; i <= roof; i++) {
		p_1 = 0;
		p_2 = 0;
		for (j = 0; j < 4; j++) {
			if (field[i + j][col] == 1) {
				p_1++;
			} else if (field[i + j][col] == -1) {
				p_2++;
			}
		}
		sum += value(p_1, p_2);
	}
	return sum;
}

int eval_diags(Game* g, int** field, int row, int col)
{
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_1;
	int	p_2;

	sum = 0;
	/* Diagonally down */
	floor = -min(3, min(row, col));
	roof = max(-3, min(g->rows - row, g->cols - col) - 4);
	for (i = floor; i <= roof; i++) {
		p_1 = 0;
		p_2 = 0;
		for (j = 0; j < 4; j++) {
			if (field[row + i + j][col + i + j] == 1) {
				p_1++;
			} else if (field[row + i + j][col + i + j] == -1) {
				p_2++;
			}
		}
		sum += value(p_1, p_2);
	}

	/* Diagonally up */
	floor = -min(3, min(g->rows - row - 1, col));
	roof = max(-3, min(row, g->cols - col) - 4);
	for (i = floor; i <= roof; i++) {
		p_1 = 0;
		p_2 = 0;
		for (j = 0; j < 4; j++) {
			if (field[row - i - j][col + i + j] == 1) {
				p_1++;
			} else if (field[row - i - j][col + i + j] == -1) {
				p_2++;
			}
		}
		sum += value(p_1, p_2);
	}
	return sum;
}

int eval_square(Game* g, int** field, int row, int col)
{
	if (field[row][col] == 0) {
		return 0;
	}
	return eval_rows(g, field, row, col) + eval_cols(g, field, row, col) +
		eval_diags(g, field, row, col);
}

void eval_state(State* s)
{
	s->eval = eval_field(s->g, s->field);
	/*
	s->eval += eval_square(s->g, s->field, s->move_row, s->move_col);
	*/
}

void reevaluate(State* state)
{
	int	old_eval;

	if (state == NULL || state->children->size == 0) {
		return;
	}
	old_eval = state->eval;
	state->children = l_sort(state->children);
	state->eval = best_state(state)->eval;
	if (old_eval != state->eval) {
		reevaluate(state->parent);
	}
}

void eval_children(List* work, State* s)
{
	List*	sorted;
	State*	child;

	s->children = possible_moves(s);
	if (s->children->size == 0) {
		/* No more available moves */
		return;
	}
	sorted = init_list();
	child = l_pop_first(s->children);
	while (child != NULL) {
		eval_state(child);
		l_append_sorted(sorted, child);
		child = l_pop_first(s->children);
	}
	free(s->children);
	s->children = sorted;
	l_add_n(s->children, work);
	s->eval = best_state(s)->eval;
	reevaluate(s->parent);
}

int get_best_move(Game* g, time_t seconds)
{
	List*	work;
	State*	root;
	State*	s;
	time_t	stamp;
	int	batch;
	int	batch_size;
	int	best;
	int	iterations;

	stamp = time(NULL);
	batch_size = 10000;
	iterations = 0;
	work = init_list();
	root = init_root(g);
	l_append(work, root);

	while (work->first != NULL && time(NULL) < stamp + seconds) {
		for (batch = 0; batch < batch_size; batch++) {
			s = l_pop_first(work);
			if (s == NULL) {
				break;
			}
			eval_children(work, s);
		}
		iterations++;
	}
	
	best = best_state(root)->move_col;
	printf("Achieved depth %d\n", tree_depth(root));

	free_list(work);
	free_state(root);
	return best;
}
