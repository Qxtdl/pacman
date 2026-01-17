#pragma once

#include <raylib.h>

#include "game.h"

extern struct resources {
    Texture2D *textures;
    Sound *sounds;
} resources;

void load_assets(void);
void map_count_max_points(void);
bool load_map(
    const char *filename,
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH], 
    int *map_height,
    int *map_width,
    int *pacman_pos_x,
    int *pacman_pos_y
);
void generate_map(
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH], 
    int *map_height,
    int *map_width,
    int *pacman_pos_x,
    int *pacman_pos_y
);