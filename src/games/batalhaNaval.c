#include "batalhaNaval.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define TEMPO_PAVIO 9.0f
#define TEMPO_BOMBA_FUSE 1.5f
#define META_DESTRUIDOS 5
#define MAX_NAVIOS 6
#define MAX_BALAS 10

#define NAVIO_W 120
#define NAVIO_H 50
#define BALA_RAIO 7
#define PLAYER_RAIO 12

#define WATER_Y_MIN 200
#define WATER_Y_MAX 380

typedef struct {
    float x, y;
    float velocidade;
    int hp;
    int ativo;
} Navio;

typedef struct {
    float x, y;
    float vx, vy;
    int ativo;
} Bala;

void batalhaNaval() {
    int screenWidth = 1200;
    int screenHeight = 720;

    SetTargetFPS(60);

    int flag = 0;
    int frames = 120;
    int destruidos = 0;

    float canX = 150.0f;
    float canY = 470.0f;
    float canAngle = -45.0f * DEG2RAD;
    float recarga = 0.0f;

    Navio navios[MAX_NAVIOS] = {0};
    Bala balas[MAX_BALAS] = {0};

    float spawnTimer = 0.0f;
    float spawnIntervalo = 1.2f;

    TimerBomba timer;
    InitTimerBomba(&timer, TEMPO_PAVIO, TEMPO_BOMBA_FUSE);

    while(timer.fase == FASE_PAVIO && destruidos < META_DESTRUIDOS && !WindowShouldClose()){
        UpdateTimerBomba(&timer);
        float dt = GetFrameTime();

        Vector2 mouse = GetMousePosition();
        float dx = mouse.x - canX;
        float dy = mouse.y - canY;
        canAngle = atan2f(dy, dx);
        if(canAngle < -80.0f * DEG2RAD) canAngle = -80.0f * DEG2RAD;
        if(canAngle > -5.0f * DEG2RAD) canAngle = -5.0f * DEG2RAD;

        recarga -= dt;
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && recarga <= 0.0f){
            for(int i = 0; i < MAX_BALAS; i++){
                if(!balas[i].ativo){
                    balas[i].x = canX + cosf(canAngle) * 65.0f;
                    balas[i].y = canY + sinf(canAngle) * 65.0f;
                    balas[i].vx = cosf(canAngle) * 550.0f;
                    balas[i].vy = sinf(canAngle) * 550.0f;
                    balas[i].ativo = 1;
                    recarga = 0.65f;
                    break;
                }
            }
        }

        for(int i = 0; i < MAX_BALAS; i++){
            if(!balas[i].ativo) continue;
            balas[i].x += balas[i].vx * dt;
            balas[i].y += balas[i].vy * dt;
            if(balas[i].x > screenWidth + 20 || balas[i].x < -20 || balas[i].y < -20 || balas[i].y > screenHeight + 20) balas[i].ativo = 0;
        }

        spawnTimer += dt;
        if(spawnTimer >= spawnIntervalo){
            spawnTimer = 0.0f;
            spawnIntervalo = 1.0f - (destruidos * 0.04f);
            if (spawnIntervalo < 0.5f) spawnIntervalo = 0.5f;

            for (int i = 0; i < MAX_NAVIOS; i++) {
                if (!navios[i].ativo) {
                    navios[i].x = (float)screenWidth + 30.0f;
                    navios[i].y = (float)(WATER_Y_MIN + rand() % (WATER_Y_MAX - WATER_Y_MIN));
                    navios[i].velocidade = 160.0f + (float)(rand() % 120);
                    navios[i].hp = 1 + (destruidos / 3);
                    if (navios[i].hp > 3) navios[i].hp = 3;
                    navios[i].ativo = 1;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_NAVIOS; i++) {
            if (!navios[i].ativo) continue;
            navios[i].x -= navios[i].velocidade * dt;
            if (navios[i].x < -(float)NAVIO_W) {
                navios[i].ativo = 0;
                continue;
            }

            Rectangle navRect = {navios[i].x, navios[i].y, NAVIO_W, NAVIO_H};
            for (int j = 0; j < MAX_BALAS; j++) {
                if (!balas[j].ativo) continue;
                if (CheckCollisionPointRec((Vector2){balas[j].x, balas[j].y}, navRect)) {
                    balas[j].ativo = 0;
                    navios[i].hp--;
                    if (navios[i].hp <= 0) {
                        navios[i].ativo = 0;
                        destruidos++;
                    }
                }
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < MAX_NAVIOS; i++) {
                if (!navios[i].ativo) continue;
                DrawRectangle((int)navios[i].x, (int)navios[i].y, NAVIO_W, NAVIO_H, DARKBROWN);
                for (int h = 0; h < navios[i].hp; h++)
                    DrawRectangle((int)navios[i].x + h * 16, (int)navios[i].y - 12, 12, 7, RED);
            }

            for (int i = 0; i < MAX_BALAS; i++) {
                if (!balas[i].ativo) continue;
                DrawCircle((int)balas[i].x, (int)balas[i].y, BALA_RAIO, BLACK);
            }

            float tipX = canX + cosf(canAngle) * 72.0f;
            float tipY = canY + sinf(canAngle) * 72.0f;
            DrawCircle((int)canX, (int)canY, PLAYER_RAIO, DARKGRAY);
            DrawLineEx((Vector2){canX, canY}, (Vector2){tipX, tipY}, 10, GRAY);

            DrawCircleLines((int)mouse.x, (int)mouse.y, 14, RED);
            DrawLine((int)mouse.x - 18, (int)mouse.y, (int)mouse.x + 18, (int)mouse.y, RED);
            DrawLine((int)mouse.x, (int)mouse.y - 18, (int)mouse.x, (int)mouse.y + 18, RED);

            if (recarga > 0.0f) {
                float pct = 1.0f - recarga / 0.65f;
                DrawRectangle((int)canX - 32, (int)canY - 36, 64, 7, LIGHTGRAY);
                DrawRectangle((int)canX - 32, (int)canY - 36, (int)(64 * pct), 7, GOLD);
            }

            DrawText(TextFormat("NAVIOS: %d/%d", destruidos, META_DESTRUIDOS), 20, 20, 24, BLACK);

            DrawTimerBomba(&timer, screenWidth, screenHeight);
        EndDrawing();
    }

    if (destruidos >= META_DESTRUIDOS) flag = 1;

    if (!flag) {
        while (!TimerTerminou(&timer) && !WindowShouldClose()) {
            UpdateTimerBomba(&timer);
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTimerBomba(&timer, screenWidth, screenHeight);
            EndDrawing();
        }
    }

    if (flag) {
        while (frames > 0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground(RAYWHITE);
                int tamanhoFonte = 40;
                int larguraTexto = MeasureText("Vitoria", tamanhoFonte);
                DrawText("Vitoria", (screenWidth / 2) - (larguraTexto / 2),
                         (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, GREEN);
            EndDrawing();
        }
    } else {
        while (frames > 0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground(RAYWHITE);
                int tamanhoFonte = 40;
                int larguraTexto = MeasureText("Derrota", tamanhoFonte);
                DrawText("Derrota", (screenWidth / 2) - (larguraTexto / 2),
                         (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, RED);
            EndDrawing();
        }
    }

    UnloadTimerBomba(&timer);
}