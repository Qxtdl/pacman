#pragma once

#include <stddef.h>

typedef enum {
    SLOT_PACMAN_MOVE,
    SLOT_PACMAN_SPRITE_STATE
} timer_slot_t;

void init_timer_slot(size_t size);
bool timer_triggered(timer_slot_t slot, double interval);