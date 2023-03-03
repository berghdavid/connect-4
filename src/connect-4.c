#include <stdio.h>
#include <stdlib.h>
#include "connect-4.h"
#include "game.h"
#include "bot.h"

Player* init_player()
{
	Player*	p;

	p = malloc(sizeof(Player));
	p->input = BOT;
	p->time = 2.0;
	p->limit = 0;
	p->offset = 0;

	return p;
}

int get_input(Game* g, Player* p)
{
	int	input;

	input = -1;
	printf("Player %d's turn...\n", g->turn);
	if (p->input == PLAYER) {
		printf("Select move: ");
		scanf("%d", &input);
	} else if (p->input == BOT) {
		input = bot_move(g, p, 0);
		printf("Selected move: %d\n", input);
	}

	return input;
}

int play_game(Player* p1, Player* p2, int rows, int cols, int starter)
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
			input = get_input(g, p1);
		} else {
			input = get_input(g, p2);
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
	Player*	p1;
	Player*	p2;
	char	input;
	int	rows;
	int	cols;
	int	starter;

	/* Default connect-4 values */
	rows = 6;
	cols = 7;
	starter = 1;
	p1 = init_player();
	p2 = init_player();

	if (argc != 2 && argc != 11) {
		printf("Run with 1 argument to specify game type.\n");
		printf("\t0: Bot vs bot.\n");
		printf("\t1: Play vs bot.\n");
		printf("\t2: Play vs self.\n");

		printf("\nRun with 10 arguments to customize the game entirely.\n");
		printf("\t1: Game type (0, 1 or 2)\n");
		printf("\t2: Number of rows, default is 6\n");
		printf("\t3: Number of columns, default is 7\n");
		printf("\t4: Who starts the game (1 or -1), default is 1\n");
		printf("\t5: Player 1 time limit, default is 4.0\n");
		printf("\t6: Player 1 decreasing child evaluation limit, default is 0\n");
		printf("\t7: Player 1 decreasing child evaluation offset, default is 0\n");
		printf("\t8: Player 2 time limit, default is 2.0\n");
		printf("\t9: Player 2 decreasing child evaluation limit, default is 0\n");
		printf("\t10: Player 2 decreasing child evaluation offset, default is 0\n");

		printf("\nExample: ./connect-4 0 6 7 1 1.0 0 0 4.0 0 0\n");
		return 1;
	}

	sscanf(argv[1], "%c", &input);
	if (argc == 11) {
		sscanf(argv[2], "%d", &rows);
		sscanf(argv[3], "%d", &cols);
		sscanf(argv[4], "%d", &starter);
		sscanf(argv[5], "%lf", &p1->time);
		sscanf(argv[6], "%d", &p1->limit);
		sscanf(argv[7], "%d", &p1->offset);
		sscanf(argv[8], "%lf", &p2->time);
		sscanf(argv[9], "%d", &p2->limit);
		sscanf(argv[10], "%d", &p2->offset);
	}

	printf(" --- Game parameters ---\n");
	printf("Rows: %d\n", rows);
	printf("Columns: %d\n", cols);
	printf("Starting player: %d\n", starter);

	if (input == '0') {
		printf("Bot vs Bot\n");
		p1->input = BOT;
		p2->input = BOT;
		play_game(p1, p2, rows, cols, starter);
	} else if (input == '1') {
		p1->input = PLAYER;
		p2->input = BOT;
		printf("Player vs Bot\n");
		play_game(p1, p2, rows, cols, starter);
	} else if (input == '2') {
		p1->input = PLAYER;
		p2->input = PLAYER;
		printf("Player vs Player\n");
		play_game(p1, p2, rows, cols, starter);
	}

	free(p1);
	free(p2);
	return 0;
}
