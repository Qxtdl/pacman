#pragma once

#include <stdbool.h>

#include "entity.h"
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

    int pos_y, pos_x;
    int spawn_pos_x, spawn_pos_y;

    enum state state, old_state;
    int state_vector[STATE_AMOUNT][2];
    bool reached_vector;

    enum direction direction;

    bool is_eaten;
} ghost_t;

extern ghost_t *ghosts;
extern int ghosts_amount;

void ghosts_setup(void);
void reset_ghosts(void);
void ghost_set_state(ghost_t *ghost, enum state state);
void ghost_tick(void);