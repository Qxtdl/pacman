#include <stdlib.h>

#include "../global.h"
#include "ghost.h"
#include "../util.h"
#include "../game.h"
#include "../timer.h"
#include "maphelp.h"

extern struct game game;

static void add_ghost(ghost_t ghost) {
    game.ghosts = srealloc(game.ghosts, ++game.ghosts_amount * sizeof(ghost_t));
    game.ghosts[game.ghosts_amount - 1] = ghost;
    game.ghosts[game.ghosts_amount - 1].spawn_pos_y = ghost.pos_y;
    game.ghosts[game.ghosts_amount - 1].spawn_pos_x = ghost.pos_x;
}

void ghosts_setup(void) {
    for (int i = 0; i < game.map_height; i++)
        for (int j = 0; j < game.map_width; j++)
            if (game.map[i][j] == CELL_GHOST_SPAWNER)
                add_ghost((ghost_t){TEXTURE_GHOST, j, i, 0, 0, 0, 0, false, STATE_CHASE, STATE_CHASE, true, {{0}}, false});
}

void reset_ghosts(void) {
    free(game.ghosts);
    game.ghosts = NULL;
    game.ghosts_amount = 0;
}

static inline void ghost_set_target(ghost_t *ghost,  enum state state,  int pos_y, int pos_x) {
    ghost->state_vector[state][0] = pos_y;
    ghost->state_vector[state][1] = pos_x;
}

static inline int lookup_ghost_target(ghost_t *ghost, enum state state, bool axis)  {
    return ghost->state_vector[state][axis];
}

char *print_state(enum state state) {
    switch (state) {
        case STATE_NULL: return "STATE_NULL\n";
        case STATE_CHASE: return "STATE_CHASE\n";
        case STATE_SCATTER: return"STATE_CHASE\n";
        case STATE_FRIGHTENED: return "STATE_CHASE\n";
        case STATE_AMOUNT: return "STATE_AMOUNT\n";
        default: return "ERROR\n";
    }
}

void set_ghost_state(ghost_t *ghost, enum state state) {
    ghost->new_state = true;
    ghost->old_state = ghost->state;
    ghost->state = state;
}

static void ghost_moveto(ghost_t *ghost, int pos_y, int pos_x) {
    if (ghost->new_state)
        ghost_set_target(ghost, ghost->state, pos_y, pos_x);

    int pos_y_diff = (ghost->new_state ? lookup_ghost_target(ghost, ghost->state, 0) : pos_y) - ghost->pos_y,
        pos_x_diff = (ghost->new_state ? lookup_ghost_target(ghost, ghost->state, 1) : pos_x) - ghost->pos_x;

    if (pos_x_diff > 0 && !is_wall_or_ghost(ghost->pos_y, ghost->pos_x + 1))
        ghost->pos_x++;
    else if (pos_x_diff < 0 && !is_wall_or_ghost(ghost->pos_y, ghost->pos_x -    1))
        ghost->pos_x--;
    else if (pos_y_diff > 0 && !is_wall_or_ghost(ghost->pos_y + 1, ghost->pos_x))
        ghost->pos_y++;
    else if (pos_y_diff < 0 && !is_wall_or_ghost(ghost->pos_y  -  1, ghost->pos_x))
        ghost->pos_y--;
    else if (!ghost->is_eaten) {
        if (ghost->state != STATE_SCATTER && ghost->state != STATE_FRIGHTENED)
            set_ghost_state(ghost, STATE_SCATTER);
        else if (ghost->state == STATE_SCATTER)
            set_ghost_state(ghost, ghost->old_state);
    }

    if (ghost->pos_y == lookup_ghost_target(ghost, ghost->state, 0) && ghost->pos_x == lookup_ghost_target(ghost, ghost->state, 1))
        set_ghost_state(ghost, ghost->old_state);
}

void ghost_tick(void) {
    if (timer_triggered(SLOT_GHOST_MOVE, game.pacman.power_mode ? SLOT_GHOST_MOVE_VALUE_FRIGHTENED : SLOT_GHOST_MOVE_VALUE))
        for (int i = 0; i < game.ghosts_amount; i++) {
            if (game.ghosts[i].is_eaten) {
                ghost_moveto(&game.ghosts[i], game.ghosts[i].spawn_pos_y, game.ghosts[i].spawn_pos_x);
                continue;
            }
            switch (game.ghosts[i].state) {
                case STATE_CHASE: ghost_moveto(&game.ghosts[i], game.pacman.pos_y, game.pacman.pos_x); break;
                case STATE_FRIGHTENED:
                    if (game.pacman.pos_y == game.ghosts[i].pos_y && game.pacman.pos_x == game.ghosts[i].pos_x + 1)
                        ghost_moveto(&game.ghosts[i], game.ghosts[i].pos_y, game.ghosts[i].pos_x - 1);
                    else if (game.pacman.pos_y == game.ghosts[i].pos_y && game.pacman.pos_x == game.ghosts[i].pos_x - 1)
                        ghost_moveto(&game.ghosts[i], game.ghosts[i].pos_y, game.ghosts[i].pos_x + 1);
                    else if (game.pacman.pos_y == game.ghosts[i].pos_y + 1 && game.pacman.pos_x == game.ghosts[i].pos_x)
                        ghost_moveto(&game.ghosts[i], game.ghosts[i].pos_y - 1, game.ghosts[i].pos_x);
                    else if (game.pacman.pos_y == game.ghosts[i].pos_y - 1 && game.pacman.pos_x == game.ghosts[i].pos_x)
                        ghost_moveto(&game.ghosts[i], game.ghosts[i].pos_y + 1, game.ghosts[i].pos_x);
                    else ghost_moveto(&game.ghosts[i], random_int(0, game.map_height), random_int(0, game.map_width));
                    break;
                case STATE_SCATTER: ghost_moveto(&game.ghosts[i], random_int(0, game.map_height), random_int(0, game.map_width)); break;
                default: app_abort("ghost_tick()", "Unknown ghost state") break;
            }
            if (game.ghosts[i].new_state)
                game.ghosts[i].new_state = false;
        }

    for (int i = 0; game.pacman.power_mode && i < game.ghosts_amount; i++)
        if (game.ghosts[i].pos_y == game.pacman.pos_y && game.ghosts[i].pos_x == game.pacman.pos_x)
            game.ghosts[i].is_eaten = true;
}