#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "assets.h"
#include "texture_index.h"
#include "timer.h"
#include "colors.h"
#include "game/pacman.h"
#include "game/ghost.h"

#define MAX_LEVEL_FILENAME_LENGTH 32

struct game game = {0};

void game_setup(void) {
    char buf[MAX_LEVEL_FILENAME_LENGTH] = {0};
    strcpy(buf, "assets/");
    strcat(buf, "level");
    sprintf(buf + strlen(buf), "%d", game.current_level);
    strcat(buf, ".txt");

    init_timer_slot(TIMER_SLOT_AMOUNT);
    load_map(buf, &game.map, &game.map_height, &game.map_width, &game.pacman.pos_x, &game.pacman.pos_y, &game.max_points);
    ghosts_setup();
}

void game_tick(void) {
    if (game.game_over) {
        if (timer_triggered(SLOT_ROUND_END_DURATION, SLOT_ROUND_END_DURATION_VALUE) || IsKeyPressed(KEY_SPACE)) {
            memset(&game, 0, sizeof(game));
            game.game_over = false;
            reset_ghosts();
            game_setup();
        }
        return;
    }

    if (game.won) {
        if (timer_triggered(SLOT_ROUND_END_DURATION, SLOT_ROUND_END_DURATION_VALUE) || IsKeyPressed(KEY_SPACE)) {
            int current_level = game.current_level;
            memset(&game, 0, sizeof(game));
            game.current_level = current_level + 1;
            reset_ghosts();
            game_setup();
        }
        return;    
    }
    if (game.pacman.points == game.max_points || game.debugging && IsKeyPressed(KEY_K)) {
        set_timer_slot(SLOT_ROUND_END_DURATION, GetTime());
        game.won = true;
    }

    if (IsKeyPressed(KEY_G))
        game.debugging = !game.debugging;
    static bool do_ghost_tick = true;
    if (game.debugging && IsKeyPressed(KEY_J))
        do_ghost_tick = !do_ghost_tick;

    pacman_tick();
    if (do_ghost_tick) ghost_tick();
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
                case CELL_POWER_PELLET: DrawCircle(j * TEXTURE_SCALE + 15, i * TEXTURE_SCALE + 15, TEXTURE_SCALE / 3.0f, WHITE); break; // todo: make it change size
                default: DrawTexture(resources.textures[TEXTURE_ERROR], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE); break;
            }
            if (i == game.pacman.pos_y && j == game.pacman.pos_x)
                DrawTexture(resources.textures[game.pacman.sprite_state], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE);
            
            for (int k = 0; k < game.ghosts_amount; k++)
                if (game.ghosts[k].pos_x == j && game.ghosts[k].pos_y == i) {
                    Color tint = WHITE;
                    if (game.pacman.power_mode) tint = DARKGRAY;
                    if (game.ghosts[k].is_eaten) tint = GREEN;
                    DrawTexture(
                        resources.textures[game.ghosts[k].sprite_state], 
                        j * TEXTURE_SCALE, 
                        i * TEXTURE_SCALE, 
                        tint
                    );
                    if (game.debugging) DrawText(TextFormat("%d", game.ghosts[k].state), game.ghosts[k].pos_x * TEXTURE_SCALE, game.ghosts[k].pos_y * TEXTURE_SCALE, TEXTURE_SCALE / 2, WHITE);
                }
        }
    }
}

void game_draw(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    draw_map();
    DrawText(TextFormat("Points: %d\nMax Points: %d", game.pacman.points, game.max_points), 0, game.map_height * TEXTURE_SCALE, TEXTURE_SCALE, WHITE);
    if (game.pacman.power_mode) DrawText(TextFormat("ATE A POWER PELLET\nTIME REMAINING: %f\n", (float)get_delta_time(SLOT_PACMAN_POWER_DURATION)), (game.pacman.pos_x - 2) * TEXTURE_SCALE, (game.pacman.pos_y - 1) * TEXTURE_SCALE, TEXTURE_SCALE / 1.5, RED);
    if (game.game_over) DrawText("GAME OVER\nPress space", 0, (game.map_height + 2) * TEXTURE_SCALE, TEXTURE_SCALE, RED);
    else if (game.won) DrawText("GAME WON!\nPress space -> next level", 0, (game.map_height + 2) * TEXTURE_SCALE, TEXTURE_SCALE, GREEN);
    if (game.debugging) DrawText("Debugging\nG to exit", 0, (game.map_height + 4) * TEXTURE_SCALE, TEXTURE_SCALE, ORANGE);

    EndDrawing();
}
