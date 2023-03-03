#ifndef BOT_H
#define BOT_H

#include <time.h>
#include "connect-4.h"
#include "game.h"

typedef struct Bot Bot;
typedef struct List List;
typedef struct Node Node;
typedef struct State State;

struct Bot {
	State*	root;
	int**	field;
	int	rows;
	int	cols;
	int	limit;
	int	offset;
};

struct List {
	Node*	first;
	Node*	last;
	int	size;
};

struct Node {
	State*	state;
	Node*	next;
};

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

List* init_list();

void free_list_and_contents(List* l);

void free_list(List* l);

void l_append(List* l, State* s);

void l_append_sorted(List* l, State* s);

State* l_pop_first(List* l);

void l_add_n(List* l_from, List* l_to, Bot* b);

List* l_sort(List* l);

State* best_state(State* s);

int get_move_row(State* s);

State* init_state(State* parent, int move);

void free_state(State* s);

List* possible_moves(State* s);

int tree_depth(State* s);

int** clone_field(Bot* b, int** field);

int eval_field(Bot* b);

/**
 * @brief Initialize a bot struct containing the information given from the 
 * game object.
 * 
 * @param g The game object, containing field information among other params.
 * @param g The player object, determining how the nodes will be evaluated.
 * @return A Bot* object.
 */
Bot* init_bot(Game* g, Player* p);

void free_bot(Bot* b);

int value(int p_opp, int p_me);

int eval_rows(Bot* b, int row, int col);

int eval_cols(Bot* b, int row, int col);

int eval_diags(Bot* b, int row, int col);

int eval_square(Bot* b, int row, int col);

void eval_state(State* s);

void reevaluate(State* parent);

void update_field(State* s);

void reset_field(State* s);

void eval_children(List* l, State* s);

long good_batch_nbr(List* work, clock_t start, clock_t stop, int first);

/**
 * @brief Compute the best move given a set of constraints
 * 
 * @param g The game being played, contains relevant information such as whose
 * turn it is and how the playing board looks like.
 * @param p The player object which contains parameters affecting the evaluation
 * process.
 * @param logging Whether to print results retrieved or not.
 * @return Which column the playing token should be placed into.
 */
int bot_move(Game* g, Player* p, int logging);

#endif
