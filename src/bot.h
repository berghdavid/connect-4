#ifndef BOT_H
#define BOT_H

#include "state.h"

int eval_field(Game* g, int** field);

State* init_root(Game* g);

int value(int p_opp, int p_me);

int eval_rows(Game* g, int** field, int row, int col);

int eval_cols(Game* g, int** field, int row, int col);

int eval_diags(Game* g, int** field, int row, int col);

int eval_square(Game* g, int** field, int row, int col);

void eval_state(State* s);

void reevaluate(State* parent);

void eval_children(List* l, State* s);

int get_best_move(Game* g, time_t seconds);

#endif
