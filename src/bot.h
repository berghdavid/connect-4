#ifndef BOT_H
#define BOT_H

#include "state.h"

State* init_root(Game* g);

int value(int p_opp, int p_me);

int eval_rows(State* s);

int eval_cols(State* s);

int eval_diags(State* s);

void eval_state(State* s);

void eval_children(List* l, State* s);

Node* pop();

int get_best_move(Game* g);

#endif
