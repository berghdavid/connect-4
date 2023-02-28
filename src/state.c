#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "state.h"

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

	n = malloc(sizeof(Node));
	n->state = s;
	n->next = NULL;

	if (l->first == NULL) {
		l->size = 1;
		l->first = n;
		l->last = n;
		return;
	}

	n_i = l->first;
	val = s->turn * s->eval;
	if (n_i->state->turn * n_i->state->eval < val) {
		l->first = n;
		n->next = n_i;
		l->size++;
		return;
	}

	while (n_i->next != NULL && n_i->next->state->turn * n_i->next->state->eval > val) {
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

void resort_element(List* l, State* s)
{
	Node*	prev;
	Node*	n;

	if (s == l->first->state) {
		l_pop_first(l);
		l_append_sorted(l, s);
		return;
	}
	n = l->first;
	prev = l->first;
	while (n != NULL && n->state != s) {
		prev = n;
		n = n->next;
	}
	if (n == NULL) {
		return;
	}
	prev->next = n->next;
	n->next = NULL;
	
	l_append_sorted(l, n->state);
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

void print_state(State* s)
{
	Node*	child;
	State*	s_i;

	printf(" --- STATE PRINT BEGIN ---\n");
	print_game(s->g, s->field);
	printf("Previous move: [row: %d, col: %d]\n", s->move_row, s->move_col);
	printf("Evaluation: %d\n", s->eval);
	printf("Turn: %d\n", s->turn);
	if (s->best_move != NULL) {
		printf("Best move: [%d, %d]\n", s->best_move->move_row, s->best_move->move_col);
	} else {
		printf("Best move: NULL\n");
	}
	child = s->children->first;
	printf("Moves:\n");
	while (child != NULL) {
		s_i = child->state;
		printf("\t[%d, %d]: Eval = %d\n", s_i->move_row, s_i->move_col, s_i->eval);
		child = child->next;
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

int state_depth(State* s)
{
	int	depth;
	State*	s_i;

	depth = 0;
	s_i = s;
	while (s_i != NULL) {
		s_i = s_i->parent;
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

	s->best_move = NULL;
	s->g = parent->g;
	s->parent = parent;
	s->move_col = move;
	s->turn = -s->parent->turn;

	s->children = NULL;
	s->move_row = get_move_row(s);
	s->field = new_field(s);
	s->eval = parent->eval;
	s->depth = parent->depth + 1;
	return s;
}
