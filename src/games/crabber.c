#include "crabber.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>

#define TEMPO_PAVIO 10.0f
#define TEMPO_BOMBA_FUSE 1.5f
#define LINHAS_AGUA 4
#define PLAT_POR_LINHA 3

typedef struct
{
    float x, y;
    float w, h;
    float vel;
} Plataforma;

static void DrawTexturaPlat(Texture2D tex, float x, float y, float w, float h)
{
    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst = {x, y, w, h};
    DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

void crabber()
{
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
    float playerH = 44.0f;
    float playerX = (float)screenWidth / 2.0f - playerW / 2.0f;
    float playerY = chaoY + (faixaH - playerH) / 2.0f;
    float passoV = faixaH;
    float passoH = 60.0f;
    int movCD = 0;
    int animFrame = 0;
    int animTick = 0;
    Plataforma plats[LINHAS_AGUA][PLAT_POR_LINHA];
    for (int l = 0; l < LINHAS_AGUA; l++)
    {
        float vel = (2.5f + l * 0.8f) * ((l % 2 == 0) ? 1.0f : -1.0f);
        for (int p = 0; p < PLAT_POR_LINHA; p++)
        {
            plats[l][p].w = 140.0f + (float)(rand() % 80);
            plats[l][p].h = faixaH;
            plats[l][p].x = margem + (float)p * (areaW / PLAT_POR_LINHA) + (float)(l * 97 % 200);
            plats[l][p].y = faixaY[l];
            plats[l][p].vel = vel;
        }
    }
    Texture2D texBackground = LoadTexture("../assets/sprites/crabber/crabber_background.png");
    Texture2D texObjRio = LoadTexture("../assets/sprites/crabber/obj_rio.png");
    Texture2D texCrab[3];
    texCrab[0] = LoadTexture("../assets/sprites/crabber/crab1.png");
    texCrab[1] = LoadTexture("../assets/sprites/crabber/crab2.png");
    texCrab[2] = LoadTexture("../assets/sprites/crabber/crab3.png");
    Rectangle srcBackground = {0, 0, (float)texBackground.width, (float)texBackground.height};
    Rectangle dstBackground = {0, 0, (float)screenWidth, (float)screenHeight};
    Rectangle srcCrab = {0, 0, 32, 32};
    Vector2 orig0 = {0.0f, 0.0f};
    TimerBomba timer;
    InitTimerBomba(&timer, TEMPO_PAVIO, TEMPO_BOMBA_FUSE);
    while (timer.fase == FASE_PAVIO && !colisao && !WindowShouldClose())
    {
        UpdateTimerBomba(&timer);
        animTick++;
        if (animTick >= 10)
        {
            animTick = 0;
            animFrame = (animFrame + 1) % 3;
        }
        for (int l = 0; l < LINHAS_AGUA; l++)
        {
            for (int p = 0; p < PLAT_POR_LINHA; p++)
            {
                plats[l][p].x += plats[l][p].vel;
                if (plats[l][p].vel > 0 && plats[l][p].x > (float)screenWidth + 20.0f)
                    plats[l][p].x = -(plats[l][p].w + 20.0f);
                if (plats[l][p].vel < 0 && plats[l][p].x + plats[l][p].w < -20.0f)
                    plats[l][p].x = (float)screenWidth + 20.0f;
            }
        }
        if (movCD > 0)
            movCD--;
        if (movCD == 0)
        {
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            {
                playerY -= passoV;
                movCD = 10;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            {
                playerY += passoV;
                movCD = 10;
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            {
                playerX -= passoH;
                movCD = 8;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            {
                playerX += passoH;
                movCD = 8;
            }
        }
        int naPlataforma = 0;
        for (int l = 0; l < LINHAS_AGUA && !naPlataforma; l++)
        {
            for (int p = 0; p < PLAT_POR_LINHA; p++)
            {
                float reduzirW = plats[l][p].w * 0.15f;
                Rectangle rPlat = {plats[l][p].x + reduzirW / 2.0f, plats[l][p].y, plats[l][p].w - reduzirW, plats[l][p].h};
                Rectangle rPlay = {playerX, playerY, playerW, playerH};
                if (CheckCollisionRecs(rPlay, rPlat))
                {
                    playerX += plats[l][p].vel;
                    naPlataforma = 1;
                    break;
                }
            }
        }
        if (playerX < margem)
            playerX = margem;
        if (playerX + playerW > margem + areaW)
            playerX = margem + areaW - playerW;
        if (playerY > chaoY + faixaH - playerH)
            playerY = chaoY + faixaH - playerH;
        Rectangle rPlayer = {playerX, playerY, playerW, playerH};
        Rectangle rMeta = {margem, metaY, areaW, metaH};
        if (CheckCollisionRecs(rPlayer, rMeta))
        {
            flag = 1;
            break;
        }
        float centroY = playerY + playerH / 2.0f;
        int naAgua = 0;
        for (int l = 0; l < LINHAS_AGUA; l++)
        {
            if (centroY > faixaY[l] && centroY < faixaY[l] + faixaH)
            {
                naAgua = 1;
                break;
            }
        }
        if (naAgua && !naPlataforma)
            colisao = 1;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(texBackground, srcBackground, dstBackground, orig0, 0.0f, WHITE);
        for (int l = 0; l < LINHAS_AGUA; l++)
        {
            for (int p = 0; p < PLAT_POR_LINHA; p++)
            {
                DrawTexturaPlat(texObjRio, plats[l][p].x, plats[l][p].y, plats[l][p].w, plats[l][p].h);
            }
        }
        Rectangle dstCrab = {playerX, playerY, playerW, playerH};
        DrawTexturePro(texCrab[animFrame], srcCrab, dstCrab, orig0, 0.0f, WHITE);
        int lwMeta = MeasureText("CHEGUE AQUI!", 22);
        DrawText("CHEGUE AQUI!", screenWidth / 2 - lwMeta / 2, (int)(metaY + metaH / 2.0f - 11), 22, WHITE);
        int lwT = MeasureText("CRABBER - ATRAVESSE O RIO!", 26);
        DrawText("CRABBER - ATRAVESSE O RIO!", screenWidth / 2 - lwT / 2, 14, 26, YELLOW);
        DrawTimerBomba(&timer, screenWidth, screenHeight);
        EndDrawing();
    }
    if (!flag && !colisao)
    {
        while (!TimerTerminou(&timer) && !WindowShouldClose())
        {
            UpdateTimerBomba(&timer);
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(texBackground, srcBackground, dstBackground, orig0, 0.0f, WHITE);
            DrawTimerBomba(&timer, screenWidth, screenHeight);
            EndDrawing();
        }
    }
    if (flag)
    {
        while (frames > 0 && !WindowShouldClose())
        {
            frames--;
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(texBackground, srcBackground, dstBackground, orig0, 0.0f, WHITE);
            int sz = 40;
            int lw = MeasureText("Vitoria", sz);
            DrawText("Vitoria", (screenWidth / 2) - (lw / 2), (screenHeight / 2) - (sz / 2), sz, GREEN);
            EndDrawing();
        }
    }
    else
    {
        while (frames > 0 && !WindowShouldClose())
        {
            frames--;
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(texBackground, srcBackground, dstBackground, orig0, 0.0f, WHITE);
            int sz = 40;
            int lw = MeasureText("Derrota", sz);
            DrawText("Derrota", (screenWidth / 2) - (lw / 2), (screenHeight / 2) - (sz / 2), sz, RED);
            EndDrawing();
        }
    }
    UnloadTexture(texBackground);
    UnloadTexture(texObjRio);
    for (int i = 0; i < 3; i++)
        UnloadTexture(texCrab[i]);
    UnloadTimerBomba(&timer);
}