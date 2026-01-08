#include <stdbool.h>
#include <raylib.h>

#include "assets.h"
#include "game.h"
#include <stdio.h>
int main() {    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MAX_MAP_HEIGHT * TEXTURE_SCALE, MAX_MAP_WIDTH * TEXTURE_SCALE, "Pacman");
    SetTargetFPS(120);

    InitAudioDevice();

    load_assets();
    game_setup();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_M)) {
            if (GetMasterVolume() == 0)
                SetMasterVolume(100);
            else
                SetMasterVolume(0);
        }
        game_draw();
        game_tick();
    }
}