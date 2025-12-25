#include <stddef.h>
#include <raylib.h>

#include "timer.h"
#include "util.h"

static double *timer_slots = NULL;

void init_timer_slot(size_t size) {
    timer_slots = smalloc(size * sizeof(double));
}

bool timer_triggered(timer_slot_t slot, double interval) {
    double current_time = GetTime();
    if (current_time - timer_slots[slot] >= interval) {
        timer_slots[slot] = current_time;
        return true;
    }
    return false;
}