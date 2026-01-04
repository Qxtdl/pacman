#pragma once

#include <stdbool.h>

#include "texture_index.h"
#include "game/pacman.h"
#include "game/ghost.h"

#define TEXTURE_SCALE 30

#define MAX_MAP_HEIGHT 40
#define MAX_MAP_WIDTH 40

#define SCORE_GIVE_EAT_POINT 1
#define SCORE_GIVE_EAT_GHOST 100
#define SCORE_LOSE_LOST 200

#define IS_DEBUGGING(x) (session.debugging.x && session.is_debugging)
#define IS_DEBUGGING_ONCE(x) (!(session.debugging.x = false) && session.is_debugging)

struct session {
    int current_level;
    int score;

    bool is_debugging;
    struct {
        bool
            ghost_tick,
            next_level;
    } debugging;
};

struct game {
    struct pacman pacman;
    ghost_t *ghosts;
    int ghosts_amount;

    int map_height, map_width;
    char map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];

    int max_points;
    bool level_won, game_over;
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