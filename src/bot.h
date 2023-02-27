#ifndef BOT_H
#define BOT_H

#include "game.h"

typedef struct State State;
typedef struct List List;

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

int l_remove(List* l, State* s);

void free_state(State* s);

void eval_state(State* s);

List* possible_moves(State* s);

int** clone_field(Game* g, int** field);

int** new_field(State* s);

State* init_state(State* parent, int move);

State* init_root(Game* g);

int get_best_move(Game* g);

#endif
