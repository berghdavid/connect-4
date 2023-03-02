#ifndef BOT2_H
#define BOT2_H

#include <time.h>
#include "game.h"

typedef struct State_b2 State_b2;
typedef struct Node_b2 Node_b2;
typedef struct List_b2 List_b2;
typedef struct Bot_b2 Bot_b2;

struct State_b2 {
	Bot_b2*	b;
	List_b2*	children;
	State_b2*	parent;
	int**	field;
	int	depth;
	int	base_eval;
	int	eval;
	int	move_col;
	int	move_row;
	int	turn;
};

struct Node_b2 {
	State_b2*	state;
	Node_b2*	next;
};

struct List_b2 {
	Node_b2*	first;
	Node_b2*	last;
	int	size;
};

struct Bot_b2 {
	State_b2*		root;
	int		rows;
	int		cols;
};

List_b2* init_list_b2();

void free_list_and_contents_b2(List_b2* l);

void free_list_b2(List_b2* l);

void l_append_b2(List_b2* l, State_b2* s);

void l_append_sorted_b2(List_b2* l, State_b2* s);

State_b2* l_get_b2(List_b2* l, int index);

State_b2* l_pop_first_b2(List_b2* l);

void l_print_b2(List_b2* l);

void l_add_n_b2(List_b2* l_from, List_b2* l_to);

List_b2* l_sort_b2(List_b2* l);

List_b2* possible_moves_b2(State_b2* s);

State_b2* best_state_b2(State_b2* s);

void print_state_b2(State_b2* s);

void free_state_b2(State_b2* s);

int tree_depth_b2(State_b2* s);

int** clone_field_b2(Bot_b2* b, int** field);

int** new_field_b2(State_b2* s);

int get_move_row_b2(State_b2* s);

State_b2* init_state_b2(State_b2* parent, int move);

int eval_field_b2(Bot_b2* g, int** field);

Bot_b2* init_bot_b2(Game* g);

int value_b2(int p_opp, int p_me);

int eval_rows_b2(Bot_b2* g, int** field, int row, int col);

int eval_cols_b2(Bot_b2* g, int** field, int row, int col);

int eval_diags_b2(Bot_b2* g, int** field, int row, int col);

int eval_square_b2(Bot_b2* g, int** field, int row, int col);

void eval_state_b2(State_b2* s);

void reevaluate_b2(State_b2* parent);

void eval_children_b2(List_b2* l, State_b2* s);

int get_best_move_b2(Game* g, time_t seconds);

#endif
