#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "assets.h"
#include "global.h"
#include "util.h"
#include "game.h"
#include "timer.h"

struct resources resources;

static void load_texture(const char *filename, int newWidth, int newHeight) {
    static unsigned long i = 0;
    Image image = LoadImage(filename);
    if (newWidth != 0 && newHeight  != 0)
        ImageResize(&image, newWidth, newHeight);
    Texture texture = LoadTextureFromImage(image);
    resources.textures = srealloc(resources.textures, ++i * sizeof(Texture2D));
    resources.textures[i - 1] = texture;
}

static void load_audio(const char *filename) {
    static unsigned long i = 0;
    Sound sound = LoadSound(filename);
    resources.sounds = srealloc(resources.sounds, ++i * sizeof(Sound));
    resources.sounds[i - 1] = sound;
}

void load_assets(void) {
    /* TEXTURES */
    // pacman
    load_texture("assets/textures/pacman/pacOpen.png", TEXTURE_SCALE, TEXTURE_SCALE);
    load_texture("assets/textures/pacman/pacClosed.png", TEXTURE_SCALE, TEXTURE_SCALE);

    // ghost
    load_texture("assets/textures/ghosts/ghost.png", TEXTURE_SCALE, TEXTURE_SCALE);

    // error
    load_texture("assets/textures/error.png", TEXTURE_SCALE, TEXTURE_SCALE);

    /* SOUNDS */
    load_audio("assets/sounds/pacman_chomp.wav");
    load_audio("assets/sounds/pacman_death.wav");
    load_audio("assets/sounds/pacman_eatfruit.wav");
    load_audio("assets/sounds/pacman_eatghost.wav");
    load_audio("assets/sounds/pacman_extrapac.wav");
    load_audio("assets/sounds/pacman_intermission.wav");
}

void load_map(
    const char *filename, // "assets/map.txt"
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH], 
    int *map_height,
    int *map_width,
    int *pacman_pos_x,
    int *pacman_pos_y,
    int *game_max_points
) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        game.won = true;
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char *map_txt = smalloc(file_size + 1);
    size_t read_size = fread(map_txt, 1, file_size, file);
    if ((long)read_size != file_size)
        app_abort("load_map()", "Error reading map.txt")
    fclose(file);
    map_txt[file_size] = '\0';

    /*
        Process map config

        ghost_speed
        ghost_frightened_speed
    */

    char *interval = strstr(map_txt, "ghost_speed=");
    if (interval) timer.ghost_move_interval = strtod(interval + strlen("ghost_speed=") + 1, NULL);
    else app_abort("load_map()", "No ghost_speed attribute")
    interval = strstr(map_txt, "ghost_speed_frightened=");
    if (interval) timer.ghost_move_interval_frightened = strtod(interval + strlen("ghost_speed_frightened=") + 1, NULL);

    if (!strstr(map_txt, "#"))
        app_abort("load_map()", "Map is missing #")

    int i = 0, j = 0;
    char *cell = strtok(strstr(map_txt, "#") + 1, ",");
    cell = strtok(NULL, ",");
    while (1) {
        if (i > MAX_MAP_HEIGHT - 1 || j > MAX_MAP_WIDTH - 1)
            app_abort("load_map()", "Buffer overflow writing in map: %d %d", i, j)

        if (strstr(cell, "\n")) {
            i++, j = 0;
            goto do_strtok;
        }
        unsigned long cell_value = strtoul(cell, NULL, 10);
        if (cell_value == CELL_PACMAN_SPAWN)
            *pacman_pos_y = i, *pacman_pos_x = j;
        if (cell_value == CELL_POINT)
            (*game_max_points)++;
        (*map)[i][j++] = cell_value;
    
    do_strtok:
        if (!(cell = strtok(NULL, ",")))
            break;
    }
    *map_height = i + 1, *map_width = j;
    free(map_txt);
}