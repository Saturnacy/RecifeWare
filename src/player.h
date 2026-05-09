#ifndef PLAYER_H
#define PLAYER_H

typedef struct jogador
{
    int ponto;
    char nome_usuario[4];
}Jogador;

void rankear(int (*mat)[5], int *nova);
void salvar_rank(int (*mat)[5]);
void recuperar_rank(int (*mat)[5]);
int validador_nome(char *nome, int (*mat)[5], int cont);
void listar_rank(int mat[11][5]);

#endif