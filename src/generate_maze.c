#include <stdlib.h>
#include <string.h>

#include "game/pacman.h"
#include "util.h"
#include "game.h"

// typedef struct {
//     int x, y;
// } cord_t;

// static cord_t *stack = NULL;
// static size_t stack_size = 0;

// static cord_t push_cord(int y, int x) {
//     stack = srealloc(stack, ++stack_size * sizeof(cord_t));
//     stack[stack_size - 1].y = y;
//     stack[stack_size - 1].x = x;
//     return stack[stack_size - 1];
// }

// static cord_t pop_cord(void) {
//     cord_t popped = stack[stack_size];
//     stack = srealloc(stack, --stack_size * sizeof(cord_t));
//     return popped;
// }

enum tunnel_direction {
    TUNNEL_DIRECTION_UP,
    TUNNEL_DIRECTION_LEFT,
    TUNNEL_DIRECTION_DOWN,
    TUNNEL_DIRECTION_RIGHT,
};

static void generate_tunnel(
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH],
    int tunnel_y,
    int tunnel_x,
    int tunnel_length,
    enum tunnel_direction direction,
    enum cell_type type
) {
    for (int i = 0; i < tunnel_length; i++) {
        switch (direction) {
            case TUNNEL_DIRECTION_UP:
                (*map)[clamp(tunnel_y - i, 0, MAX_MAP_HEIGHT - 1)][clamp(tunnel_x, 0, MAX_MAP_WIDTH - 1)] = type; break;
            case TUNNEL_DIRECTION_LEFT:
                (*map)[clamp(tunnel_y, 0, MAX_MAP_HEIGHT - 1)][clamp(tunnel_x - i, 0, MAX_MAP_WIDTH - 1)] = type; break; 
            case TUNNEL_DIRECTION_DOWN:
                (*map)[clamp(tunnel_y + i, 0, MAX_MAP_HEIGHT - 1)][clamp(tunnel_x, 0, MAX_MAP_WIDTH - 1)] = type; break;
            case TUNNEL_DIRECTION_RIGHT:
                (*map)[clamp(tunnel_y, 0, MAX_MAP_HEIGHT - 1)][clamp(tunnel_x + i, 0, MAX_MAP_WIDTH - 1)] = type; break;
        }
    }
}

void generate_maze(
    char (*map)[MAX_MAP_HEIGHT][MAX_MAP_WIDTH],
    int *map_height,
    int *map_width,
    int *game_max_points
) {
/*
    step 1
        first we generate a maze filled with walls that is X*Y big (decided by rng) 
    step 2
        we fill the map with random tunnels of variable size.
        pick random x, y cord. now pick random number (tunnel length) and a random direction
        can be up down left right
        and construct the tunnel
    step 3
        connect the tunnels together
*/

    // step 1
    *map_height = random_int(MAX_MAP_HEIGHT/4, MAX_MAP_HEIGHT);
    *map_width = random_int(MAX_MAP_WIDTH/4, MAX_MAP_WIDTH);
    memset(map, CELL_WALL, sizeof(*map));

    for (int i = 0; i < 100; i++)
        generate_tunnel(
            map,
            random_int(0, *map_height),
            random_int(0, *map_height),
            random_int(0, *map_width),
            random_int(0, 3), 
            random_int(0, 1) ? CELL_POINT : CELL_BACKGROUND);

    int ghosts_amount = 0;
    for (int i = 0; i < *map_height; i++)
        for (int j = 0; i < *map_height && ghosts_amount < 6; i++)
            if (game.map[i][j] == CELL_BACKGROUND) game.map[i][j] = CELL_GHOST_SPAWNER;

    // step 2
    // for (int i = 0; i < random_int(3, 10000); i++) {
    //     generate_tunnel(map, random_int(0, *map_height), random_int(0, *map_width), random_int(0, 5), TUNNEL_DIRECTION_LEFT);
    // }
}   