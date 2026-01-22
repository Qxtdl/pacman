#include <stdbool.h>

#include "../game.h"
#include "ghost.h"

bool is_wall(int pos_y, int pos_x) {
    return game.map[pos_y][pos_x] == CELL_WALL;
}

bool is_ghost(int pos_y, int pos_x) {
    for (int i = 0; i < ghosts_amount; i++)
        if (ghosts[i].pos_y == pos_y && ghosts[i].pos_x == pos_x && !ghosts[i].is_eaten)
            return true;
    return false;
}

bool is_wall_or_ghost(int pos_y, int pos_x) {
    return is_wall(pos_y, pos_x) | is_ghost(pos_y, pos_x);
}