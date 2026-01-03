#include <stdlib.h>
#include <raylib.h>

#include "timer.h"
#include "util.h"

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

double get_delta_time(timer_slot_t slot) {
    return GetTime() - timer_slots[slot];
}

void set_timer_slot(timer_slot_t slot, double time) {
    timer_slots[slot] = time;
}