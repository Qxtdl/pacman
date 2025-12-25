#pragma once

#include "../texture_index.h"

typedef struct {
    texture_index_t sprite_state;
    int pos_x, pos_y;

    enum {
        STATE_CHASE,
        STATE_SCATTER,
        STATE_FRIGHTENED,
        STATE_EATEN
    } state;
} ghost_t;

void ghosts_setup(void);
void ghost_tick(void);