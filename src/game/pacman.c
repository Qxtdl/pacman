#include <raylib.h>

#include "../global.h"
#include "pacman.h"
#include "../game.h"
#include "../timer.h"
#include "ghost.h"
#include "maphelp.h"

static inline void pacman_move(int *pos, int new_pos) {
    *pos = new_pos;
}

static inline void set_pacman_direction(enum direction direction) {
    game.pacman.old_direction = game.pacman.direction;
    game.pacman.direction = direction;
}

void pacman_tick(void) { 
    if (timer_triggered(SLOT_PACMAN_SPRITE_STATE, SLOT_PACMAN_SPRITE_STATE_VALUE)) {
        if (game.pacman.sprite_state == TEXTURE_PACOPEN)
            game.pacman.sprite_state = TEXTURE_PACCLOSED;
        else game.pacman.sprite_state = TEXTURE_PACOPEN;
    }

    if (IsKeyDown(KEY_W)) game.pacman.direction = DIRECTION_MOVE_UP;
    if (IsKeyDown(KEY_A)) game.pacman.direction = DIRECTION_MOVE_LEFT;
    if (IsKeyDown(KEY_S)) game.pacman.direction = DIRECTION_MOVE_DOWN;
    if (IsKeyDown(KEY_D)) game.pacman.direction = DIRECTION_MOVE_RIGHT;

    if (timer_triggered(SLOT_PACMAN_MOVE, SLOT_PACMAN_MOVE_VALUE)) {
        bool retried = false;
        enum direction direction = game.pacman.direction;
        retry:
        int new_pos_x = game.pacman.pos_x, new_pos_y = game.pacman.pos_y;
        switch (direction) {
            case DIRECTION_NULL: break;
            case DIRECTION_MOVE_UP: new_pos_y = game.pacman.pos_y - 1; break;
            case DIRECTION_MOVE_LEFT: new_pos_x = game.pacman.pos_x - 1; break;
            case DIRECTION_MOVE_DOWN: new_pos_y = game.pacman.pos_y + 1; break;
            case DIRECTION_MOVE_RIGHT: new_pos_x = game.pacman.pos_x + 1; break;
            default: app_abort("pacman_tick()", "Unknown pacman direction")
        }
        if (!is_wall(new_pos_y, new_pos_x)) {
            game.pacman.pos_x = new_pos_x;
            game.pacman.pos_y = new_pos_y;
            game.pacman.old_direction = direction;
        } else if (!retried) {
            direction = game.pacman.old_direction;
            retried = true;
            goto retry;
        }
        if (!is_wall(new_pos_y, new_pos_x))
            game.pacman.real_direction = direction;
        else
            game.pacman.real_direction = DIRECTION_NULL;
    }

    if (timer_triggered(SLOT_PACMAN_POWER_DURATION, SLOT_PACMAN_POWER_DURATION_VALUE)) {
        game.pacman.power_mode = false;

        for (int i = 0; i < game.ghosts_amount; i++)
            if (!game.ghosts[i].is_eaten) set_ghost_state(&game.ghosts[i], STATE_CHASE);
    }
    
    if (game.map[game.pacman.pos_y][game.pacman.pos_x] == CELL_POINT) {
        game.map[game.pacman.pos_y][game.pacman.pos_x] = CELL_BACKGROUND;
        game.pacman.points++;
        session.score += SCORE_GIVE_EAT_POINT;
    }
    else if (game.map[game.pacman.pos_y][game.pacman.pos_x] == CELL_POWER_PELLET) {
        game.map[game.pacman.pos_y][game.pacman.pos_x] = CELL_BACKGROUND;
        game.pacman.power_mode = true;

        for (int i = 0; i < game.ghosts_amount; i++)
            if (!game.ghosts[i].is_eaten) set_ghost_state(&game.ghosts[i], STATE_FRIGHTENED);

        set_timer_slot(SLOT_PACMAN_POWER_DURATION, GetTime());
    }

    for (int i = 0; !game.pacman.power_mode && i < game.ghosts_amount; i++)
        if (!game.ghosts[i].is_eaten && game.ghosts[i].pos_x == game.pacman.pos_x && game.ghosts[i].pos_y == game.pacman.pos_y) {
            game.game_over = true;
            set_timer_slot(SLOT_ROUND_END_DURATION, GetTime());
            session.score -= SCORE_LOSE_LOST;
            session.score -= game.pacman.points;
        }
}