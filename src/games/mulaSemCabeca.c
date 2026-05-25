#include "mulaSemCabeca.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>

#define TEMPO_PAVIO      8.0f
#define TEMPO_BOMBA_FUSE 1.5f

typedef struct {
    float x;
    float y;
} Obstaculo;

void mulaSemCabeca() {
    int screenWidth=1200;
    int screenHeight=720;

    SetTargetFPS(60);

    int flag=0;
    int colisao=0;
    int noChao=1;
    int frames=120;

    float playerY=480.0f;
    float velY=0.0f;
    float chaoY=560.0f;
    float playerW=70.0f;
    float playerH=90.0f;
    float obsW=45.0f;
    float obsH=70.0f;
    float gravidade=0.75f;
    float velObs=6.0f;

    Obstaculo obs[4];
    for (int i=0;i<4;i++) {
        obs[i].x=screenWidth+200+i*320+(rand()%150);
        obs[i].y=chaoY-obsH;
    }

    TimerBomba timer;
    InitTimerBomba(&timer,TEMPO_PAVIO,TEMPO_BOMBA_FUSE);

    while (timer.fase==FASE_PAVIO && !colisao && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);

        if (IsKeyPressed(KEY_SPACE) && noChao) {
            velY=-16.0f;
            noChao=0;
        }

        velY+=gravidade;
        playerY+=velY;

        if (playerY>=chaoY-playerH) {
            playerY=chaoY-playerH;
            velY=0.0f;
            noChao=1;
        }

        for (int i=0;i<4;i++) {
            obs[i].x-=velObs;
            if (obs[i].x<-obsW) {
                obs[i].x=screenWidth+100+(rand()%300);
            }
        }

        Rectangle playerRect={200,playerY,playerW,playerH};
        for (int i=0;i<4;i++) {
            Rectangle obsRect={obs[i].x,obs[i].y,obsW,obsH};
            if (CheckCollisionRecs(playerRect,obsRect)) {
                colisao=1;
                break;
            }
        }

        BeginDrawing();
            ClearBackground((Color){10,10,30,255});
            DrawRectangle(0,(int)chaoY,screenWidth,screenHeight-(int)chaoY,(Color){20,60,20,255});
            DrawRectangle(200,(int)playerY,(int)playerW,(int)playerH,RED);
            for (int i=0;i<4;i++) {
                DrawRectangle((int)obs[i].x,(int)obs[i].y,(int)obsW,(int)obsH,WHITE);
            }
            int larguraPule=MeasureText("PULE!",30);
            DrawText("PULE!",(screenWidth/2)-(larguraPule/2),30,30,YELLOW);
            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }

    if (!colisao) {
        flag=1;
        while (!TimerTerminou(&timer) && !WindowShouldClose()) {
            UpdateTimerBomba(&timer);
            BeginDrawing();
                ClearBackground((Color){10,10,30,255});
                DrawRectangle(0,(int)chaoY,screenWidth,screenHeight-(int)chaoY,(Color){20,60,20,255});
                DrawRectangle(200,(int)playerY,(int)playerW,(int)playerH,RED);
                for (int i=0;i<4;i++) {
                    DrawRectangle((int)obs[i].x,(int)obs[i].y,(int)obsW,(int)obsH,WHITE);
                }
                DrawTimerBomba(&timer,screenWidth,screenHeight);
            EndDrawing();
        }
    }

    if (flag) {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){10,10,30,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Vitoria",tamanhoFonte);
                DrawText("Vitoria",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,GREEN);
            EndDrawing();
        }
    } else {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){10,10,30,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Derrota",tamanhoFonte);
                DrawText("Derrota",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,RED);
            EndDrawing();
        }
    }

    UnloadTimerBomba(&timer);
}