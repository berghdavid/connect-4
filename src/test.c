#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "bot.h"
#include "test.h"

int get_eval(Game* g, int moves[], int len)
{
	State*	s;
	int	i;
	int	eval;

	reset_game(g);
	for (i = 0; i < len; i++) {
		make_move(g, moves[i]);
	}

	s = init_root(g);
	eval = s->eval;

	free_state(s);
	return eval;
}

void test_row(Game* g)
{
	int	moves[] = {0, 0, 1, 1, 2, 2, 3};
	int	len;
	int	pass;
	int	i;
	int	j;

	len = 7;
	pass = 1;
	for (i = 0; i < g->cols - 3; i++) {
		if (abs(get_eval(g, moves, len)) < 999) {
			pass = 0;
			break;
		}

		for (j = 0; j < len; j++) {
			moves[j]++;
		}
	}
	
	printf("test_row: ");
	if (pass) {
		printf("PASSED\n");
	} else {
		printf("FAILED\n");
	}
}

void test_col(Game* g)
{
	int	moves[] = {0, 1, 0, 1, 0, 1, 0};
	int	len;
	int	pass;
	int	i;
	int	j;

	len = 7;
	pass = 1;
	for (i = 0; i < g->cols - 1; i++) {
		if (abs(get_eval(g, moves, len)) < 999) {
			pass = 0;
			break;
		}

		for (j = 0; j < len; j++) {
			moves[j]++;
		}
	}
	
	printf("test_col: ");
	if (pass) {
		printf("PASSED\n");
	} else {
		printf("FAILED\n");
	}
}

void test_diag_up(Game* g)
{
	int	moves[] = {0, 1, 1, 2, 2, 3, 2, 3, 3, 0, 3};
	int	len;
	int	pass;
	int	i;
	int	j;

	len = 11;
	pass = 1;
	for (i = 0; i < g->cols - 3; i++) {
		if (abs(get_eval(g, moves, len)) < 999) {
			pass = 0;
			break;
		}

		for (j = 0; j < len; j++) {
			moves[j]++;
		}
	}
	
	printf("test_diag_up: ");
	if (pass) {
		printf("PASSED\n");
	} else {
		printf("FAILED\n");
	}
}

void test_diag_down(Game* g)
{
	int	moves[] = {3, 2, 2, 1, 1, 0, 1, 0, 0, 3, 0};
	int	len;
	int	pass;
	int	i;
	int	j;

	len = 11;
	pass = 1;
	for (i = 0; i < 4; i++) {
		if (abs(get_eval(g, moves, len)) < 999) {
			pass = 0;
			break;
		}

		for (j = 0; j < len; j++) {
			moves[j]++;
		}
	}
	
	printf("test_diag_up: ");
	if (pass) {
		printf("PASSED\n");
	} else {
		printf("FAILED\n");
	}
}

int main()
{
	Game*	g;

	g = init_game(6, 7);
	test_row(g);
	test_col(g);
	test_diag_up(g);
	test_diag_down(g);

	free_game(g);
	return 0;
}
