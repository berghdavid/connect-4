#ifndef GAME_H
#define GAME_H

typedef struct Game Game;

struct Game {
	int**	field;
	int	rows;
	int	cols;
	int	turn;
};

/**
 * @brief Initialize the playing board.
 * 
 * @param rows The number of rows in the playing field.
 * @param cols The number of columns in the playing field.
 */
Game* init_game(int rows, int cols);

/**
 * @brief Free the memory allocated for the playing board.
 * 
 * @param g The game being played.
 */
void free_game(Game* g);

/**
 * @brief Reset the field and turn of the game.
 * 
 * @param g The game to reset.
 */
void reset_game(Game* g);

/**
 * @brief Print the playing board.
 * 
 * @param g The game being played.
 * @param g The field to display.
 */
void print_game(Game* g, int** field);

/**
 * @brief Check whether the given move is possible according to the game rules.
 * 
 * @param g The game being played.
 * @param col The column the token will be put into.
 * @return int of 1 if true and 0 if false.
 */
int possible_move(Game* g, int col);

/**
 * @brief Make a move for the next player in the given column. Return 1 if the move is not possible.
 * 
 * @param g The game being played.
 * @param col The column to put the token into.
 * @return int of 1 if true and 0 if false.
 */
int make_move(Game* g, int col);

/**
 * @brief Check whether the entire playing board is filled and no more moves can be made.
 * 
 * @param g The game being played.
 * @return int of 1 if true and 0 if false.
 */
int field_filled(Game* g);

/**
 * @brief Check whether there is a 4-in-a-row victory column-wise.
 * 
 * @param g The game being played.
 * @return int of 1 if true and 0 if false.
 */
int col_win(Game* g);

/**
 * @brief Check whether there is a 4-in-a-row victory row-wise.
 * 
 * @param g The game being played.
 * @return int of 1 if true and 0 if false.
 */
int row_win(Game* g);

/**
 * @brief Check whether there is a 4-in-a-row victory diagonal-wise.
 * 
 * @param g The game being played.
 * @return int of 1 if true and 0 if false.
 */
int diag_win(Game* g);

/**
 * @brief Check whether the game is wither won or tied.
 * 
 * @param g The game being played.
 * @return int of 1 if true and 0 if false.
 */
int game_over(Game* g);

/**
 * @brief Compute the winner of the game. Should only be run if the game is definately finished.
 * 
 * @param g The game being played.
 * @return int of 1 or -1 if that player won and 0 if it was tied.
 */
int winner(Game* g);

#endif
