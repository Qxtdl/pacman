#pragma once

#include <raylib.h>

#include "game.h"

extern struct resources {
    Texture2D *textures;
    Sound *sounds;
} resources;

void load_assets(void);
void load_map(
    const char *filename,
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH], 
    int *map_height,
    int *map_width,
    int *pacman_pos_x,
    int *pacman_pos_y,
    int *game_max_points
);
