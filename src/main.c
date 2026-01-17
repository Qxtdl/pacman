#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "assets.h"
#include "game.h"
#include "timer.h"

int main() {
    srand(time(NULL));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MAX_MAP_HEIGHT * TEXTURE_SCALE, MAX_MAP_WIDTH * TEXTURE_SCALE, "Pacman");
    SetTargetFPS(60);

    InitAudioDevice();

    load_assets();
    memcpy(&original_timer, &timer, sizeof(timer));
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