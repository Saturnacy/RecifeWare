#ifndef PLAYER_H
#define PLAYER_H

typedef struct jogador
{
    int ponto;
    char nome_usuario[4];
}Jogador;

void update_ranking(int mat[11][5], int *nova);
void save_ranking(int mat[11][5]);
void restore_ranking(int mat[11][5]);
int validate_name(char *nome, int mat[11][5], int cont);
void display_ranking(int mat[11][5]);

#endif