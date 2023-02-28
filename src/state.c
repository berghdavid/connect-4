#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "state.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

List* init_list()
{
	List*	l;
	l = malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

void free_list_and_contents(List* l)
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

void free_list(List* l)
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

void l_append(List* l, State* s)
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

void l_append_sorted(List* l, State* s)
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

State* l_get(List* l, int index)
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

State* l_pop_first(List* l)
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

void l_print(List* l)
{
	Node*	n;

	n = l->first;
	printf("List of size %d for turn %d\n", l->size, l->first->state->turn);
	while (n != NULL) {
		printf("\t%d: %d\n", n->state->move_col, n->state->eval);
		n = n->next;
	}
}

void l_add_n(List* l_from, List* l_to, int min_nodes)
{
	int	i;
	Node*	n;
	int	limit;

	if (l_from->first == NULL) {
		return;
	}

	n = l_from->first;
	limit = max(min_nodes, n->state->g->cols - n->state->depth);
	i = 0;
	while (n != NULL && i < limit && abs(n->state->eval) < 99999) {
		l_append(l_to, n->state);
		n = n->next;
		i++;
	}
}

List* l_sort(List* l)
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

List* possible_moves(State* s)
{
	List*	list;
	int	i;

	list = init_list();
	for (i = 0; i < s->g->cols; i++) {
		if (s->field[0][i] == 0) {
			l_append(list, init_state(s, i));
		}
	}
	return list;
}

State* best_state(State* s)
{
	if (s->children != NULL && s->children->first != NULL) {
		return s->children->first->state;
	}
	return NULL;
}

void print_state(State* s)
{
	Node*	child;
	State*	s_i;

	printf(" --- STATE PRINT BEGIN ---\n");
	//printf("\tPrevious move: [row: %d, col: %d]\n", s->move_row, s->move_col);
	//printf("\tTurn: %d\n", s->turn);
	//printf("\tEvaluation: %d\n", s->eval);
	if (s->children != NULL) {
		child = s->children->first;
		printf("\tMoves:\n");
		while (child != NULL) {
			s_i = child->state;
			printf("\t\t[%d, %d]: Eval = %d\n", s_i->move_row, s_i->move_col, s_i->eval);
			child = child->next;
		}
	}
	printf(" --- STATE PRINT END ---\n");
}

void free_state(State* s)
{
	int	i;

	for (i = 0; i < s->g->rows; i++) {
		free(s->field[i]);
	}
	free(s->field);

	if (s->children != NULL) {
		free_list_and_contents(s->children);
	}
	free(s);
}

int tree_depth(State* root)
{
	int	depth;
	State*	s_i;

	depth = -1;
	s_i = root;
	while (s_i != NULL && s_i->children != NULL) {
		print_state(s_i);
		s_i = s_i->children->first->state;
		depth++;
	}
	return depth;
}

int** clone_field(Game* g, int** old_field)
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

int** new_field(State* s)
{
	int**	new_f;
	new_f = clone_field(s->g, s->parent->field);
	new_f[s->move_row][s->move_col] = -s->turn;
	return new_f;
}

int get_move_row(State* s)
{
	int	row;
	assert(s->parent->field[0][s->move_col] == 0);

	row = 0;
	while (row < s->g->rows - 1) {
		if (s->parent->field[row + 1][s->move_col] != 0) {
			break;
		}
		row++;
	}
	return row;
}

State* init_state(State* parent, int move)
{
	State*	s;
	s = malloc(sizeof(State));

	s->g = parent->g;
	s->parent = parent;
	s->children = NULL;
	s->move_col = move;
	s->turn = -s->parent->turn;

	s->eval = parent->eval;
	s->depth = parent->depth + 1;
	s->move_row = get_move_row(s);
	s->field = new_field(s);
	return s;
}
