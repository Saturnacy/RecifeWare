#include "crabber.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>

#define TEMPO_PAVIO 10.0f
#define TEMPO_BOMBA_FUSE 1.5f

#define LINHAS_AGUA 4
#define PLAT_POR_LINHA 3

typedef struct {
    float x, y;
    float w, h;
    float vel;
} Plataforma;

void crabber() {
    int screenWidth = 1200;
    int screenHeight = 720;

    SetTargetFPS(60);

    int flag = 0;
    int colisao = 0;
    int frames = 120;

    float margem = 60.0f;
    float areaW = screenWidth - margem * 2.0f;
    float areaH = screenHeight - margem * 2.0f;
    float faixaH = areaH / (LINHAS_AGUA + 2.0f);

    float metaY = margem;
    float metaH = faixaH;
    float chaoY = margem + faixaH * (LINHAS_AGUA + 1.0f);

    float faixaY[LINHAS_AGUA];
    for (int i = 0; i < LINHAS_AGUA; i++)
        faixaY[i] = margem + faixaH * (float)(i + 1);

    float playerW = 44.0f;
    float playerH = 36.0f;
    float playerX = (float)screenWidth / 2.0f - playerW / 2.0f;
    float playerY = chaoY + (faixaH - playerH) / 2.0f;
    float passoV = faixaH;
    float passoH = 60.0f;
    int movCD = 0;

    Plataforma plats[LINHAS_AGUA][PLAT_POR_LINHA];
    for (int l = 0; l < LINHAS_AGUA; l++) {
        float vel = (2.5f + l * 0.8f) * ((l % 2 == 0) ? 1.0f : -1.0f);
        for (int p = 0; p < PLAT_POR_LINHA; p++) {
            plats[l][p].w = 140.0f + (float)(rand() % 80);
            plats[l][p].h = faixaH;
            plats[l][p].x = margem + (float)p * (areaW / PLAT_POR_LINHA) + (float)(l * 97 % 200);
            plats[l][p].y = faixaY[l];
            plats[l][p].vel = vel;
        }
    }

    TimerBomba timer;
    InitTimerBomba(&timer, TEMPO_PAVIO, TEMPO_BOMBA_FUSE);

    while (timer.fase == FASE_PAVIO && !colisao && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);

        for (int l = 0; l < LINHAS_AGUA; l++) {
            for (int p = 0; p < PLAT_POR_LINHA; p++) {
                plats[l][p].x += plats[l][p].vel;
                if (plats[l][p].vel > 0 && plats[l][p].x > (float)screenWidth + 20.0f)
                    plats[l][p].x = -(plats[l][p].w + 20.0f);
                if (plats[l][p].vel < 0 && plats[l][p].x + plats[l][p].w < -20.0f)
                    plats[l][p].x = (float)screenWidth + 20.0f;
            }
        }

        if (movCD > 0) movCD--;
        if (movCD == 0) {
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) { playerY -= passoV; movCD = 10; }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) { playerY += passoV; movCD = 10; }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { playerX -= passoH; movCD = 8; }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { playerX += passoH; movCD = 8; }
        }

        int naPlataforma = 0;
        for (int l = 0; l < LINHAS_AGUA && !naPlataforma; l++) {
            for (int p = 0; p < PLAT_POR_LINHA; p++) {
                Rectangle rPlat = { plats[l][p].x, plats[l][p].y, plats[l][p].w, plats[l][p].h };
                Rectangle rPlay = { playerX, playerY, playerW, playerH };
                if (CheckCollisionRecs(rPlay, rPlat)) {
                    playerX += plats[l][p].vel;
                    naPlataforma = 1;
                    break;
                }
            }
        }

        if (playerX < margem) playerX = margem;
        if (playerX + playerW > margem + areaW) playerX = margem + areaW - playerW;

        if (playerY > chaoY + faixaH - playerH) playerY = chaoY + faixaH - playerH;

        Rectangle rPlayer = { playerX, playerY, playerW, playerH };
        Rectangle rMeta = { margem, metaY, areaW, metaH };
        if (CheckCollisionRecs(rPlayer, rMeta)) {
            flag = 1;
            break;
        }

        float centroY = playerY + playerH / 2.0f;
        int naAgua = 0;
        for (int l = 0; l < LINHAS_AGUA; l++) {
            if (centroY > faixaY[l] && centroY < faixaY[l] + faixaH) {
                naAgua = 1;
                break;
            }
        }
        if (naAgua && !naPlataforma) {
            colisao = 1;
        }

        BeginDrawing();
            ClearBackground((Color){ 20, 80, 20, 255 });

            DrawRectangle((int)margem, (int)metaY, (int)areaW, (int)metaH, (Color){ 30, 160, 30, 255 });
            int lwMeta = MeasureText("CHEGUE AQUI!", 22);
            DrawText("CHEGUE AQUI!", screenWidth / 2 - lwMeta / 2, (int)(metaY + metaH / 2.0f - 11), 22, WHITE);

            for (int l = 0; l < LINHAS_AGUA; l++) {
                DrawRectangle((int)margem, (int)faixaY[l], (int)areaW, (int)faixaH, (Color){ 20, 80, 180, 255 });
                for (float wx = margem + 20; wx < margem + areaW - 20; wx += 50) {
                    DrawCircle((int)wx, (int)(faixaY[l] + faixaH / 2.0f), 5, (Color){ 50, 120, 220, 255 });
                }
            }

            for (int l = 0; l < LINHAS_AGUA; l++) {
                Color corPlat = (l % 2 == 0) ? (Color){ 120, 72, 20, 255 } : (Color){ 90, 90, 90, 255 };
                for (int p = 0; p < PLAT_POR_LINHA; p++) {
                    DrawRectangleRounded((Rectangle){ plats[l][p].x, plats[l][p].y + 4, plats[l][p].w, plats[l][p].h - 8 }, 0.35f, 6, corPlat);
                    DrawRectangleRoundedLines((Rectangle){ plats[l][p].x, plats[l][p].y + 4, plats[l][p].w, plats[l][p].h - 8 }, 0.35f, 6, DARKBROWN);
                }
            }

            DrawRectangle((int)margem, (int)chaoY, (int)areaW, (int)faixaH, (Color){ 30, 120, 30, 255 });

            Color corC = (Color){ 220, 60, 30, 255 };
            DrawEllipse((int)(playerX + playerW / 2.0f), (int)(playerY + playerH / 2.0f), (int)(playerW / 2.0f), (int)(playerH / 2.5f), corC);
            DrawCircle((int)(playerX - 5), (int)(playerY + playerH / 2.0f), 9, corC);
            DrawCircle((int)(playerX + playerW + 5), (int)(playerY + playerH / 2.0f), 9, corC);
            DrawCircle((int)(playerX + playerW * 0.33f), (int)(playerY + playerH * 0.28f), 4, WHITE);
            DrawCircle((int)(playerX + playerW * 0.67f), (int)(playerY + playerH * 0.28f), 4, WHITE);
            DrawCircle((int)(playerX + playerW * 0.33f), (int)(playerY + playerH * 0.28f), 2, BLACK);
            DrawCircle((int)(playerX + playerW * 0.67f), (int)(playerY + playerH * 0.28f), 2, BLACK);

            int lwT = MeasureText("CRABBER - ATRAVESSE O RIO!", 26);
            DrawText("CRABBER - ATRAVESSE O RIO!", screenWidth / 2 - lwT / 2, 14, 26, YELLOW);

            DrawTimerBomba(&timer, screenWidth, screenHeight);
        EndDrawing();
    }

    if (!flag && !colisao) {
        while (!TimerTerminou(&timer) && !WindowShouldClose()) {
            UpdateTimerBomba(&timer);
            BeginDrawing();
                ClearBackground((Color){ 20, 80, 20, 255 });
                DrawTimerBomba(&timer, screenWidth, screenHeight);
            EndDrawing();
        }
    }

    if (flag) {
        while (frames > 0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){ 20, 80, 20, 255 });
                int tamanhoFonte = 40;
                int larguraTexto = MeasureText("Vitoria", tamanhoFonte);
                DrawText("Vitoria", (screenWidth / 2) - (larguraTexto / 2), (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, GREEN);
            EndDrawing();
        }
    } else {
        while (frames > 0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){ 20, 80, 20, 255 });
                int tamanhoFonte = 40;
                int larguraTexto = MeasureText("Derrota", tamanhoFonte);
                DrawText("Derrota", (screenWidth / 2) - (larguraTexto / 2), (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, RED);
            EndDrawing();
        }
    }

    UnloadTimerBomba(&timer);
}