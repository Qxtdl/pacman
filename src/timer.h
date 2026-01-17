#pragma once

#include <stddef.h>

typedef enum {
    SLOT_PACMAN_MOVE,
    SLOT_GHOST_MOVE,
    SLOT_GHOST_MOVE_FRIGHTHENED,

    SLOT_PACMAN_SPRITE_STATE,
    SLOT_PACMAN_POWER_DURATION,

    SLOT_ROUND_END_DURATION,

    TIMER_SLOT_AMOUNT
} timer_slot_t;

extern struct timer {
    double
        pacman_move_interval,
        ghost_move_interval,
        ghost_move_interval_frightened,
        pacman_sprite_state_interval,
        pacman_power_duration_interval,
        round_end_duration_interval;
} timer;

extern struct timer original_timer;

#define SLOT_PACMAN_MOVE_VALUE (timer.pacman_move_interval)
#define SLOT_PACMAN_FRIGHTENED_MOVE_VALUE (timer.pacman_frightened_move_value)

#define SLOT_GHOST_MOVE_VALUE (timer.ghost_move_interval)
#define SLOT_GHOST_MOVE_VALUE_FRIGHTENED (timer.ghost_move_interval_frightened)

#define SLOT_PACMAN_SPRITE_STATE_VALUE (timer.pacman_sprite_state_interval)
#define SLOT_PACMAN_POWER_DURATION_VALUE (timer.pacman_power_duration_interval)

#define SLOT_ROUND_END_DURATION_VALUE (timer.round_end_duration_interval)

void init_timer_slot(size_t size);
bool timer_triggered(timer_slot_t slot, double interval);
void set_timer_slot(timer_slot_t slot, double time);
double get_timer_slot_delta(timer_slot_t slot);
double get_delta_time(timer_slot_t slot);