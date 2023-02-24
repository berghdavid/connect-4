typedef struct Game Game;

struct Game {
	int**	field;
	int	rows;
	int	cols;
	int	turn;
};

Game* init_game(int cols, int rows);

void print_game(Game* g);

int make_move(Game* g, int col);

int game_won(Game* g);

void cleanup_game(Game* g);
