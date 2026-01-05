#include <stdlib.h>
#include <raylib.h>

#include "timer.h"
#include "util.h"

struct timer timer = {
    .pacman_move_interval = 0.3,
    .ghost_move_interval = 0.3,
    .ghost_move_interval_frightened = 0.3,
    .pacman_sprite_state_interval = 0.2,
    .pacman_power_duration_interval = 15,
    .round_end_duration_interval = 4
};

static double *timer_slots = NULL;

void init_timer_slot(size_t size) {
    if (timer_slots) free(timer_slots);
    timer_slots = scalloc(size, sizeof(double));
}

bool timer_triggered(timer_slot_t slot, double interval) {
    double current_time = GetTime();
    if (current_time - timer_slots[slot] >= interval) { // the subtraction result can be used for smooth movement ?
        timer_slots[slot] = current_time;
        return true;
    }
    return false;
}

void set_timer_slot(timer_slot_t slot, double time) {
    timer_slots[slot] = time;
}

double get_timer_slot_delta(timer_slot_t slot) {
    return timer_slots[slot] - GetTime();
}

double get_delta_time(timer_slot_t slot) {
    return GetTime() - timer_slots[slot];
}
