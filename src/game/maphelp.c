#include <stdbool.h>

#include "../game.h"

extern struct game game;

bool is_wall(int pos_y, int pos_x) {
    return game.map[pos_y][pos_x] == CELL_WALL;
}

bool is_ghost(int pos_y, int pos_x) {
    for (int i = 0; i < game.ghosts_amount; i++)
        if (game.ghosts[i].pos_y == pos_y && game.ghosts[i].pos_x == pos_x && !game.ghosts[i].is_eaten)
            return true;
    return false;
}

bool is_wall_or_ghost(int pos_y, int pos_x) {
    return is_wall(pos_y, pos_x) | is_ghost(pos_y, pos_x);
}