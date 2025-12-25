#include <raylib.h>
#include <stdio.h>

#include "game.h"
#include "assets.h"
#include "timer.h"
#include "colors.h"
#include "game/pacman.h"
#include "game/ghost.h"

struct game game = {0};

void game_setup(void) {
    init_timer_slot(3);
    load_map(&game.map, &game.map_height, &game.map_width, &game.pacman.pos_x, &game.pacman.pos_y, &game.max_points);
    ghosts_setup();
}

void game_tick(void) {
    if (game.game_over) return;

    pacman_tick();
    ghost_tick();
}

static void draw_map(void) {
    for (int i = 0; i < game.map_height; i++) {
        for (int j = 0; j < game.map_width; j++) {
            switch (game.map[i][j]) {
                case CELL_PACMAN_SPAWN:
                case CELL_GHOST_SPAWNER:
                case CELL_BACKGROUND: DrawRectangle( j * TEXTURE_SCALE, i * TEXTURE_SCALE, TEXTURE_SCALE, TEXTURE_SCALE, BLACK); break;
                case CELL_WALL: DrawRectangle( j * TEXTURE_SCALE, i * TEXTURE_SCALE, TEXTURE_SCALE, TEXTURE_SCALE, WALL_COLOR); break;
                case CELL_POINT: DrawCircle(j * TEXTURE_SCALE + 15, i * TEXTURE_SCALE + 15, TEXTURE_SCALE / 4.0f, WHITE); break;
                default: DrawTexture(resources.textures[TEXTURE_ERROR], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE); break;
            }
            if (i == game.pacman.pos_y && j == game.pacman.pos_x)
                DrawTexture(resources.textures[game.pacman.sprite_state], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE);
            
            for (int k = 0; k < game.ghosts_amount; k++)
                if (game.ghosts[k].pos_x == j && game.ghosts[k].pos_y == i)
                    DrawTexture(resources.textures[game.ghosts[k].sprite_state], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE);
        }
    }
}

void game_draw(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    draw_map();
    DrawText(TextFormat("Points: %d\nMax Points: %d", game.pacman.points, game.max_points), 0, game.map_height * TEXTURE_SCALE, TEXTURE_SCALE, WHITE);
    if (game.game_over) DrawText("GAME OVER", 0, 0, TEXTURE_SCALE * 2, RED);

    EndDrawing();
}