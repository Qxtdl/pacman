#pragma once

#include <raylib.h>

#include "game.h"

extern struct {
    Texture2D *textures;
} resources;

void load_texture(const char *filename);
void load_assets(void);
void load_map(
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH], 
    int *map_height,
    int *map_width,
    int *pacman_pos_x,
    int *pacman_pos_y,
    int *game_max_points
);
