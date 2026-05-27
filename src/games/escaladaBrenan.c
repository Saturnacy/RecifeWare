#include "player.h"
#include "escaladaBrenan.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer_bomba.h"

#define TEMPO_PAVIO      5.0f
#define TEMPO_BOMBA_FUSE 1.5f
#define FRAME_W_RUN  17
#define FRAME_H_RUN  33
#define FRAME_W_WIN  17
#define FRAME_H_WIN  20

void escaladaBrenan() {
    int screenWidth=1200;
    int screenHeight=720;
    SetTargetFPS(60);
    int y=500;
    int altura=0;
    int flag=0;
    int frames=300;
    int frameAtual=0;
    float PLAYERscale=4.0f;
    Texture2D background=LoadTexture("../assets/sprites/escaleBrenan/bgobren.png");
    Texture2D texRun=LoadTexture("../assets/sprites/escaleBrenan/mouse.png");
    Texture2D texWin=LoadTexture("../assets/sprites/escaleBrenan/mouse_win.png");
    Rectangle srcFundo={0,0,(float)background.width,(float)background.height};
    Rectangle dstFundo={0,0,(float)screenWidth,(float)screenHeight};
    TimerBomba timer;
    InitTimerBomba(&timer,TEMPO_PAVIO,TEMPO_BOMBA_FUSE);
    while (!TimerTerminou(&timer) && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);
        if (IsKeyPressed(KEY_SPACE)) {
            y-=15;
            altura+=5;
            frameAtual=(frameAtual+1)%2;
        }
        if (altura>=135) {
            flag=1;
            break;
        }
        frames--;
        Rectangle srcPlayer={(float)(frameAtual*FRAME_W_RUN),0.0f,(float)FRAME_W_RUN,(float)FRAME_H_RUN};
        Rectangle dstPlayer={(screenWidth/2.0f)-(FRAME_W_RUN*PLAYERscale/2.0f),(float)y,FRAME_W_RUN*PLAYERscale,FRAME_H_RUN*PLAYERscale};
        BeginDrawing();
            DrawTexturePro(background,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
            DrawTexturePro(texRun,srcPlayer,dstPlayer,(Vector2){0,0},0.0f,WHITE);
            DrawText(TextFormat("Altura: %d/135",altura),20,20,20,RED);
            DrawText(TextFormat("Tempo Restante: %d",frames/60),20,50,20,RED);
            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }
    if (flag) {
        int winFrames=120;
        Rectangle srcWin={0,0,(float)FRAME_W_WIN,(float)FRAME_H_WIN};
        Rectangle dstWin={(screenWidth/2.0f)-(FRAME_W_WIN*PLAYERscale/2.0f),(float)y,FRAME_W_WIN*PLAYERscale,FRAME_H_WIN*PLAYERscale};
        while (winFrames>0 && !WindowShouldClose()) {
            winFrames--;
            BeginDrawing();
                DrawTexturePro(background,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
                DrawTexturePro(texWin,srcWin,dstWin,(Vector2){0,0},0.0f,WHITE);
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Vitoria",tamanhoFonte);
                DrawText("Vitoria",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,GREEN);
            EndDrawing();
        }
    } else {
        int loseFrames=120;
        Rectangle srcPlayer={0.0f,0.0f,(float)FRAME_W_RUN,(float)FRAME_H_RUN};
        Rectangle dstPlayer={(screenWidth/2.0f)-(FRAME_W_RUN*PLAYERscale/2.0f),(float)y,FRAME_W_RUN*PLAYERscale,FRAME_H_RUN*PLAYERscale};
        while (loseFrames>0 && !WindowShouldClose()) {
            loseFrames--;
            BeginDrawing();
                DrawTexturePro(background,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
                DrawTexturePro(texRun,srcPlayer,dstPlayer,(Vector2){0,0},0.0f,WHITE);
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Derrota",tamanhoFonte);
                DrawText("Derrota",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,RED);
            EndDrawing();
        }
    }
    UnloadTimerBomba(&timer);
    UnloadTexture(background);
    UnloadTexture(texRun);
    UnloadTexture(texWin);
}