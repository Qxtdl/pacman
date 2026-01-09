#include <stdio.h>
#include <stdlib.h>

#include "../global.h"
#include "ghost.h"
#include "../util.h"
#include "../game.h"
#include "../timer.h"
#include "maphelp.h"
#include "../assets.h"

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
                add_ghost((ghost_t){TEXTURE_GHOST, i, j, i, j, STATE_CHASE, STATE_CHASE, {{0}}, true, false});
}

void reset_ghosts(void) {
    free(game.ghosts);
    game.ghosts = NULL;
    game.ghosts_amount = 0;
}

void ghost_set_state(ghost_t *ghost, enum state state) {
    ghost->old_state = ghost->state;
    ghost->state = state;
}

static inline void ghost_set_target(ghost_t *ghost,  enum state state,  int pos_y, int pos_x) {
    if (!ghost->reached_vector) return;
    ghost->reached_vector = false;
    ghost->state_vector[state][0] = pos_y;
    ghost->state_vector[state][1] = pos_x;
}

static inline void ghost_set_target_now(ghost_t *ghost, enum state state, int pos_y, int pos_x) {
    if (!ghost->reached_vector) return;
    ghost->state_vector[state][0] = pos_y;
    ghost->state_vector[state][1] = pos_x;
}

static inline int lookup_ghost_target(ghost_t *ghost, enum state state, int axis)  {
    return ghost->state_vector[state][axis];
}

static void ghost_moveto_target(ghost_t *ghost) {
    int pos_y_diff = lookup_ghost_target(ghost, ghost->state, 0) - ghost->pos_y,
        pos_x_diff = lookup_ghost_target(ghost, ghost->state, 1) - ghost->pos_x;

    if (pos_x_diff > 0 && (!is_wall_or_ghost(ghost->pos_y, ghost->pos_x + 1) || ghost->is_eaten))
        ghost->pos_x++;
    else if (pos_x_diff < 0 && (!is_wall_or_ghost(ghost->pos_y, ghost->pos_x -1) || ghost->is_eaten))
        ghost->pos_x--;
    else if (pos_y_diff > 0 && (!is_wall_or_ghost(ghost->pos_y + 1, ghost->pos_x) || ghost->is_eaten))
        ghost->pos_y++;
    else if (pos_y_diff < 0 && (!is_wall_or_ghost(ghost->pos_y  -  1, ghost->pos_x) || ghost->is_eaten))
        ghost->pos_y--;
    else {
        if (!ghost->is_eaten && ghost->state != STATE_SCATTER)
            ghost_set_state(ghost, STATE_SCATTER);
        else if (ghost->state == STATE_SCATTER)
            ghost_set_state(ghost, ghost->old_state);
    }

    if (ghost->pos_y == lookup_ghost_target(ghost, ghost->state, 0) && ghost->pos_x == lookup_ghost_target(ghost, ghost->state, 1)) {
        ghost->reached_vector = true;
        if (ghost->state != STATE_EATEN)
            ghost_set_state(ghost, ghost->old_state);
        else if (!game.pacman.power_mode && ghost->is_eaten) {
            ghost->is_eaten = false;
            ghost->old_state = STATE_CHASE; // now it's in state chase and if it cant find it's way it dies
            ghost_set_state(ghost, STATE_CHASE);
        }
    }
}

void ghost_tick(void) {
    for (int i = 0; i < game.ghosts_amount; i++) {
        switch (game.ghosts[i].state) {
            case STATE_NULL: break;
            case STATE_CHASE: {
                if (game.map[game.pacman.pos_y][game.pacman.pos_x] == CELL_X_TUNNEL)
                    ghost_set_target(&game.ghosts[i], game.ghosts[i].state,game.pacman.pos_y, game.pacman.pos_x);
                else 
                    ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state,game.pacman.pos_y, game.pacman.pos_x);
            }
            break;
            case STATE_FRIGHTENED:
                if (game.pacman.pos_y == game.ghosts[i].pos_y && game.pacman.pos_x == game.ghosts[i].pos_x + 1)
                    ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, game.ghosts[i].pos_y, game.ghosts[i].pos_x - 1);
                else if (game.pacman.pos_y == game.ghosts[i].pos_y && game.pacman.pos_x == game.ghosts[i].pos_x - 1)
                    ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, game.ghosts[i].pos_y, game.ghosts[i].pos_x + 1);
                else if (game.pacman.pos_y == game.ghosts[i].pos_y + 1 && game.pacman.pos_x == game.ghosts[i].pos_x)
                    ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, game.ghosts[i].pos_y - 1, game.ghosts[i].pos_x);
                else if (game.pacman.pos_y == game.ghosts[i].pos_y - 1 && game.pacman.pos_x == game.ghosts[i].pos_x)
                    ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, game.ghosts[i].pos_y + 1, game.ghosts[i].pos_x);
                else 
                    ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, random_int(0, game.map_height - 1), random_int(0, game.map_width - 1));
                break;
            case STATE_SCATTER: ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, random_int(0, game.map_height - 1), random_int(0, game.map_width - 1)); break;
            case STATE_EATEN: ghost_set_target_now(&game.ghosts[i], game.ghosts[i].state, game.ghosts[i].spawn_pos_y, game.ghosts[i].spawn_pos_x); break;
            default: app_abort("ghost_tick()", "Unknown ghost state") break;
        }
    }

    if (timer_triggered(SLOT_GHOST_MOVE, game.pacman.power_mode ? SLOT_GHOST_MOVE_VALUE_FRIGHTENED : SLOT_GHOST_MOVE_VALUE))
        for (int i = 0; i < game.ghosts_amount; i++)
            ghost_moveto_target(&game.ghosts[i]);

    if (game.pacman.power_mode)
    for (int i = 0; i < game.ghosts_amount; i++)
        if (game.ghosts[i].pos_y == game.pacman.pos_y && game.ghosts[i].pos_x == game.pacman.pos_x && !game.ghosts[i].is_eaten) {
            game.ghosts[i].is_eaten = true;
            ghost_set_state(&game.ghosts[i], STATE_EATEN);
            game.round_score += SCORE_GIVE_EAT_GHOST;
            PlaySound(resources.sounds[SOUND_PACMAN_EATGHOST]);
        }
}