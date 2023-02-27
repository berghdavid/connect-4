#ifndef BOT_H
#define BOT_H

#include "state.h"

State* init_root(Game* g);

int get_best_move(Game* g);

#endif
