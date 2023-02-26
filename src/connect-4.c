#include <stdio.h>
#include <stdlib.h>
#include "connect-4.h"
#include "game.h"

int get_input(Game* g, InputType type)
{
	int	input;

	printf("Player %d's turn: ", g->turn);

	input = -1;
	if (type == PLAYER) {
		scanf("%d", &input);
	} else if (type == BOT) {
		/* Todo */
		input = 0;
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
		print_game(g);
		if (game_over(g)) {
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
	printf("0: Play against self.\n");
	/*
	* printf("1: Play against bot.\n");
	* printf("2: Bot against bot.\n");
	* printf("3: Bot against AI course bot.\n");
	*/
	return 1;
}