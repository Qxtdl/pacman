#pragma once

#include <stdbool.h>

#include "../asset_index.h"

enum state {
    STATE_NULL,
    STATE_CHASE,
    STATE_SCATTER,
    STATE_FRIGHTENED,
    STATE_EATEN,
    
    STATE_AMOUNT
};

typedef struct {
    texture_index_t sprite_state;

    int pos_x, pos_y;
    int spawn_pos_x, spawn_pos_y;
    int move_to_target_pos_y, move_to_target_pos_x;

    bool goto_target;

    enum state state, old_state;
    bool new_state;
    int state_vector[STATE_AMOUNT][2];

    bool is_eaten;
} ghost_t;

void ghosts_setup(void);
void reset_ghosts(void);
void set_ghost_state(ghost_t *ghost, enum state state);
void ghost_tick(void);