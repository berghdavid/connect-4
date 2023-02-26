#ifndef BOT_H
#define BOT_H

#include "game.h"

typedef struct State State;
typedef struct List List;

struct State {
	int	best_move;
	List*	children;
	int	eval;
	int**	field;
	State*	next;
	State*	parent;
	int	prev_move;
};

struct List {
	State*	first;
	State*	last;
	int	size;
};

State* init_list();

void free_list(List* l);

void append(List* l, State* s);

int remove(List* l, State* s);

State* init_state(State* parent, int move);

void free_state(State* s);

int eval_state(State* s);

List* possible_moves(State* s);

int get_best_move(Game* g);

#endif
