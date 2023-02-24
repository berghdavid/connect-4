#include <stdio.h>
#include <stdlib.h>
#include "connect-4.h"
#include "game.h"

int main()
{
        Game*   g;

        g = init_game(7, 6);
        print_game(g);
        cleanup_game(g);

        return 0;
}