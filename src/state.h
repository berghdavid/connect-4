#ifndef STATE_H
#define STATE_H

#include "game.h"

typedef struct List List;
typedef struct State State;

struct State {
	State*	best_move;
	List*	children;
	int	eval;
	int**	field;
	Game*	g;
	State*	next;
	State*	parent;
	int	prev_move;
	int	turn;
};

struct List {
	State*	first;
	State*	last;
	int	size;
};

List* init_list();

void free_list(List* l);

void l_append(List* l, State* s);

State* l_get(List* l, int index);

int l_remove(List* l, State* s);

List* possible_moves(State* s);

void print_state(State* s);

void free_state(State* s);

void eval_state(State* s);

int state_depth(State* s);

int** clone_field(Game* g, int** field);

int** new_field(State* s);

State* init_state(State* parent, int move);

#endif
