#include "sort.h"
#include <stdlib.h>
#include "raylib.h"

gameprops *assign_game_weights(gameprops *game_array, int len) {
    if (game_array != NULL && len > 0) {
        for (int i = 0; i < len - 1; i++) {
            game_array[i].weight = GetRandomValue(0, 100);
            game_array[i].id = i + 1;
        }

        game_array[len-1].weight = 0; 
        game_array[len-1].id = len;

        return game_array;
    } else {
        return NULL;
    }
}

gameprops *insertion_sort_games(gameprops *game_array, int len) {
    if (game_array != NULL && len > 0) {
        for (int i = 1; i < len - 1; i++) {
            int j = i;
            while(j > 0 && (game_array[j].weight < game_array[j-1].weight)) {
                gameprops temp = game_array[j-1];
                game_array[j-1] = game_array[j];
                game_array[j] = temp;
                --j;
            }
        }

        return game_array;
    } else {
        return NULL;
    }
}