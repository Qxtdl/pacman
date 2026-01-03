#pragma once

#include "../texture_index.h"

struct pacman {
    texture_index_t sprite_state;

    int pos_x, pos_y;
    enum direction {
        DIRECTION_NULL,
        DIRECTION_MOVE_UP,
        DIRECTION_MOVE_LEFT,
        DIRECTION_MOVE_DOWN,
        DIRECTION_MOVE_RIGHT
    } direction, old_direction;

    int points;

    bool power_mode;
};

void pacman_tick(void);