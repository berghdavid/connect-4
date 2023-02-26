#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

Game* init_game(int cols, int rows)
{
	Game*	g;
	int	i;

	assert(rows > 1 && cols > 1);

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

void cleanup_game(Game* g)
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
	printf("\nPlayer %d's turn.\n\n", g->turn);
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

int game_over(Game* g)
{
	if (field_filled(g)) {
		return 1;
	}

	printf("*TODO*: %d\n", g->turn);
	
	return 0;
}

int winner(Game* g)
{
	printf("*TODO*: %d\n", g->turn);
	return 0;
}
