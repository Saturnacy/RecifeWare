#include "escapeDoTubarao.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <math.h>

#define TEMPO_PAVIO      7.0f
#define TEMPO_BOMBA_FUSE 1.5f

void escapeDoTubarao(int dificuldade) {
    int screenWidth=1200;
    int screenHeight=720;
    SetTargetFPS(60);
    int flag=0;
    int colisao=0;
    int frames=120;
    float arenaSize=500.0f;
    float arenaX=(screenWidth-arenaSize)/2.0f;
    float arenaY=(screenHeight-arenaSize)/2.0f-30.0f;
    float playerX=arenaX+arenaSize/2.0f;
    float playerY=arenaY+arenaSize*0.75f;
    float playerRaio=18.0f;
    float playerVel=4.0f;
    float tubaraoX=arenaX+40.0f;
    float tubaraoY=arenaY+40.0f;
    float tubaraoRaio=25.0f;
    float tubaraoVel=1.5f+(dificuldade-1)*1.5f;
    TimerBomba timer;
    InitTimerBomba(&timer,TEMPO_PAVIO,TEMPO_BOMBA_FUSE);
    while (timer.fase==FASE_PAVIO && !colisao && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);
        if (IsKeyDown(KEY_RIGHT)) playerX+=playerVel;
        if (IsKeyDown(KEY_LEFT)) playerX-=playerVel;
        if (IsKeyDown(KEY_DOWN)) playerY+=playerVel;
        if (IsKeyDown(KEY_UP)) playerY-=playerVel;
        if (playerX-playerRaio<arenaX) playerX=arenaX+playerRaio;
        if (playerX+playerRaio>arenaX+arenaSize) playerX=arenaX+arenaSize-playerRaio;
        if (playerY-playerRaio<arenaY) playerY=arenaY+playerRaio;
        if (playerY+playerRaio>arenaY+arenaSize) playerY=arenaY+arenaSize-playerRaio;
        float dx=playerX-tubaraoX;
        float dy=playerY-tubaraoY;
        float dist=sqrtf(dx*dx+dy*dy);
        if (dist>0) {
            tubaraoX+=(dx/dist)*tubaraoVel;
            tubaraoY+=(dy/dist)*tubaraoVel;
        }
        if (CheckCollisionCircles((Vector2){playerX,playerY},playerRaio,(Vector2){tubaraoX,tubaraoY},tubaraoRaio)) {
            colisao=1;
        }
        BeginDrawing();
            ClearBackground((Color){135,206,235,255});
            DrawRectangle((int)arenaX,(int)arenaY,(int)arenaSize,(int)arenaSize,(Color){30,100,200,255});
            DrawRectangleLines((int)arenaX,(int)arenaY,(int)arenaSize,(int)arenaSize,DARKBLUE);
            int larguraObj=MeasureText("EVITE O TUBARAO!",26);
            DrawText("EVITE O TUBARAO!",(screenWidth/2)-(larguraObj/2),18,26,DARKBLUE);
            DrawCircle((int)tubaraoX,(int)tubaraoY,tubaraoRaio,DARKGRAY);
            DrawCircle((int)playerX,(int)playerY,playerRaio,YELLOW);
            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }
    if (!colisao) {
        flag=1;
        while (!TimerTerminou(&timer) && !WindowShouldClose()) {
            UpdateTimerBomba(&timer);
            BeginDrawing();
                ClearBackground((Color){135,206,235,255});
                DrawRectangle((int)arenaX,(int)arenaY,(int)arenaSize,(int)arenaSize,(Color){30,100,200,255});
                DrawCircle((int)tubaraoX,(int)tubaraoY,tubaraoRaio,DARKGRAY);
                DrawCircle((int)playerX,(int)playerY,playerRaio,YELLOW);
                DrawTimerBomba(&timer,screenWidth,screenHeight);
            EndDrawing();
        }
    }
    if (flag) {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){135,206,235,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Vitoria",tamanhoFonte);
                DrawText("Vitoria",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,GREEN);
            EndDrawing();
        }
    } else {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){135,206,235,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Derrota",tamanhoFonte);
                DrawText("Derrota",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,RED);
            EndDrawing();
        }
    }
    UnloadTimerBomba(&timer);
}