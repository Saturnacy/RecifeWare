#include "pegueAsCapivaras.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>

#define TEMPO_PAVIO      8.0f
#define TEMPO_BOMBA_FUSE 1.5f

typedef struct {
    float x, y;
    float tempoAtivo;
    int   ativa;
} Buraco;

void pegueAsCapivaras() {
    int screenWidth=1200;
    int screenHeight=720;

    SetTargetFPS(60);

    int flag=0;
    int frames=120;
    int clicadas=0;
    int totalApareceu=0;
    int frameSpawn=0;
    int intervaloSpawn=80;

    float raioCapivara=45.0f;
    float raioBuraco=50.0f;
    float tempoVisivel=1.2f;
    Buraco buracos[9];
    float inicioX=(screenWidth-3*200.0f)/2.0f+100.0f;
    float inicioY=(screenHeight-3*160.0f)/2.0f+80.0f-30.0f;

    for (int i=0;i<9;i++) {
        buracos[i].x=inicioX+(i%3)*200.0f;
        buracos[i].y=inicioY+(i/3)*160.0f;
        buracos[i].ativa=0;
        buracos[i].tempoAtivo=0.0f;
    }

    TimerBomba timer;
    InitTimerBomba(&timer,TEMPO_PAVIO,TEMPO_BOMBA_FUSE);

    while (timer.fase == FASE_PAVIO && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);

        float dt=GetFrameTime();

        frameSpawn++;
        if (frameSpawn>=intervaloSpawn) {
            frameSpawn=0;

            int tentativas=0;
            int idx=rand()%9;
            while (buracos[idx].ativa && tentativas<9) {
                idx=(idx+1)%9;
                tentativas++;
            }
            if (!buracos[idx].ativa) {
                buracos[idx].ativa=1;
                buracos[idx].tempoAtivo=tempoVisivel;
                totalApareceu++;
            }
        }

        for (int i=0;i<9;i++) {
            if (!buracos[i].ativa) continue;
            buracos[i].tempoAtivo-=dt;
            if (buracos[i].tempoAtivo<=0.0f) {
                buracos[i].ativa=0;
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse=GetMousePosition();
            for (int i=0;i<9;i++) {
                if (!buracos[i].ativa) continue;
                if (CheckCollisionPointCircle(mouse,(Vector2){buracos[i].x,buracos[i].y},raioCapivara)) {
                    buracos[i].ativa=0;
                    clicadas++;
                    break;
                }
            }
        }

        BeginDrawing();
            ClearBackground((Color){180,140,80,255});
            int larguraObj=MeasureText("PEGUE AS CAPIVARAS!",28);
            DrawText("PEGUE AS CAPIVARAS!",(screenWidth/2)-(larguraObj/2),18,28,DARKBROWN);
            DrawText(TextFormat("Capturadas: %d",clicadas),20,18,24,WHITE);

            for (int i=0;i<9;i++) {
                DrawCircle((int)buracos[i].x,(int)buracos[i].y,raioBuraco,DARKBROWN);

                if (buracos[i].ativa) {
                    DrawCircle((int)buracos[i].x,(int)buracos[i].y,raioCapivara,(Color){139,90,43,255});
                    DrawCircle((int)buracos[i].x-13,(int)buracos[i].y-10,7,BLACK);
                    DrawCircle((int)buracos[i].x+13,(int)buracos[i].y-10,7,BLACK);
                    DrawCircle((int)buracos[i].x-11,(int)buracos[i].y-12,3,WHITE);
                    DrawCircle((int)buracos[i].x+15,(int)buracos[i].y-12,3,WHITE);
                    DrawEllipse((int)buracos[i].x,(int)buracos[i].y+10,16,10,(Color){160,100,50,255});
                    DrawCircle((int)buracos[i].x-6,(int)buracos[i].y+8,3,DARKBROWN);
                    DrawCircle((int)buracos[i].x+6,(int)buracos[i].y+8,3,DARKBROWN);
                }
            }

            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }

    if (totalApareceu>0 && clicadas>totalApareceu/2) {
        flag=1;
    }

    while (!TimerTerminou(&timer) && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);
        BeginDrawing();
            ClearBackground((Color){180,140,80,255});
            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }

    if (flag) {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){180,140,80,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Vitoria",tamanhoFonte);
                DrawText("Vitoria",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,GREEN);
            EndDrawing();
        }
    } else {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){180,140,80,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Derrota",tamanhoFonte);
                DrawText("Derrota",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,RED);
            EndDrawing();
        }
    }

    UnloadTimerBomba(&timer);
}