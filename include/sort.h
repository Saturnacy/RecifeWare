#ifndef SORT_H
#define SORT_H

typedef struct { 
    int weight;
    int id;
} gameprops;

gameprops *assign_game_weights(gameprops *game_array, int len);
gameprops *insertion_sort_games(gameprops *game_array, int len);

#endif