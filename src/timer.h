#pragma once

#include <stddef.h>

typedef enum {
    SLOT_PACMAN_MOVE,
    SLOT_GHOST_MOVE,
    
    SLOT_PACMAN_SPRITE_STATE,
    SLOT_PACMAN_POWER_DURATION,

    SLOT_ROUND_END_DURATION,

    TIMER_SLOT_AMOUNT
} timer_slot_t;

#define SLOT_PACMAN_MOVE_VALUE 0.3

#define SLOT_GHOST_MOVE_VALUE 0.3
#define SLOT_GHOST_MOVE_VALUE_FRIGHTENED 0.3

#define SLOT_PACMAN_SPRITE_STATE_VALUE 0.2
#define SLOT_PACMAN_POWER_DURATION_VALUE 15

#define SLOT_ROUND_END_DURATION_VALUE 4

void init_timer_slot(size_t size);
bool timer_triggered(timer_slot_t slot, double interval);
double get_delta_time(timer_slot_t slot);
void set_timer_slot(timer_slot_t slot, double time);