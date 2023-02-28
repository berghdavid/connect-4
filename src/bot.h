#ifndef BOT_H
#define BOT_H

#include "state.h"

State* init_root(Game* g);

int value(int p_opp, int p_me);

int eval_rows(State* s);

int eval_cols(State* s);

int eval_diags(State* s);

void eval_state(State* s);

void reevaluate(State* parent);

void l_add_n(List* l_from, List* l_to, int nbr);

void eval_children(List* l, State* s);

int get_best_move(Game* g, time_t seconds);

#endif
