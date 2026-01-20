#include "../game.h"

void tunnel_entity(int *pos_y, int *pos_x) {
    // TODO: fix y tunnels
    // if (game.map[*pos_y][*pos_x] == CELL_Y_TUNNEL) {
    //     if ((game.map_height - 1)/2 > *pos_y) {
    //         for (int i = *pos_y; i < game.map_height; i++)
    //         if (game.map[*pos_y][j] == CELL_Y_TUNNEL)
    //             *pos_y = i - 1;
    //     }
    //     else if ((game.map_width - 1)/2 < *pos_x) {
    //         for (int j = *pos_x; j >= 0; j--)
    //         if (game.map[*pos_y][j] == CELL_Y_TUNNEL)
    //             *pos_x = j + 1;            
    //     }
    // }
    if (game.map[*pos_y][*pos_x] == CELL_X_TUNNEL) {
        if ((game.map_width - 1)/2 > *pos_x) {
            for (int j = *pos_x; j < game.map_width; j++)
            if (game.map[*pos_y][j] == CELL_X_TUNNEL)
                *pos_x = j - 1;
        }
        else if ((game.map_width - 1)/2 < *pos_x) {
            for (int j = *pos_x; j >= 0; j--)
            if (game.map[*pos_y][j] == CELL_X_TUNNEL)
                *pos_x = j + 1;            
        }
    }
}