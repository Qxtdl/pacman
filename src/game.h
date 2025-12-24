#pragma once

#define TEXTURE_SCALE 30

#define MAX_MAP_HEIGHT 32
#define MAX_MAP_WIDTH 32

enum {
    CELL_BACKGROUND,
    CELL_WALL,
    CELL_PACMAN_SPAWN,
    CELL_GHOST_SPAWNER,

};

void game_setup(void);
void game_tick(void);
void game_draw(void);