#pragma once

#include "texture_index.h"

#define TEXTURE_SCALE 30

#define MAX_MAP_HEIGHT 32
#define MAX_MAP_WIDTH 32

struct game {
    struct {
        texture_index_t sprite_state;
        int pos_x, pos_y;
        bool move_up, move_left, move_down, move_right;
        int points;
    } pacman;
    int max_points;
    char map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
    int map_height, map_width;
};

enum {
    CELL_BACKGROUND,
    CELL_WALL,

    CELL_PACMAN_SPAWN,
    CELL_GHOST_SPAWNER,
    
    CELL_POINT
};

void game_setup(void);
void game_tick(void);
void game_draw(void);