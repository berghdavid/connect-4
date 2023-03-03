#include <stdio.h>
#include <stdlib.h>
#include "connect-4.h"
#include "game.h"
#include "bot.h"

int get_input(Game* g, InputType type, double time)
{
	int	input;

	input = -1;

	if (type == PLAYER) {
		printf("Player %d's turn: ", g->turn);
		scanf("%d", &input);
	} else if (type == BOT) {
		printf("Player %d's turn:\n", g->turn);
		input = bot_move(g, time, 0);
	}
	printf("Selected move: %d\n", input);

	return input;
}

int play_game(InputType p1, InputType p2, int rows, int cols, double time, int starter)
{
	Game*	g;
	int	input;
	int	player;
	int	fails;
	int	round;

	g = init_game(rows, cols, starter);
	fails = 0;
	round = 0;
	player = 0;
	while (1) {
		printf("\n --- ROUND %d ---\n", round);
		print_game(g, g->field);
		if (g->turn == 1) {
			input = get_input(g, p1, time);
		} else {
			input = get_input(g, p2, time);
		}
		if (!possible_move(g, input)) {
			fails++;
			printf("Invalid move %d/3. ", fails);
			if (fails < 3) {
				printf("Try again.\n");
			} else {
				printf("Player %d gave bad inputs\n", g->turn);
				printf("Player %d won!\n", -g->turn);
				break;
			}
			continue;
		}
		fails = 0;
		make_move(g, input);
		round++;
		if (game_over(g)) {
			print_game(g, g->field);
			player = winner(g);
			if (player == 0) {
				printf("Game tied, no winner!\n");
			} else {
				printf("Player %d won!\n", player);
			}
			break;
		}
	}
	free_game(g);
	return player;
}

int main(int argc, char **argv)
{
	char	input;
	int	rows;
	int	cols;
	int	starter;
	double	time;

	/* Default connect-4 values */
	rows = 6;
	cols = 7;
	time = 4.0;
	starter = 1;

	if (argc != 2 && argc != 6) {
		printf("Run with 1 argument to specify game type.\n");
		printf("\t0: Bot vs bot.\n");
		printf("\t1: Play vs bot.\n");
		printf("\t2: Play vs self.\n");
		printf("\nRun with 5 arguments to customize the game further.\n");
		printf("\t1: Game type (0, 1 or 2)\n");
		printf("\t2: Number of rows, default is 6\n");
		printf("\t3: Number of columns, default is 7\n");
		printf("\t4: Time limit for bots, default is 4.0\n");
		printf("\t5: Who starts the game (1 or -1), default is 1\n");
		return 1;
	}

	sscanf(argv[1], "%c", &input);
	if (argc == 6) {
		sscanf(argv[2], "%d", &rows);
		sscanf(argv[3], "%d", &cols);
		sscanf(argv[4], "%lf", &time);
		sscanf(argv[5], "%d", &starter);
	}

	printf(" --- Game parameters ---\n");
	printf("Rows: %d\n", rows);
	printf("Columns: %d\n", cols);
	printf("Bot thinking time: %lf\n", time);
	printf("Starting player: %d\n", starter);

	if (input == '0') {
		printf("Bot vs Bot\n");
		play_game(BOT, BOT, rows, cols, time, starter);
	} else if (input == '1') {
		printf("Player vs Bot\n");
		play_game(PLAYER, BOT, rows, cols, time, starter);
	} else if (input == '2') {
		printf("Player vs Player\n");
		play_game(PLAYER, PLAYER, rows, cols, time, starter);
	}
	return 0;
}
