#pragma once

#include <raylib.h>

#include "game.h"

typedef enum {
    TEXTURE_PACOPEN,
    TEXTURE_PACCLOSED,

    TEXTURE_MAP
} texture_index_t;

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
    int *pacman_pos_y
);
