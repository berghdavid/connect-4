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

int make_move(Game* g, int col)
{
	int	i;

	if (g->field[0][col] != 0) {
		return 1;
	}

	for (i = 1; i < g->rows; i++) {
		if (g->field[i][col] != 0) {
			g->field[i][col] = g->turn;
			g->turn = -g->turn;
			return 0;
		}
	}

	g->field[g->rows - 1][col] = g->turn;
	g->turn = -g->turn;
	return 0;
}

int game_won(Game* g)
{
	printf("%d", g->turn);
	return 0;
}
