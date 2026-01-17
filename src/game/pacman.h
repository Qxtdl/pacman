#pragma once

#include "../asset_index.h"

struct pacman {
    texture_index_t sprite_state;

    int pos_x, pos_y;
    enum pacman_direction {
        PACMAN_DIRECTION_NULL,
        PACMAN_DIRECTION_MOVE_UP,
        PACMAN_DIRECTION_MOVE_LEFT,
        PACMAN_DIRECTION_MOVE_DOWN,
        PACMAN_DIRECTION_MOVE_RIGHT
    } direction, old_direction, real_direction;

    int points;

    bool power_mode;
};

void pacman_tick(void);