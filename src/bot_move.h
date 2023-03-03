#ifndef BOT_MOVE_H
#define BOT_MOVE_H

#include "game.h"

/**
 * @brief Compute the best move given a set of constraints
 * 
 * @param g The game being played, contains relevant information such as whose
 * turn it is and how the playing board looks like.
 * @param seconds The approximate number of seconds the bot is allowed to
 * think. Note that the bot might exceed this number by 1/4 approximately.
 * @param logging Whether to print results retrieved or not.
 * @return Which column the playing token should be placed into.
 */
int bot_move(Game* g, double seconds, int logging);

#endif
