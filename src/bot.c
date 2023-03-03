#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "game.h"
#include "bot.h"
#include "bot_move.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

static List* init_list()
{
	List*	l;
	l = malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

static void free_list_and_contents(List* l)
{
	Node*	child;
	Node*	next;

	child = l->first;
	while (child != NULL) {
		next = child->next;
		free_state(child->state);
		free(child);
		child = next;
	}
	free(l);
}

static void free_list(List* l)
{
	Node*	child;
	Node*	next;

	child = l->first;
	while (child != NULL) {
		next = child->next;
		free(child);
		child = next;
	}
	free(l);
}

static void l_append(List* l, State* s)
{
	Node*	n;

	n = malloc(sizeof(Node));
	n->state = s;
	n->next = NULL;
	if (l->last == NULL) {
		l->first = n;
	} else {
		l->last->next = n;
	}
	l->last = n;
	l->size++;
}

static void l_append_sorted(List* l, State* s)
{
	Node*	n_i;
	Node*	n;
	int	val;
	int	turn;

	n = malloc(sizeof(Node));
	n->state = s;
	n->next = NULL;
	turn = -s->turn;

	if (l->first == NULL) {
		l->size = 1;
		l->first = n;
		l->last = n;
		return;
	}

	n_i = l->first;
	val = turn * s->eval;
	if (turn * n_i->state->eval < val) {
		l->first = n;
		n->next = n_i;
		l->size++;
		return;
	}

	while (n_i->next != NULL && turn * n_i->next->state->eval > val) {
		n_i = n_i->next;
	}
	if (n_i->next == NULL) {
		l->last->next = n;
		l->last = n;
	} else {
		n->next = n_i->next;
		n_i->next = n;
	}
	l->size++;
}

static State* l_pop_first(List* l)
{
	Node*	n;
	State*	s;

	n = l->first;
	if (n == NULL) {
		return NULL;
	}
	s = n->state;
	if (l->first == l->last) {
		l->last = NULL;
	}
	l->first = l->first->next;
	free(n);
	l->size--;
	return s;
}

static void l_add_n(List* l_from, List* l_to, int limit)
{
	Node*	n;
	int	floor;
	int	i;

	if (l_from->first == NULL) {
		return;
	}

	n = l_from->first;
	if (n == NULL) {
		return;
	}

	floor = max(limit, 1 + n->state->b->cols - n->state->depth);
	for (i = 0; i < floor; i++) {
		if (n == NULL || abs(n->state->base_eval) > 999) {
			return;
		}
		l_append(l_to, n->state);
		n = n->next;
	}
}

static List* l_sort(List* l)
{
	List*	sorted;
	State*	child;

	sorted = init_list();
	child = l_pop_first(l);
	while (child != NULL) {
		l_append_sorted(sorted, child);
		child = l_pop_first(l);
	}
	free(l);
	return sorted;
}

static State* best_state(State* s)
{
	if (s->children != NULL && s->children->size > 0) {
		return s->children->first->state;
	}
	return NULL;
}

static int get_move_row(State* s)
{
	int**	field;
	int	row;

	field = s->b->field;
	assert(field[0][s->move_col] == 0);

	row = 0;
	while (row < s->b->rows - 1) {
		if (field[row + 1][s->move_col] != 0) {
			break;
		}
		row++;
	}
	return row;
}

static State* init_state(State* parent, int move)
{
	State*	s;
	s = malloc(sizeof(State));

	s->b = parent->b;
	s->parent = parent;
	s->children = NULL;
	s->move_col = move;
	s->turn = -s->parent->turn;
	s->base_eval = 0;
	s->eval = 0;
	s->depth = parent->depth + 1;
	s->move_row = get_move_row(s);
	return s;
}

static void free_state(State* s)
{
	if (s->children != NULL) {
		free_list_and_contents(s->children);
	}
	free(s);
}

static List* possible_moves(State* s)
{
	List*	list;
	int	i;

	list = init_list();
	for (i = 0; i < s->b->cols; i++) {
		if (s->b->field[0][i] == 0) {
			l_append(list, init_state(s, i));
		}
	}
	return list;
}

static int tree_depth(State* root)
{
	int	depth;
	State*	s_i;

	depth = 0;
	s_i = root;
	while (s_i != NULL && s_i->children != NULL) {
		s_i = s_i->children->first->state;
		depth++;
	}
	return depth;
}

static int** clone_field(Bot* g, int** old_field)
{
	int**	new_f;
	int	i;
	int	j;

	new_f = malloc(g->rows * sizeof(int*));
	for (i = 0; i < g->rows; i++) {
		new_f[i] = malloc(g->cols * sizeof(int));
		for (j = 0; j < g->cols; j++) {
			new_f[i][j] = old_field[i][j];
		}
	}
	return new_f;
}

static int eval_field(Bot* b)
{
	int	sum;
	int	i;
	int	j;

	sum = 0;
	for (i = 0; i < b->rows; i++) {
		for (j = 0; j < b->cols; j++) {
			sum += eval_square(b, i, j);
		}
	}

	return sum;
}

static Bot* init_bot(Game* g)
{
	Bot*	b;
	State*	root;

	b = malloc(sizeof(Bot));
	root = malloc(sizeof(State));

	b->root = root;
	b->rows = g->rows;
	b->cols = g->cols;
	b->field = clone_field(b, g->field);

	root->b = b;
	root->depth = 0;
	root->parent = NULL;
	root->move_col = -1;
	root->move_row = -1;
	root->turn = g->turn;
	root->children = NULL;
	root->base_eval = eval_field(b);
	root->eval = root->base_eval;
	return b;
}

static void free_bot(Bot* b)
{
	int	i;

	for (i = 0; i < b->rows; i++) {
		free(b->field[i]);
	}
	free(b->field);
	free_state(b->root);
	free(b);
}

static int value(int p_1, int p_2)
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

static int eval_rows(Bot* b, int row, int col)
{
	int**	field;
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_1;
	int	p_2;

	field = b->field;
	sum = 0;
	floor = max(0, col - 3);
	roof = min(b->cols, col + 4) - 4;
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

static int eval_cols(Bot* b, int row, int col)
{
	int**	field;
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_1;
	int	p_2;

	field = b->field;
	sum = 0;
	floor = max(0, row - 3);
	roof = min(b->rows, row + 4) - 4;
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

static int eval_diags(Bot* b, int row, int col)
{
	int**	field;
	int	sum;
	int	i;
	int	j;
	int	floor;
	int	roof;
	int	p_1;
	int	p_2;

	field = b->field;
	sum = 0;
	/* Diagonally down */
	floor = -min(3, min(row, col));
	roof = max(-3, min(b->rows - row, b->cols - col) - 4);
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
	floor = -min(3, min(b->rows - row - 1, col));
	roof = max(-3, min(row, b->cols - col) - 4);
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

static int eval_square(Bot* b, int row, int col)
{
	if (b->field[row][col] == 0) {
		return 0;
	}
	return eval_rows(b, row, col) + eval_cols(b, row, col) +
		eval_diags(b, row, col);
}

static void eval_state(State* s)
{
	s->b->field[s->move_row][s->move_col] = -s->turn;
	s->base_eval = eval_square(s->b, s->move_row, s->move_col);
	s->eval = s->parent->base_eval + s->base_eval;
	s->b->field[s->move_row][s->move_col] = 0;
}

static void reevaluate(State* state)
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

static void update_field(State* s)
{
	State*	s_i;
	int**	field;

	field = s->b->field;
	s_i = s;
	while (s_i->parent != NULL) {
		field[s_i->move_row][s_i->move_col] = -s_i->turn;
		s_i = s_i->parent;
	}
}

static void reset_field(State* s)
{
	State*	s_i;
	int**	field;

	field = s->b->field;
	s_i = s;
	while (s_i->parent != NULL) {
		field[s_i->move_row][s_i->move_col] = 0;
		s_i = s_i->parent;
	}
}

static void eval_children(List* work, State* s)
{
	List*	sorted;
	State*	child;

	update_field(s);
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
	reset_field(s);
	free(s->children);
	s->children = sorted;
	l_add_n(s->children, work, s->b->cols);
	s->eval = best_state(s)->eval;
	reevaluate(s->parent);
}

static long good_batch_nbr(List* work, clock_t start, clock_t stop, int first)
{
	State*	s;
	int	batch;
	clock_t	passed;
	clock_t	batch_ticks;

	for (batch = 0; batch < first; batch++) {
		s = l_pop_first(work);
		if (s == NULL) {
			break;
		}
		eval_children(work, s);
	}
	passed = clock() - start;
	batch_ticks = (stop - clock()) / 4;
	return first * batch_ticks / passed;
}

int bot_move(Game* g, double seconds, int logging)
{
	Bot*	b;
	List*	work;
	State*	s;
	clock_t	start;
	clock_t	stop;
	double	time_diff;
	long	batch_size;
	long	best;
	int	first_batch;
	int	batch;
	int	iterations;

	start = clock();
	b = init_bot(g);
	work = init_list();
	l_append(work, b->root);
	batch = 0;
	iterations = 0;

	first_batch = 1000;
	stop = start + ((clock_t) (seconds * CLOCKS_PER_SEC));
	batch_size = good_batch_nbr(work, start, stop, first_batch);
	while (work->first != NULL && clock() < stop) {
		iterations++;
		for (batch = 0; batch < batch_size; batch++) {
			s = l_pop_first(work);
			if (s == NULL) {
				iterations--;
				break;
			}
			eval_children(work, s);
		}
	}
	s = best_state(b->root);
	best = -1;
	if (s != NULL) {
		best = s->move_col;
	}

	if (logging) {
		time_diff = (double) (clock() - start) / CLOCKS_PER_SEC;
		printf("\tTime passed (ms): %.3f\n", time_diff);
		printf("\tBatch size: %ld\n", batch_size);
		printf("\tEvaluated nodes: %ld\n", (iterations * batch_size) +
			batch + first_batch);
		printf("\tDepth computed: %d\n", tree_depth(b->root));
		printf("\tCurrent evaluation: %d\n", b->root->eval);
	}

	free_list(work);
	free_bot(b);
	return best;
}
