#pragma once

#include "entity.h"
#include "../asset_index.h"

struct pacman {
    texture_index_t sprite_state;

    int pos_x, pos_y;
    enum direction direction, old_direction, real_direction;

    int points;

    bool power_mode;
};

void pacman_tick(void);