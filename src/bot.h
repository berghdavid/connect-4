#ifndef BOT_H
#define BOT_H

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
	State*	root;
	int**	field;
	int	rows;
	int	cols;
};

static List* init_list();

static void free_list_and_contents(List* l);

static void free_list(List* l);

static void l_append(List* l, State* s);

static void l_append_sorted(List* l, State* s);

static State* l_pop_first(List* l);

static void l_add_n(List* l_from, List* l_to, int limit);

static List* l_sort(List* l);

static State* best_state(State* s);

static int get_move_row(State* s);

static State* init_state(State* parent, int move);

static void free_state(State* s);

static List* possible_moves(State* s);

static int tree_depth(State* s);

static int** clone_field(Bot* b, int** field);

static int eval_field(Bot* b);

static Bot* init_bot(Game* g);

static void free_bot(Bot* b);

static int value(int p_opp, int p_me);

static int eval_rows(Bot* b, int row, int col);

static int eval_cols(Bot* b, int row, int col);

static int eval_diags(Bot* b, int row, int col);

static int eval_square(Bot* b, int row, int col);

static void eval_state(State* s);

static void reevaluate(State* parent);

static void update_field(State* s);

static void reset_field(State* s);

static void eval_children(List* l, State* s);

static long good_batch_nbr(List* work, clock_t start, clock_t stop, int first);

#endif
