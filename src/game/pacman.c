#include <raylib.h>

#include "../global.h"
#include "pacman.h"
#include "../game.h"
#include "../timer.h"
#include "ghost.h"
#include "maphelp.h"
#include "../assets.h"

struct pacman pacman = {0};

static inline void set_pacman_direction(enum direction direction) {
    pacman.old_direction = pacman.direction;
    pacman.direction = direction;
}

void pacman_tick(void) { 
    if (IsKeyDown(KEY_W)) pacman.direction = DIRECTION_UP;
    if (IsKeyDown(KEY_A)) pacman.direction = DIRECTION_LEFT;
    if (IsKeyDown(KEY_S)) pacman.direction = DIRECTION_DOWN;
    if (IsKeyDown(KEY_D)) pacman.direction = DIRECTION_RIGHT;

    if (timer_triggered(SLOT_PACMAN_SPRITE_STATE, SLOT_PACMAN_SPRITE_STATE_VALUE)) {
        if (pacman.sprite_state == TEXTURE_PACOPEN)
            pacman.sprite_state = TEXTURE_PACCLOSED;
        else pacman.sprite_state = TEXTURE_PACOPEN;
    }

    if (timer_triggered(SLOT_PACMAN_MOVE, SLOT_PACMAN_MOVE_VALUE)) {
        bool retried = false;
        enum direction direction = pacman.direction;
        retry:;
        int new_pos_x = pacman.pos_x, new_pos_y = pacman.pos_y;
        switch (direction) {
            case DIRECTION_NULL: break;
            case DIRECTION_UP: new_pos_y = pacman.pos_y - 1; break;
            case DIRECTION_LEFT: new_pos_x = pacman.pos_x - 1; break;
            case DIRECTION_DOWN: new_pos_y = pacman.pos_y + 1; break;
            case DIRECTION_RIGHT: new_pos_x = pacman.pos_x + 1; break;
        }
        if (!is_wall(new_pos_y, new_pos_x)) {
            pacman.pos_x = new_pos_x;
            pacman.pos_y = new_pos_y;
            pacman.old_direction = direction;
        } else if (!retried) {
            direction = pacman.old_direction;
            retried = true;
            goto retry;
        }
        if (!is_wall(new_pos_y, new_pos_x))
            pacman.real_direction = direction;
        else
            pacman.real_direction = DIRECTION_NULL;
    }
    
    if (game.map[pacman.pos_y][pacman.pos_x] == CELL_POINT) {
        game.map[pacman.pos_y][pacman.pos_x] = CELL_BACKGROUND;
        pacman.points++;
        game.round_score += SCORE_GIVE_EAT_POINT;
        PlaySound(resources.sounds[SOUND_PACMAN_CHOMP]);
    }

    if (timer_triggered(SLOT_PACMAN_POWER_DURATION, SLOT_PACMAN_POWER_DURATION_VALUE)) {
        pacman.power_mode = false;
    }
    if (game.map[pacman.pos_y][pacman.pos_x] == CELL_POWER_PELLET) {
        game.map[pacman.pos_y][pacman.pos_x] = CELL_BACKGROUND;

        // FOR NOW this code stays here since 
        for (int i = 0; i < ghosts_amount; i++)
            if (!pacman.power_mode && !ghosts[i].is_eaten) ghost_set_state(&ghosts[i], STATE_FRIGHTENED);

        set_timer_slot(SLOT_PACMAN_POWER_DURATION, GetTime());
        PlaySound(resources.sounds[SOUND_PACMAN_EATFRUIT]);
        PlaySound(resources.sounds[SOUND_PACMAN_EXTRAPAC]);

        pacman.power_mode = true;
    }

    for (int i = 0; !pacman.power_mode && i < ghosts_amount; i++)
        if (!ghosts[i].is_eaten && ghosts[i].pos_x == pacman.pos_x && ghosts[i].pos_y == pacman.pos_y) {
            game.game_over = true;
            set_timer_slot(SLOT_ROUND_END_DURATION, GetTime());
            game.round_score -= SCORE_GIVE_EAT_GHOST * 2;
            PlaySound(resources.sounds[SOUND_PACMAN_DEATH]);
        }
}