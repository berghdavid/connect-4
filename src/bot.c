#include <stdlib.h>
#include "bot.h"

State* init_list()
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

void append(List* l, State* s)
{
	if (l->last == NULL) {
		l->first = s;
	} else {
		l->last->next = s;
	}
	l->last = s;
	l->size++;
}

int remove(List* l, State* s)
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

State* init_state(State* parent, int move)
{
	State*	s;
	s = malloc(sizeof(State));

	s->best_move = NULL;
	s->children = init_list();
	s->eval = s->parent->eval;
	s->field = NULL;	/* TODO */
	s->parent = parent;
	s->prev_move = move;

	return s;
}

void free_state(State* s)
{
	if (s->children != NULL) {
		free_list(s->children);
	}
	free(s);
}

int eval_state(State* s)
{
	return 0;
}

List* possible_moves(Game* g, State* s)
{
	List*	l;
	int	i;

	l = init_list();
	for (i = 0; i < g->cols; i++) {
		/* TODO */
	}
}

int get_best_move(Game* g)
{
	int     out;

	out = rand() % g->cols;

	return out;
}
