#ifndef TIMER_BOMBA_H
#define TIMER_BOMBA_H

#include "raylib.h"

#define BOMBA_FRAMES 4
#define PAVIO_FRAMES 2

typedef enum {
    FASE_PAVIO, 
    FASE_BOMBA_PAVIO, 
    FASE_EXPLOSAO 
} FaseTimer;

typedef struct {
    Texture2D bombaTex;
    Texture2D pavioTex;
    int bombaFrameWidth;
    int pavioFrameWidth;
    float tempoTotal;
    float tempoRestante;
    float tempoBombaFuse; 
    int bombaFrameAtual;
    int animFrameCounter;
    FaseTimer  fase;
} TimerBomba;

void InitTimerBomba(TimerBomba *timer, float tempoTotal, float tempoBombaFuse);
void UpdateTimerBomba(TimerBomba *timer);
void DrawTimerBomba(TimerBomba *timer, int screenWidth, int screenHeight);
void UnloadTimerBomba(TimerBomba *timer);

int TimerTerminou(TimerBomba *timer);

#endif