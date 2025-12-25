#include "ghost.h"
#include "../util.h"
#include "../game.h"
#include "../timer.h"

extern struct game game;

static void add_ghost(ghost_t ghost) {
    game.ghosts = srealloc(game.ghosts, ++game.ghosts_amount * sizeof(ghost_t));
    game.ghosts[game.ghosts_amount - 1] = ghost;
}

void ghosts_setup(void) {
    for (int i = 0; i < game.map_height; i++) {
        for (int j = 0; j < game.map_width; j++) {
            if (game.map[i][j] == CELL_GHOST_SPAWNER)
                add_ghost((ghost_t){TEXTURE_GHOST, j, i, STATE_SCATTER});
        }
    }
}

static bool tile_free(int pos_y, int pos_x) {
    if (game.map[pos_y][pos_x] != CELL_WALL) {
        for (int i = 0; i < game.ghosts_amount; i++)
            if (game.ghosts[i].pos_y == pos_y && game.ghosts[i].pos_x == pos_x)
                return false;
        return true;
    }
    return false;
}

static void ghost_moveto(ghost_t *ghost, int pos_y, int pos_x) {
    bool calculated_a_move = false;
    int pos_x_diff = pos_x - ghost->pos_x;
    int pos_y_diff = pos_y - ghost->pos_y;

    if (pos_x_diff > 0 && tile_free(ghost->pos_y, ghost->pos_x + 1)) {
        ghost->pos_x++;
        calculated_a_move = true;
    }
    else if (pos_x_diff < 0 && tile_free(ghost->pos_y, ghost->pos_x - 1)) {
        ghost->pos_x--;
        calculated_a_move = true;
    }
    else if (pos_y_diff > 0 && tile_free(ghost->pos_y + 1, ghost->pos_x)) {
        ghost->pos_y++;
        calculated_a_move = true;
    }
    else if (pos_y_diff < 0 && tile_free(ghost->pos_y - 1, ghost->pos_x)) {
        ghost->pos_y--;
        calculated_a_move = true;
    }
    if (!calculated_a_move) {
        ghost->state = STATE_SCATTER;       
    } else ghost->state = STATE_CHASE;
}

void ghost_tick(void) {
    if (timer_triggered(SLOT_GHOST_MOVE, SLOT_GHOST_MOVE_VALUE)) {
        for (int i = 0; i < game.ghosts_amount; i++) {
            if (game.ghosts[i].state == STATE_CHASE)
                ghost_moveto(&game.ghosts[i], game.pacman.pos_y, game.pacman.pos_x);
            else if (game.ghosts[i].state == STATE_SCATTER) {
                ghost_moveto(&game.ghosts[i], random_int(0, game.map_height), random_int(0, game.map_width));
            }
        }
    }
}