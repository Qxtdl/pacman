#include <stdbool.h>
#include <raylib.h>

#include "assets.h"
#include "game.h"

int main() {    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MAX_MAP_HEIGHT * TEXTURE_SCALE, MAX_MAP_WIDTH * TEXTURE_SCALE, "Pacman");
    SetTargetFPS(120);

    InitAudioDevice();

    load_assets();
    game_setup();

    while (!WindowShouldClose()) {
        game_draw();
        game_tick();
    }
}