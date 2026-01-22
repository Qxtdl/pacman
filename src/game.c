#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "game.h"
#include "assets.h"
#include "global.h"
#include "asset_index.h"
#include "timer.h"
#include "colors.h"
#include "game/pacman.h"
#include "game/ghost.h"
#include "game/tunnel.h"
#include "game/entity.h"

#define MAX_LEVEL_FILENAME_LENGTH 32

struct game game = {0};
struct session session = {0};

void game_setup(void) {
    char buf[MAX_LEVEL_FILENAME_LENGTH] = {0};
    strcpy(buf, "assets/");
    strcat(buf, "level");
    sprintf(buf + strlen(buf), "%d", session.current_level);
    strcat(buf, ".txt");

    init_timer_slot(TIMER_SLOT_AMOUNT);
    if (!load_map(buf, &game.map, &game.map_height, &game.map_width, &pacman.pos_y, &pacman.pos_x))
        generate_map(&game.map, &game.map_height, &game.map_width, &pacman.pos_y, &pacman.pos_x);
    map_count_max_points();
    ghosts_setup();
}

void game_tick(void) {
    if (game.won) return;

    if (IsKeyPressed(KEY_G))
        session.is_debugging = !session.is_debugging;
    if (IsKeyPressed(KEY_J))
        session.debugging.ghost_tick = !session.debugging.ghost_tick;
    if (IsKeyPressed(KEY_K))
        session.debugging.next_level = !session.debugging.next_level;

    if (game.game_over) {
        if (timer_triggered(SLOT_ROUND_END_DURATION, SLOT_ROUND_END_DURATION_VALUE) || IsKeyPressed(KEY_SPACE)) {
            memset(&game, 0, sizeof(game));
            reset_ghosts();
            game_setup();
        }
        return;
    }
    else if (game.level_won) {
        if (timer_triggered(SLOT_ROUND_END_DURATION, SLOT_ROUND_END_DURATION_VALUE) || IsKeyPressed(KEY_SPACE)) {
            int round_score = game.round_score;
            memset(&game, 0, sizeof(game));
            session.current_level++;
            session.score += round_score;
            reset_ghosts();
            game_setup();
            PlaySound(resources.sounds[SOUND_PACMAN_INTERMISSION]);
        }
        return;    
    }
    if (pacman.points == game.max_points || IS_DEBUGGING(next_level)) {
        set_timer_slot(SLOT_ROUND_END_DURATION, GetTime());
        game.level_won = true;
    }

    pacman_tick();
    if (!IS_DEBUGGING(ghost_tick)) ghost_tick();

    tunnel_entity(&pacman.pos_y, &pacman.pos_x);
    for (int i = 0; i < ghosts_amount; i++)
        tunnel_entity(&ghosts[i].pos_y, &ghosts[i].pos_x);
}

static void draw_entity(int pos_y, int pos_x, enum direction direction, double delta, texture_index_t sprite) {
    int draw_pos_y = pos_y * TEXTURE_SCALE, draw_pos_x = pos_x * TEXTURE_SCALE;
    switch (direction) {
        case DIRECTION_NULL: break;
        case DIRECTION_UP: draw_pos_y = delta * TEXTURE_SCALE + pos_y * TEXTURE_SCALE; break;
        case DIRECTION_LEFT: draw_pos_x = delta * TEXTURE_SCALE + pos_x * TEXTURE_SCALE; break;
        case DIRECTION_DOWN: draw_pos_y = fabs(delta) * TEXTURE_SCALE + pos_y * TEXTURE_SCALE; break;
        case DIRECTION_RIGHT: draw_pos_x = fabs(delta) * TEXTURE_SCALE + pos_x * TEXTURE_SCALE; break;
    }
    DrawTexture(resources.textures[sprite], 
        draw_pos_x, 
        draw_pos_y, 
        WHITE);
}

static void draw_map(void) {
    for (int i = 0; i < game.map_height; i++) {
        for (int j = 0; j < game.map_width; j++) {
            switch (game.map[i][j]) {
                case CELL_PACMAN_SPAWN:
                case CELL_GHOST_SPAWNER:
                case CELL_Y_TUNNEL:
                case CELL_X_TUNNEL:
                case CELL_BACKGROUND: DrawRectangle( j * TEXTURE_SCALE, i * TEXTURE_SCALE, TEXTURE_SCALE, TEXTURE_SCALE, BLACK); break;
                case CELL_WALL: DrawRectangle( j * TEXTURE_SCALE, i * TEXTURE_SCALE, TEXTURE_SCALE, TEXTURE_SCALE, WALL_COLOR); break;
                case CELL_POINT: DrawCircle(j * TEXTURE_SCALE + 15, i * TEXTURE_SCALE + 15, TEXTURE_SCALE/4.0f, WHITE); break;
                case CELL_POWER_PELLET: DrawCircle(j * TEXTURE_SCALE + 15, i * TEXTURE_SCALE + 15, TEXTURE_SCALE/3.0f, WHITE); break; // todo: make it change size
                default: DrawTexture(resources.textures[TEXTURE_ERROR], j * TEXTURE_SCALE, i * TEXTURE_SCALE, WHITE); break;
            }
            if (i == pacman.pos_y && j == pacman.pos_x)
                draw_entity(pacman.pos_y, pacman.pos_x, pacman.real_direction, get_timer_slot_delta(SLOT_PACMAN_MOVE), pacman.sprite_state);
            
            for (int k = 0; k < ghosts_amount; k++)
                if (ghosts[k].pos_y == i && ghosts[k].pos_x == j) {
                    #ifdef SMOOTH_GHOST_MOVEMENT
                    draw_entity(ghosts[k].pos_y, ghosts[k].pos_x, ghosts[k].direction, get_timer_slot_delta(SLOT_GHOST_MOVE), ghosts[k].sprite_state);
                    #else
                    draw_entity(ghosts[k].pos_y, ghosts[k].pos_x, ghosts[k].direction, 0, ghosts[k].sprite_state);
                    #endif
                    // Color tint = WHITE;

                    // if (pacman.power_mode) tint = DARKGRAY;
                    // if (ghosts[k].is_eaten) tint = GREEN;
                    // DrawTexture(
                    //     resources.textures[ghosts[k].sprite_state], 
                    //     j * TEXTURE_SCALE, 
                    //     i * TEXTURE_SCALE, 
                    //     tint
                    // );
                    if (session.is_debugging) DrawText(TextFormat("%d", ghosts[k].state), ghosts[k].pos_x * TEXTURE_SCALE, ghosts[k].pos_y * TEXTURE_SCALE, TEXTURE_SCALE/2, WHITE);
                }
        }
    }
}

void game_draw(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    draw_map();

    DrawText(TextFormat("Points: %d\nMax Points: %d", pacman.points, game.max_points), 0, game.map_height * TEXTURE_SCALE, TEXTURE_SCALE, WHITE);
    DrawText(TextFormat("Score: %d Lvl Score: %d", session.score, game.round_score), 8 * TEXTURE_SCALE, game.map_height * TEXTURE_SCALE, TEXTURE_SCALE, WHITE);
    if (pacman.power_mode) DrawText(TextFormat("ATE A POWER PELLET\nTIME REMAINING: %f\n", get_delta_time(SLOT_PACMAN_POWER_DURATION)), (pacman.pos_x - 2) * TEXTURE_SCALE, (pacman.pos_y - 1) * TEXTURE_SCALE, TEXTURE_SCALE/1.5, RED);
    if (game.game_over) DrawText("GAME OVER\nPress space", 0, (game.map_height + 2) * TEXTURE_SCALE, TEXTURE_SCALE, RED);
    if (game.level_won) DrawText("GAME WON!\nPress space -> next level", 0, (game.map_height + 2) * TEXTURE_SCALE, TEXTURE_SCALE, GREEN);
    if (game.won) DrawText(TextFormat("YOU\nWON\n%d POINTS", session.score), GetScreenWidth()/2, GetScreenHeight()/2, TEXTURE_SCALE, GREEN);
    if (session.is_debugging) DrawText("Debugging\nG to exit", 0, (game.map_height + 4) * TEXTURE_SCALE, TEXTURE_SCALE, ORANGE);

    EndDrawing();
}
