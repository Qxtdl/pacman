#include <stdbool.h>
#include <raylib.h>

#include "assets.h"
#include "game.h"

int main() {    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Pacman");
    SetTargetFPS(60);
    load_assets();
    game_setup();

    while (!WindowShouldClose()) {
        game_tick();
        game_draw();
    }
}