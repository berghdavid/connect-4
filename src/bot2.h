#ifndef BOT_H
#define BOT_H

#include <time.h>
#include "game.h"
#include "state.h"

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
