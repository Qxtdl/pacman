#pragma once

#include "game.h"

#define play_sound(sound) \
    if (session.play_sound) \
    {  \
        PlaySound(sound); \
    }
