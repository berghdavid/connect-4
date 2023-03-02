#ifndef BOT_H
#define BOT_H

#include "game.h"
#include "state.h"

int eval_field(Bot* g, int** field);

Bot* init_bot(Game* g);

int value(int p_opp, int p_me);

int eval_rows(Bot* g, int** field, int row, int col);

int eval_cols(Bot* g, int** field, int row, int col);

int eval_diags(Bot* g, int** field, int row, int col);

int eval_square(Bot* g, int** field, int row, int col);

void eval_state(State* s);

void reevaluate(State* parent);

void eval_children(List* l, State* s);

int get_best_move(Game* g, time_t seconds);

#endif
