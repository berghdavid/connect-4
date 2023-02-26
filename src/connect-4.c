#include <stdio.h>
#include <stdlib.h>
#include "connect-4.h"
#include "game.h"
#include "bot.h"

int get_input(Game* g, InputType type)
{
	int	input;

	printf("Player %d's turn: ", g->turn);

	input = -1;
	if (type == PLAYER) {
		scanf("%d", &input);
	} else if (type == BOT) {
		input = get_best_move(g);
		printf("%d\n", input);
	}

	return input;
}

int play_game(InputType p1, InputType p2)
{
	Game*	g;
	int	input;
	int	player;

	g = init_game(7, 6);
	while (1) {
		print_game(g);
		if (g->turn == 1) {
			input = get_input(g, p1);
		} else {
			input = get_input(g, p2);
		}
		if (input == -1 || !possible_move(g, input)) {
			printf("Invalid move, try again.\n");
			continue;
		}
		make_move(g, input);
		if (game_over(g)) {
			print_game(g);
			player = winner(g);
			if (player == 0) {
				printf("Game tied, no winner!\n");
			} else {
				printf("Player %d won!\n", player);
			}
			break;
		}
	}
	cleanup_game(g);
	return 0;
}

int main(int argc, char **argv)
{
	char	input;

	if (argc == 2) {
		input = argv[1][0];
		printf("Input: %c\n", input);
		if (input == '0') {
			return play_game(PLAYER, PLAYER);
		} else if (input == '1') {
			return play_game(PLAYER, BOT);
		} else if (input == '2') {
			return play_game(BOT, BOT);
		}
	}

	printf("Must run with argument...\n");
	printf("\t0: Play against self.\n");
	printf("\t1: Play against bot.\n");
	printf("\t2: Bot against bot.\n");
	/* printf("3: Bot against AI course bot.\n"); */
	return 1;
}