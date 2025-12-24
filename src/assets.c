#include <stdio.h>
#include <string.h>
#include <raylib.h>

#include "global.h"
#include "util.h"
#include "game.h"

struct {
    Texture2D *textures;
} resources;

void load_texture(const char *filename, int newWidth, int newHeight) {
    static unsigned long i = 0;
    Image image = LoadImage(filename);
    if (newWidth != 0 && newHeight  != 0)
        ImageResize(&image, newWidth, newHeight);
    Texture texture = LoadTextureFromImage(image);
    resources.textures = srealloc(resources.textures, ++i * sizeof(Texture2D));
    resources.textures[i - 1] = texture;
}

void load_assets(void) {
    // pacman
    load_texture("assets/textures/pacman/pacOpen.png", TEXTURE_SCALE, TEXTURE_SCALE);
    load_texture("assets/textures/pacman/pacClosed.png", TEXTURE_SCALE, TEXTURE_SCALE);

    // map
    load_texture("assets/textures/map/map3-12.png", 0, 0);
}

void load_map(
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH], 
    int *map_height,
    int *map_width,
    int *pacman_pos_x,
    int *pacman_pos_y
) {
    FILE *file = fopen("assets/map.txt", "r");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char *map_txt = smalloc(file_size + 1);
    size_t read_size = fread(map_txt, 1, file_size, file);
    if ((long)read_size != file_size)
        app_abort("load_map()", "Error reading map.txt")
    map_txt[file_size] = '\0';

    int i = 0, j = 0;
    char *cell = strtok(map_txt, ",");
    while (1) {
        if (i > MAX_MAP_HEIGHT - 1 || j > MAX_MAP_WIDTH - 1)
            app_abort("load_map()", "Buffer overflow writing in map: %d %d", i, j)

        if (strstr(cell, "\n")) {
            i++, j = 0;
            goto do_strtok;
        }
        unsigned long cell_value = strtoul(cell, NULL, 10);
        (*map)[i][j++] = cell_value;
        if  (cell_value == CELL_PACMAN_SPAWN)
            *pacman_pos_y = i, *pacman_pos_x = j;
    
    do_strtok:
        if (!(cell = strtok(NULL, ",")))
            break;
    }
    *map_height = i + 1, *map_width = j;
}