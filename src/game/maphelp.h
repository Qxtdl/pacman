#pragma once

#include <stdbool.h>

bool is_wall(int pos_y, int pos_x);
bool is_ghost(int pos_y, int pos_x);
bool is_wall_or_ghost(int pos_y, int pos_x);