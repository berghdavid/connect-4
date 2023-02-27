#include <assert.h>
#include <stdlib.h>
#include "bot.h"

List* init_list()
{
	List*	l;
	l = malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

void free_list(List* l)
{
	State*	child;
	State*	next;

	child = l->first;
	while (child != NULL) {
		next = child->next;
		free_state(child);
		child = next;
	}
	free(l);
}

void l_append(List* l, State* s)
{
	if (l->last == NULL) {
		l->first = s;
	} else {
		l->last->next = s;
	}
	l->last = s;
	l->size++;
}

int l_remove(List* l, State* s)
{
	State*	s_i;
	State*	prev;

	if (l->first == s) {
		l->first = l->first->next;
		free_state(s);
		l->size--;
		return 1;
	}

	prev = l->first;
	s_i = l->first->next;
	while (s_i != NULL) {
		if (s_i == s) {
			prev->next = s_i->next;
			free_state(s);
			l->size--;
			return 1;
		}
		s_i = s_i->next;
	}
	return 0;
}

void free_state(State* s)
{
	int	i;

	for (i = 0; i < s->g->rows; i++) {
		free(s->field[i]);
	}
	free(s->field);

	if (s->children != NULL) {
		free_list(s->children);
	}
	free(s);
}

void eval_state(State* s)
{
	s->eval = 0;
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

int** clone_field(Game* g, int** old_field)
{
	int**	new_f;
	int	i;
	int	j;

	new_f = malloc(g->rows * sizeof(int*));
	for (i = 0; i < g->cols; i++) {
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
	int	i;

	new_f = clone_field(s->g, s->parent->field);
	assert(new_f[0][s->prev_move] == 0);

	i = 0;
	while (i < s->g->rows - 1) {
		if (new_f[i + 1][s->prev_move] != 0) {
			break;
		}
		i++;
	}

	new_f[i][s->prev_move] = -s->turn;
	return new_f;
}

State* init_state(State* parent, int move)
{
	State*	s;
	s = malloc(sizeof(State));

	s->best_move = NULL;
	s->g = s->parent->g;
	s->parent = parent;
	s->prev_move = move;
	s->turn = -s->parent->prev_move;

	s->children = init_list();
	s->field = new_field(s);
	s->eval = 0;
	return s;
}

State* init_root(Game* g)
{
	State*	s;
	s = malloc(sizeof(State));

	s->best_move = NULL;
	s->g = s->parent->g;
	s->parent = NULL;
	s->prev_move = -1;

	s->field = clone_field(g, g->field);
	s->children = possible_moves(s);
	s->eval = 0;
	return s;

}

int get_best_move(Game* g)
{
	int     out;

	out = rand() % g->cols;

	return out;
}
