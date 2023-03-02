#ifndef BOT2_H
#define BOT2_H

#include <time.h>
#include "game.h"

typedef struct State State;
typedef struct Node Node;
typedef struct List List;
typedef struct Bot Bot;

struct State {
	Bot*	b;
	List*	children;
	State*	parent;
	int**	field;
	int	depth;
	int	base_eval;
	int	eval;
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

struct Bot {
	State*		root;
	int		rows;
	int		cols;
};

List* init_list_b2();

void free_list_and_contents_b2(List* l);

void free_list_b2(List* l);

void l_append_b2(List* l, State* s);

void l_append_sorted_b2(List* l, State* s);

State* l_get_b2(List* l, int index);

State* l_pop_first_b2(List* l);

void l_print_b2(List* l);

void l_add_n_b2(List* l_from, List* l_to);

List* l_sort_b2(List* l);

List* possible_moves_b2(State* s);

State* best_state_b2(State* s);

void print_state_b2(State* s);

void free_state_b2(State* s);

int tree_depth_b2(State* s);

int** clone_field_b2(Bot* b, int** field);

int** new_field_b2(State* s);

int get_move_row_b2(State* s);

State* init_state_b2(State* parent, int move);

int eval_field_b2(Bot* g, int** field);

Bot* init_bot_b2(Game* g);

int value_b2(int p_opp, int p_me);

int eval_rows_b2(Bot* g, int** field, int row, int col);

int eval_cols_b2(Bot* g, int** field, int row, int col);

int eval_diags_b2(Bot* g, int** field, int row, int col);

int eval_square_b2(Bot* g, int** field, int row, int col);

void eval_state_b2(State* s);

void reevaluate_b2(State* parent);

void eval_children_b2(List* l, State* s);

int get_best_move_b2(Game* g, time_t seconds);

#endif
