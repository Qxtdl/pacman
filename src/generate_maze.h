#pragma once

#include "game.h"

void generate_maze(
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH],
    int *maze_height,
    int *maze_width
);