#include "player.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int validate_name(char *nome, int mat[11][5], int cont){
    if(cont<3) return -1;

    for(int i=0;i<10;i++){
        int cont=0;
        for(int j=0;j<3;j++){
            if(mat[i][j]!=(int)nome[j]){
                break;
            }
            cont++;
        }
        if(cont==3){
            return 0;
        }
    }
    return 1;

}

void update_ranking(int mat[11][5], int *nova) {
    for(int i=0;i<10;i++){
        if(mat[i][4]<nova[4]){
            for (int j = 9;j >=i; j--) {
                for(int x=0;x<5;x++){
                    mat[j+1][x]=mat[j][x];
                }
            }
            for(int j=0;j<5;j++){
                mat[i][j]=nova[j];
            }
            return;
        }
    }
}

void display_ranking(int mat[11][5]) {
    int tableX = 50;
    int tableY = 50;
    int cellW = 200;  
    int cellH = 40;

    char texto[64];

    for (int r = 0; r < 11; r++) {

        Rectangle cell = (Rectangle) {
            tableX,
            tableY + r * cellH,
            cellW,
            cellH
        };

        DrawRectangleLinesEx(cell, 1, RED);

        snprintf(texto, sizeof(texto),"%c%c%c  |  %d   |  %d",
            (char)mat[r][0],(char)mat[r][1],(char)mat[r][2],mat[r][3],mat[r][4]);
        DrawText(texto, cell.x + 10, cell.y + 10, 20, RED);
    }
}

void save_ranking(int mat[11][5]){
    FILE *rank=fopen("rank.txt", "w+");
    for(int i=0;i<11;i++){
        for(int j=0;j<5;j++){
            fprintf(rank, "%d ", mat[i][j]);
        }
        fprintf(rank, "\n");
    }
    fclose(rank);
}
void restore_ranking(int mat[11][5]){
    FILE *rank=fopen("rank.txt", "r");
    for(int i=0;i<11;i++){
        for(int j=0;j<5;j++){
            fscanf(rank, "%d", &mat[i][j]);
        }
    }
    fclose(rank);
}