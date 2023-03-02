#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "bot2.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

List* init_list_b2()
{
	List*	l;
	l = malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

void free_list_and_contents_b2(List* l)
{
	Node*	child;
	Node*	next;

	child = l->first;
	while (child != NULL) {
		next = child->next;
		free_state_b2(child->state);
		free(child);
		child = next;
	}
	free(l);
}

void free_list_b2(List* l)
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

void l_append_b2(List* l, State* s)
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

void l_append_sorted_b2(List* l, State* s)
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

State* l_get_b2(List* l, int index)
{
	Node*	n;
	int	i;

	if (index < 0 || l->size <= index) {
		return NULL;
	}
	n = l->first;
	for (i = 0; i < index; i++) {
		n = n->next;
	}
	return n->state;
}

State* l_pop_first_b2(List* l)
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

void l_print_b2(List* l)
{
	Node*	n;

	n = l->first;
	printf("List of size %d for turn %d\n", l->size, l->first->state->turn);
	while (n != NULL) {
		printf("\t%d: %d\n", n->state->move_col, n->state->eval);
		n = n->next;
	}
}

void l_add_n_b2(List* l_from, List* l_to)
{
	Node*	n;

	if (l_from->first == NULL) {
		return;
	}

	n = l_from->first;
	while (n != NULL && abs(n->state->eval) < 999) {
		l_append_b2(l_to, n->state);
		n = n->next;
	}
}

List* l_sort_b2(List* l)
{
	List*	sorted;
	State*	child;

	sorted = init_list_b2();
	child = l_pop_first_b2(l);
	while (child != NULL) {
		l_append_sorted_b2(sorted, child);
		child = l_pop_first_b2(l);
	}
	free(l);
	return sorted;
}

List* possible_moves_b2(State* s)
{
	List*	list;
	int	i;

	list = init_list_b2();
	for (i = 0; i < s->b->cols; i++) {
		if (s->field[0][i] == 0) {
			l_append_b2(list, init_state_b2(s, i));
		}
	}
	return list;
}

State* best_state_b2(State* s)
{
	if (s->children != NULL && s->children->size > 0) {
		return s->children->first->state;
	}
	return NULL;
}

void print_state_b2(State* s)
{
	Node*	child;
	State*	s_i;

	if (s->children != NULL) {
		child = s->children->first;
		printf("\tMoves:\n");
		while (child != NULL) {
			s_i = child->state;
			printf("\t\t[%d, %d]: Eval = %d\n", s_i->move_row, s_i->move_col, s_i->eval);
			child = child->next;
		}
	}
}

void free_state_b2(State* s)
{
	int	i;

	for (i = 0; i < s->b->rows; i++) {
		free(s->field[i]);
	}
	free(s->field);

	if (s->children != NULL) {
		free_list_and_contents_b2(s->children);
	}
	free(s);
}

int tree_depth_b2(State* root)
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

int** clone_field_b2(Bot* g, int** old_field)
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

int** new_field_b2(State* s)
{
	int**	new_f;
	new_f = clone_field_b2(s->b, s->parent->field);
	new_f[s->move_row][s->move_col] = -s->turn;
	return new_f;
}

int get_move_row_b2(State* s)
{
	int	row;
	assert(s->parent->field[0][s->move_col] == 0);

	row = 0;
	while (row < s->b->rows - 1) {
		if (s->parent->field[row + 1][s->move_col] != 0) {
			break;
		}
		row++;
	}
	return row;
}

State* init_state_b2(State* parent, int move)
{
	State*	s;
	s = malloc(sizeof(State));

	s->b = parent->b;
	s->parent = parent;
	s->children = NULL;
	s->move_col = move;
	s->turn = -s->parent->turn;
	s->base_eval = parent->base_eval;
	s->eval = parent->base_eval;
	s->depth = parent->depth + 1;
	s->move_row = get_move_row_b2(s);
	s->field = new_field_b2(s);
	return s;
}

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
	root->field = clone_field_b2(b, g->field);
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
	state->children = l_sort_b2(state->children);
	state->eval = best_state_b2(state)->eval;
	if (old_eval != state->eval) {
		reevaluate_b2(state->parent);
	}
}

void eval_children_b2(List* work, State* s)
{
	List*	sorted;
	State*	child;

	s->children = possible_moves_b2(s);
	if (s->children->size == 0) {
		/* No more available moves */
		return;
	}
	sorted = init_list_b2();
	child = l_pop_first_b2(s->children);
	while (child != NULL) {
		eval_state_b2(child);
		l_append_sorted_b2(sorted, child);
		child = l_pop_first_b2(s->children);
	}
	free(s->children);
	s->children = sorted;
	l_add_n_b2(s->children, work);
	s->eval = best_state_b2(s)->eval;
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
	work = init_list_b2();
	l_append_b2(work, b->root);

	while (work->first != NULL && time(NULL) < start + seconds) {
		for (batch = 0; batch < batch_size; batch++) {
			s = l_pop_first_b2(work);
			if (s == NULL) {
				break;
			}
			eval_children_b2(work, s);
		}
		iterations++;
	}

	printf("Evaluated nodes: %d\n", ((iterations - 1) * batch_size) + batch);
	printf("Achieved depth %d\n", tree_depth_b2(b->root));
	s = best_state_b2(b->root);
	best = -1;
	if (s != NULL) {
		best = s->move_col;
	}

	free_list_b2(work);
	free_state_b2(b->root);
	free(b);
	return best;
}
