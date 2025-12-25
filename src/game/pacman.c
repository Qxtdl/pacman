#include <raylib.h>

#include "../game.h"
#include "../timer.h"

extern struct game game;

void pacman_tick(void) {
    if (timer_triggered(SLOT_PACMAN_SPRITE_STATE, 0.2)) {
        if (game.pacman.sprite_state == TEXTURE_PACOPEN)
            game.pacman.sprite_state = TEXTURE_PACCLOSED;
        else game.pacman.sprite_state = TEXTURE_PACOPEN;
    }    
    if (timer_triggered(SLOT_PACMAN_MOVE, 0.3)) {
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
    
    if (game.map[game.pacman.pos_y][game.pacman.pos_x] == CELL_POINT) {
        game.map[game.pacman.pos_y][game.pacman.pos_x] = CELL_BACKGROUND;
        game.pacman.points++;
    }
}