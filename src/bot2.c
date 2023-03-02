#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "bot.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

int eval_field_b2(Bot* g, int** field)
{
	int	sum;
	int	i;
	int	j;

	sum = 0;
	for (i = 0; i < g->rows; i++) {
		for (j = 0; j < g->cols; j++) {
			sum += eval_square_b2(g, field, i, j);
		}
	}

	return sum;
}

Bot* init_bot_b2(Game* g)
{
	Bot*	b;
	State*	root;

	b = malloc(sizeof(Bot));
	root = malloc(sizeof(State));

	b->root = root;
	b->rows = g->rows;
	b->cols = g->cols;

	root->b = b;
	root->depth = 0;
	root->parent = NULL;
	root->move_col = -1;
	root->move_row = -1;
	root->turn = g->turn;
	root->children = NULL;
	root->field = clone_field(b, g->field);
	root->base_eval = eval_field_b2(b, root->field);
	root->eval = root->base_eval;
	return b;
}

int value_b2(int p_1, int p_2)
{
	if ((p_1 > 0 && p_2 > 0) || (p_1 == 0 && p_2 == 0)) {
		return 0;
	}
	if (p_1 > 0) {
		if (p_1 == 1) {
			return 1;
		} else if (p_1 == 2) {
			return 2;
		} else if (p_1 == 3) {
			return 20;
		} else {
			return 99999;
		}
	}
	if (p_2 == 1) {
		return -1;
	} else if (p_2 == 2) {
		return -2;
	} else if (p_2 == 3) {
		return -20;
	}
	return -99999;
}

int eval_rows_b2(Bot* g, int** field, int row, int col)
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
		sum += value_b2(p_1, p_2);
	}

	return sum;
}

int eval_cols_b2(Bot* g, int** field, int row, int col)
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
		sum += value_b2(p_1, p_2);
	}
	return sum;
}

int eval_diags_b2(Bot* g, int** field, int row, int col)
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
		sum += value_b2(p_1, p_2);
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
		sum += value_b2(p_1, p_2);
	}
	return sum;
}

int eval_square_b2(Bot* g, int** field, int row, int col)
{
	if (field[row][col] == 0) {
		return 0;
	}
	return eval_rows_b2(g, field, row, col) + eval_cols_b2(g, field, row, col) +
		eval_diags_b2(g, field, row, col);
}

void eval_state_b2(State* s)
{
	s->base_eval += eval_square_b2(s->b, s->field, s->move_row, s->move_col);
	s->eval = s->base_eval;
}

void reevaluate_b2(State* state)
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

void eval_children_b2(List* work, State* s)
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
		eval_state_b2(child);
		l_append_sorted(sorted, child);
		child = l_pop_first(s->children);
	}
	free(s->children);
	s->children = sorted;
	l_add_n(s->children, work);
	s->eval = best_state(s)->eval;
	reevaluate_b2(s->parent);
}

int get_best_move_b2(Game* g, time_t seconds)
{
	Bot*	b;
	List*	work;
	State*	s;
	time_t	start;
	int	best;
	int	batch;
	int	batch_size;
	int	iterations;

	b = init_bot_b2(g);

	start = time(NULL);
	batch_size = 10000;
	iterations = 0;
	work = init_list();
	l_append(work, b->root);

	while (work->first != NULL && time(NULL) < start + seconds) {
		for (batch = 0; batch < batch_size; batch++) {
			s = l_pop_first(work);
			if (s == NULL) {
				break;
			}
			eval_children_b2(work, s);
		}
		iterations++;
	}

	printf("Evaluated nodes: %d\n", ((iterations - 1) * batch_size) + batch);
	printf("Achieved depth %d\n", tree_depth(b->root));
	s = best_state(b->root);
	best = -1;
	if (s != NULL) {
		best = s->move_col;
	}

	free_list(work);
	free_state(b->root);
	free(b);
	return best;
}
