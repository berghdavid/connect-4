#ifndef STATE_H
#define STATE_H

#include "game.h"

typedef struct State State;
typedef struct Node Node;
typedef struct List List;

struct State {
	State*	best_move;
	List*	children;
	int	eval;
	int**	field;
	Game*	g;
	State*	parent;
	int	move_col;
	int	move_row;
	int	turn;
};

struct Node {
	State*	state;
	Node*	next;
};

struct List {
	Node*	first;
	Node*	last;
	int	size;
};

List* init_list();

void free_list_and_contents(List* l);

void free_list(List* l);

void l_append(List* l, State* s);

State* l_get(List* l, int index);

State* l_pop_first(List* l);

List* possible_moves(State* s);

void print_state(State* s);

void free_state(State* s);

int state_depth(State* s);

int** clone_field(Game* g, int** field);

int** new_field(State* s);

int get_move_row(State* s);

State* init_state(State* parent, int move);

#endif
