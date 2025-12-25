#pragma once

#include <stddef.h>

typedef enum {
    SLOT_PACMAN_MOVE,
    SLOT_GHOST_MOVE,
    SLOT_PACMAN_SPRITE_STATE
} timer_slot_t;

#define SLOT_PACMAN_MOVE_VALUE 0.2
#define SLOT_GHOST_MOVE_VALUE 0.4
#define SLOT_PACMAN_SPRITE_STATE_VALUE 0.2

void init_timer_slot(size_t size);
bool timer_triggered(timer_slot_t slot, double interval);