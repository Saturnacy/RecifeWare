#include "escapeDoTubarao.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <math.h>

#define TEMPO_PAVIO 7.0f
#define TEMPO_BOMBA_FUSE 1.5f

void escapeDoTubarao(int dificuldade)
{
    Texture2D borderGame = LoadTexture("assets/sprites/border_game.png");
    int screenWidth = 1200;
    int screenHeight = 720;
    SetTargetFPS(60);
    int flag = 0;
    int colisao = 0;
    int frames = 120;
    float arenaSizeW = 1200.0f;
    float arenaSizeH = 720.0f;
    float arenaX = 0.0f;
    float arenaY = 0.0f;
    float playerX = arenaSizeW / 2.0f;
    float playerY = arenaSizeH * 0.75f;
    float playerRaio = 18.0f;
    float playerVel = 4.0f;
    float tubaraoX = 40.0f;
    float tubaraoY = 40.0f;
    float tubaraoRaio = 25.0f;
    float tubaraoVel = 1.5f + (dificuldade - 1) * 1.5f;
    Texture2D background = LoadTexture("assets/sprites/escapedoTubarao/fuja_tubarao_background.png");
    Texture2D texSeverino = LoadTexture("assets/sprites/escapedoTubarao/severino.png");
    Texture2D texShark = LoadTexture("assets/sprites/escapedoTubarao/shark.png");
    Texture2D texSeta = LoadTexture("assets/sprites/escapedoTubarao/seta.png");
    float sevW = texSeverino.width * 1.8f;
    float sevH = texSeverino.height * 1.8f;
    float shrW = texShark.width * 2.0f;
    float shrH = texShark.height * 2.0f;
    float setW = texSeta.width * 1.0f;
    float setH = texSeta.height * 1.0f;
    Rectangle srcBg = {0, 0, (float)background.width, (float)background.height};
    Rectangle srcSev = {0, 0, (float)texSeverino.width, (float)texSeverino.height};
    Rectangle srcShr = {0, 0, (float)texShark.width, (float)texShark.height};
    Rectangle srcSet = {0, 0, (float)texSeta.width, (float)texSeta.height};
    Rectangle dstBg = {0, 0, (float)screenWidth, (float)screenHeight};
    Vector2 orig0 = {0.0f, 0.0f};
    Vector2 origSet = {setW / 2.0f, setH / 2.0f};
    TimerBomba timer;
    InitTimerBomba(&timer, TEMPO_PAVIO, TEMPO_BOMBA_FUSE);
    while (timer.fase == FASE_PAVIO && !colisao && !WindowShouldClose())
    {
        UpdateTimerBomba(&timer);
        if (IsKeyDown(KEY_RIGHT))
            playerX += playerVel;
        if (IsKeyDown(KEY_LEFT))
            playerX -= playerVel;
        if (IsKeyDown(KEY_DOWN))
            playerY += playerVel;
        if (IsKeyDown(KEY_UP))
            playerY -= playerVel;
        if (playerX - playerRaio < arenaX)
            playerX = arenaX + playerRaio;
        if (playerX + playerRaio > arenaX + arenaSizeW)
            playerX = arenaX + arenaSizeW - playerRaio;
        if (playerY - playerRaio < arenaY)
            playerY = arenaY + playerRaio;
        if (playerY + playerRaio > arenaY + arenaSizeH)
            playerY = arenaY + arenaSizeH - playerRaio;
        float dx = playerX - tubaraoX;
        float dy = playerY - tubaraoY;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist > 0)
        {
            tubaraoX += (dx / dist) * tubaraoVel;
            tubaraoY += (dy / dist) * tubaraoVel;
        }
        if (CheckCollisionCircles((Vector2){playerX, playerY}, playerRaio, (Vector2){tubaraoX, tubaraoY}, tubaraoRaio))
            colisao = 1;
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawTexturePro(background, srcBg, dstBg, orig0, 0.0f, WHITE);
        DrawTexturePro(texSeta, srcSet, (Rectangle){arenaX + arenaSizeW / 2.0f, arenaY + setH / 2.0f, setW, setH}, origSet, 180.0f, WHITE);
        DrawTexturePro(texSeta, srcSet, (Rectangle){arenaX + arenaSizeW / 2.0f, arenaY + arenaSizeH - setH / 2.0f, setW, setH}, origSet, 0.0f, WHITE);
        DrawTexturePro(texSeta, srcSet, (Rectangle){arenaX + setW / 2.0f, arenaY + arenaSizeH / 2.0f, setW, setH}, origSet, 90.0f, WHITE);
        DrawTexturePro(texSeta, srcSet, (Rectangle){arenaX + arenaSizeW - setW / 2.0f, arenaY + arenaSizeH / 2.0f, setW, setH}, origSet, 270.0f, WHITE);
        DrawTexturePro(texShark, srcShr, (Rectangle){tubaraoX - shrW / 2.0f, tubaraoY - shrH / 2.0f, shrW, shrH}, orig0, 0.0f, WHITE);
        DrawTexturePro(texSeverino, srcSev, (Rectangle){playerX - sevW / 2.0f, playerY - sevH / 2.0f, sevW, sevH}, orig0, 0.0f, WHITE);
        int larguraObj = MeasureText("EVITE O TUBARAO!", 26);
        DrawText("EVITE O TUBARAO!", (screenWidth / 2) - (larguraObj / 2), 18, 26, WHITE);
        DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
        DrawTimerBomba(&timer, screenWidth, screenHeight);
        EndDrawing();
    }
    if (!colisao)
    {
        flag = 1;
        while (!TimerTerminou(&timer) && !WindowShouldClose())
        {
            UpdateTimerBomba(&timer);
            BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawTexturePro(background, srcBg, dstBg, orig0, 0.0f, WHITE);
            DrawTimerBomba(&timer, screenWidth, screenHeight);
            DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
            EndDrawing();
        }
    }
    if (flag)
    {
        while (frames > 0 && !WindowShouldClose())
        {
            frames--;
            BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawTexturePro(background, srcBg, dstBg, orig0, 0.0f, WHITE);
            int tamanhoFonte = 40;
            int larguraTexto = MeasureText("Vitoria", tamanhoFonte);
            DrawText("Vitoria", (screenWidth / 2) - (larguraTexto / 2), (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, GREEN);
            DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
            EndDrawing();
        }
    }
    else
    {
        while (frames > 0 && !WindowShouldClose())
        {
            frames--;
            BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawTexturePro(background, srcBg, dstBg, orig0, 0.0f, WHITE);
            int tamanhoFonte = 40;
            int larguraTexto = MeasureText("Derrota", tamanhoFonte);
            DrawText("Derrota", (screenWidth / 2) - (larguraTexto / 2), (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, RED);
            DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
            EndDrawing();
        }
    }
    UnloadTexture(borderGame);
    UnloadTexture(background);
    UnloadTexture(texSeverino);
    UnloadTexture(texShark);
    UnloadTexture(texSeta);
    UnloadTimerBomba(&timer);
}