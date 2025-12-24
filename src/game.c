#include <stdlib.h>
#include <raylib.h>

#include "game.h"
#include "util.h"
#include "assets.h"
#include "colors.h"

struct {
    struct {
        texture_index_t sprite_state;
        int pos_x, pos_y;
        bool move_up, move_left, move_down, move_right;
    } pacman;
    char map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
    int map_height, map_width;
} game = {0};

typedef enum {
    SLOT_PACMAN_MOVE,
    SLOT_PACMAN_SPRITE_STATE
} timer_slot_t;

static double *timer_slots = NULL;

static void init_timer_slot(size_t size) {
    timer_slots = smalloc(size * sizeof(double));
}

static bool timer_triggered(timer_slot_t slot, double interval) {
    double current_time = GetTime();
    if (current_time - timer_slots[slot] >= interval) {
        timer_slots[slot] = current_time;
        return true;
    }
    return false;
}

void game_setup(void) {
    init_timer_slot(2);
    load_map(&game.map, &game.map_height, &game.map_width, &game.pacman.pos_x, &game.pacman.pos_y);

}

void game_tick(void) {
    if (timer_triggered(SLOT_PACMAN_SPRITE_STATE, 0.2)) {
        if (game.pacman.sprite_state == TEXTURE_PACOPEN)
            game.pacman.sprite_state = TEXTURE_PACCLOSED;
        else game.pacman.sprite_state = TEXTURE_PACOPEN;
    }    
    if (timer_triggered(SLOT_PACMAN_MOVE,0.3)) {
        if (game.pacman.move_up && game.map[game.pacman.pos_y - 1][game.pacman.pos_x] != CELL_WALL)
            game.pacman.pos_y--;
        if (game.pacman.move_left && game.map[game.pacman.pos_y][game.pacman.pos_x - 1] != CELL_WALL)
            game.pacman.pos_x--;
        if (game.pacman.move_down && game.map[game.pacman.pos_y + 1][game.pacman.pos_x] != CELL_WALL)
            game.pacman.pos_y++;
        if (game.pacman.move_right && game.map[game.pacman.pos_y][game.pacman.pos_x + 1] != CELL_WALL)
            game.pacman.pos_x++;        
    }
    if (IsKeyDown(KEY_W)) {
        game.pacman.move_up = true;
        game.pacman.move_left = false;
        game.pacman.move_down = false;
        game.pacman.move_right = false;
    }
    if (IsKeyDown(KEY_A)) {
        game.pacman.move_up = false;
        game.pacman.move_left = true;
        game.pacman.move_down = false;
        game.pacman.move_right = false;
    }
    if (IsKeyDown(KEY_S)) {
        game.pacman.move_up = false;
        game.pacman.move_left = false;
        game.pacman.move_down = true;
        game.pacman.move_right = false;
    }
    if (IsKeyDown(KEY_D)) {
        game.pacman.move_up = false;
        game.pacman.move_left = false;
        game.pacman.move_down = false;
        game.pacman.move_right = true;
    }
}

static void draw_map(void) {
    for (int i = 0; i < game.map_height; i++) {
        for (int j = 0; j < game.map_width; j++) {
            switch (game.map[i][j]) {
                case CELL_PACMAN_SPAWN:
                case CELL_GHOST_SPAWNER:
                case CELL_BACKGROUND: DrawRectangle( j * TEXTURE_SCALE, i * TEXTURE_SCALE, TEXTURE_SCALE, TEXTURE_SCALE, BLACK); break;
                case CELL_WALL: DrawRectangle( j * TEXTURE_SCALE, i * TEXTURE_SCALE, TEXTURE_SCALE, TEXTURE_SCALE, WALL_COLOR); break;
                default: DrawTexture(resources.textures[TEXTURE_ERROR], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE);
            }
            if (i == game.pacman.pos_y && j == game.pacman.pos_x)
                DrawTexture(resources.textures[game.pacman.sprite_state], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE);
        }
    }
}

void game_draw(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    draw_map();
    DrawFPS(0, game.map_height * TEXTURE_SCALE);

    EndDrawing();
}