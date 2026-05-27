#include "timer_bomba.h"
#include "raylib.h"

#define BOMBA_SCALE 3.0f
#define PAVIO_SCALE 2.0f
#define BOMBA_VISIBLE_TOP 31 
#define BOMBA_VISIBLE_BOT 63 
#define PAVIO_CENTER_Y 23 

void InitTimerBomba(TimerBomba *timer, float tempoTotal, float tempoBombaFuse) {
    timer->bombaTex = LoadTexture("assets/sprites/Bomba-Sheet.png");
    timer->pavioTex = LoadTexture("assets/sprites/Pavio-Sheet.png");

    timer->bombaFrameWidth = timer->bombaTex.width / BOMBA_FRAMES; 
    timer->pavioFrameWidth = timer->pavioTex.width / PAVIO_FRAMES; 

    timer->tempoTotal = tempoTotal;
    timer->tempoRestante = tempoTotal;
    timer->tempoBombaFuse = tempoBombaFuse;
    timer->bombaFrameAtual = 0;
    timer->animFrameCounter = 0;
    timer->fase = FASE_PAVIO;
}

void UpdateTimerBomba(TimerBomba *timer) {
    timer->animFrameCounter++;

    switch (timer->fase) {

        case FASE_PAVIO:
            timer->tempoRestante -= GetFrameTime();
            if (timer->tempoRestante <= 0.0f) {
                timer->tempoRestante = timer->tempoBombaFuse; 
                timer->bombaFrameAtual = 0;
                timer->animFrameCounter = 0;
                timer->fase = FASE_BOMBA_PAVIO;
            }
            break;

        case FASE_BOMBA_PAVIO:
            if (timer->animFrameCounter >= 6) {
                timer->animFrameCounter = 0;
                if (timer->bombaFrameAtual < 1) {
                    timer->bombaFrameAtual = 1;
                }
            }
            
            timer->tempoRestante -= GetFrameTime();
            if (timer->tempoRestante <= 0.0f) {
                timer->tempoRestante = 0.0f;
                timer->bombaFrameAtual = 2; 
                timer->animFrameCounter = 0;
                timer->fase = FASE_EXPLOSAO;
            }
            break;

        case FASE_EXPLOSAO:
            if (timer->animFrameCounter >= 10) {
                timer->animFrameCounter = 0;
                if (timer->bombaFrameAtual < BOMBA_FRAMES - 1)
                    timer->bombaFrameAtual++;
            }
            break;
    }
}

void DrawTimerBomba(TimerBomba *timer, int screenWidth, int screenHeight) {
    const float MARGEM_BOT = 6.0f;

    float bombaW = timer->bombaFrameWidth * (BOMBA_SCALE - 1); 
    float bombaH = timer->bombaTex.height * (BOMBA_SCALE - 1);  

    float bombaY = (float)screenHeight - MARGEM_BOT - BOMBA_VISIBLE_BOT * (BOMBA_SCALE - 1);
    float bombaX = 8.0f;

    float fuseLineY = bombaY + BOMBA_VISIBLE_TOP * BOMBA_SCALE * 1.18f;

    float fuseStartX = bombaX + bombaW;
    float fuseEndX = (float)screenWidth - 8.0f;
    float fuseMaxW = fuseEndX - fuseStartX;

    float pavioW = timer->pavioFrameWidth * PAVIO_SCALE;   
    float pavioH = timer->pavioTex.height * PAVIO_SCALE;   

    float pavioY = fuseLineY - PAVIO_CENTER_Y * PAVIO_SCALE;

    if (timer->fase == FASE_PAVIO) {
        int totalPavios = (int)(fuseMaxW / pavioW);
        float ratio = timer->tempoRestante / timer->tempoTotal;
        int paviosAtivos = (int)(totalPavios * ratio + 0.5f);
        if (paviosAtivos > totalPavios) paviosAtivos = totalPavios;

        for (int i = 0; i < paviosAtivos - 1; i++) {
            Rectangle srcFio = {
                0.0f, 0.0f, 
                (float)timer->pavioFrameWidth,
                (float)timer->pavioTex.height
            };
            Rectangle dstFio = {
                fuseStartX + i * pavioW, pavioY,
                pavioW, pavioH
            };
            DrawTexturePro(timer->pavioTex, srcFio, dstFio, (Vector2){0,0}, 0.0f, WHITE);
        }

        if (paviosAtivos > 0) {
            Rectangle srcFaisca = {
                (float)(1 * timer->pavioFrameWidth), 0.0f,
                (float)timer->pavioFrameWidth,
                (float)timer->pavioTex.height
            };
            
            Rectangle dstFaisca = {
                fuseStartX + (paviosAtivos - 1) * pavioW, pavioY,
                pavioW, pavioH
            };
            DrawTexturePro(timer->pavioTex, srcFaisca, dstFaisca, (Vector2){0,0}, 0.0f, WHITE);
        }
    }

    Rectangle srcBomba = {(float)(timer->bombaFrameAtual * timer->bombaFrameWidth), 0.0f, (float)timer->bombaFrameWidth, (float)timer->bombaTex.height};
    Rectangle dstBomba = { bombaX, bombaY, bombaW, bombaH };
    DrawTexturePro(timer->bombaTex, srcBomba, dstBomba, (Vector2){0,0}, 0.0f, WHITE);
}

int TimerTerminou(TimerBomba *timer) {
    return (timer->fase == FASE_EXPLOSAO && timer->bombaFrameAtual == BOMBA_FRAMES - 1);
}

void UnloadTimerBomba(TimerBomba *timer) {
    UnloadTexture(timer->bombaTex);
    UnloadTexture(timer->pavioTex);
}