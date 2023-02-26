#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

#define min(a, b) (((a) < (b)) ? (a) : (b));

Game* init_game(int cols, int rows)
{
	Game*	g;
	int	i;

	assert(rows > 3 && cols > 3);

	g = malloc(sizeof(Game));
	g->rows = rows;
	g->cols = cols;
	g->turn = 1;
	g->field = malloc(rows * sizeof(int*));
	for (i = 0; i < rows; i++) {
		g->field[i] = calloc(cols, sizeof(int));
	}

	return g;
}

void free_game(Game* g)
{
	int	i;
	for (i = 0; i < g->rows; i++) {
		free(g->field[i]);
	}
	free(g->field);
	free(g);
}

void print_game(Game* g)
{
	int	i;
	int	j;

	for (j = 0; j < (3 * g->cols) + 2; j++) {
		printf("-");
	}
	printf("\n");
	for (i = 0; i < g->rows; i++) {
		printf("|");
		for (j = 0; j < g->cols; j++) {
			printf("%2d ", g->field[i][j]);
		}
		printf("|\n");
	}
	for (j = 0; j < (3 * g->cols) + 2; j++) {
		printf("-");
	}
	printf("\n\n");
}

int possible_move(Game* g, int col)
{

	return 0 <= col && col < g->cols && g->field[0][col] == 0;
}

int make_move(Game* g, int col)
{
	int	row;

	if (g->field[0][col] != 0) {
		return 1;
	}

	row = 0;
	while (row < g->rows - 1) {
		if (g->field[row + 1][col] != 0) {
			break;
		}
		row++;
	}

	g->field[row][col] = g->turn;
	g->turn = -g->turn;
	return 0;
}

int field_filled(Game* g)
{
	int	col;
	int	game_over;

	game_over = 1;
	for (col = 0; col < g->cols; col++) {
		if (g->field[0][col] == 0) {
			game_over = 0;
			break;
		}
	}
	return game_over;
}

int col_win(Game* g)
{
	int	col;
	int	row_0;
	int	prev_p;
	int**	f;

	prev_p = -g->turn;
	f = g->field;
	for (col = 0; col < g->cols; col++) {
		for (row_0 = 0; row_0 < g->rows - 3; row_0++) {
			if (f[row_0][col] == prev_p && f[row_0 + 1][col] == prev_p &&
			f[row_0 + 2][col] == prev_p && f[row_0 + 3][col] == prev_p) {
				return 1;
			}
		}
	}
	return 0;
}

int row_win(Game* g)
{
	int	row;
	int	col_0;
	int	prev_p;
	int*	f;

	prev_p = -g->turn;
	for (row = 0; row < g->rows; row++) {
		for (col_0 = 0; col_0 < g->cols - 3; col_0++) {
			f = g->field[row];
			if (f[col_0] == prev_p && f[col_0 + 1] == prev_p &&
			f[col_0 + 2] == prev_p && f[col_0 + 3] == prev_p) {
				return 1;
			}
		}
	}
	return 0;
}

int diag_win(Game* g)
{
	int	row;
	int	col;
	int	prev_p;
	int**	f;

	prev_p = -g->turn;
	f = g->field;

	/* Check diagonally down */
	for (row = 0; row < g->rows - 3; row++) {
		for (col = 0; col < g->cols - 3; col++) {
			if (f[row][col] == prev_p && f[row + 1][col + 1] == prev_p && 
			f[row + 2][col + 2] == prev_p && f[row + 3][col + 3] == prev_p) {
				return 1;
			}
		}
	}
	/* Check diagonally up */
	for (row = 3; row < g->rows; row++) {
		for (col = 0; col < g->cols - 3; col++) {
			if (f[row][col] == prev_p && f[row - 1][col + 1] == prev_p && 
			f[row - 2][col + 2] == prev_p && f[row - 3][col + 3] == prev_p) {
				return 1;
			}
		}
	}
	return 0;
}

int game_over(Game* g)
{
	if (field_filled(g)) {
		return 1;
	}
	return col_win(g) || row_win(g) || diag_win(g);
}

int winner(Game* g)
{
	if (col_win(g) || row_win(g) || diag_win(g)) {
		return -g->turn;
	}
	return 0;
}
