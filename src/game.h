#pragma once

#include <stdbool.h>

#include "texture_index.h"
#include "game/pacman.h"
#include "game/ghost.h"

#define TEXTURE_SCALE 30

#define MAX_MAP_HEIGHT 40
#define MAX_MAP_WIDTH 40

struct game {
    struct pacman pacman;
    ghost_t *ghosts;

    int ghosts_amount;

    int current_level;
    char map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
    int map_height, map_width;
    int max_points;

    bool won, game_over;

    bool debugging;
};

enum {
    CELL_BACKGROUND,
    CELL_WALL,

    CELL_PACMAN_SPAWN,
    CELL_GHOST_SPAWNER,
    
    CELL_POINT,
    CELL_POWER_PELLET
};

void game_setup(void);
void game_tick(void);
void game_draw(void);