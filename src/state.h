#ifndef STATE_H
#define STATE_H

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

List* init_list();

void free_list_and_contents(List* l);

void free_list(List* l);

void l_append(List* l, State* s);

void l_append_sorted(List* l, State* s);

State* l_get(List* l, int index);

State* l_pop_first(List* l);

void l_print(List* l);

void l_add_n(List* l_from, List* l_to);

List* l_sort(List* l);

List* possible_moves(State* s);

State* best_state(State* s);

void print_state(State* s);

void free_state(State* s);

int tree_depth(State* s);

int** clone_field(Bot* b, int** field);

int** new_field(State* s);

int get_move_row(State* s);

State* init_state(State* parent, int move);

#endif
