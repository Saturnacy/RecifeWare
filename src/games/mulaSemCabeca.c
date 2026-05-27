#include "mulaSemCabeca.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>

#define TEMPO_PAVIO 8.0f
#define TEMPO_BOMBA_FUSE 1.5f
#define SPR "../assets/sprites/mulaSemCabeca/"
#define MULA_SCALE 2.0f
#define OBS_SCALE 3.0f
#define NUM_OBS 4
#define CHAO_Y 592.0f

typedef struct
{
    float x, y;
} Obstaculo;

static void DrawBgScroll(Texture2D bg, float offsetX, int sw, int sh)
{
    Rectangle src = {0, 0, (float)bg.width, (float)bg.height};
    float w = (float)sw;
    DrawTexturePro(bg, src, (Rectangle){offsetX, 0, w, (float)sh}, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(bg, src, (Rectangle){offsetX + w, 0, w, (float)sh}, (Vector2){0, 0}, 0.0f, WHITE);
}

static void DrawSprite(Texture2D tex, float x, float y, float scale)
{
    DrawTexturePro(tex, (Rectangle){0, 0, (float)tex.width, (float)tex.height}, (Rectangle){x, y, tex.width * scale, tex.height * scale}, (Vector2){0, 0}, 0.0f, WHITE);
}

void mulaSemCabeca(void)
{
    Texture2D borderGame = LoadTexture("../assets/sprites/border_game.png");
    int screenWidth = 1200;
    int screenHeight = 720;
    SetTargetFPS(60);
    int flag = 0;
    int colisao = 0;
    int noChao = 1;
    int frames = 120;
    float bgX = 0.0f;
    float bgVel = 3.0f;
    Texture2D background = LoadTexture(SPR "mula_background.png");
    Texture2D mulaFrames[4] = {
        LoadTexture(SPR "mula.png"),
        LoadTexture(SPR "mula1.png"),
        LoadTexture(SPR "mula2.png"),
        LoadTexture(SPR "mula3.png"),
    };
    Texture2D obsTextures[NUM_OBS] = {
        LoadTexture(SPR "obj_rio.png"),
        LoadTexture(SPR "obj_rio (1).png"),
        LoadTexture(SPR "obj_rio (2).png"),
        LoadTexture(SPR "obj_rio (3).png"),
    };
    float mulaW = mulaFrames[0].width * MULA_SCALE;
    float mulaH = mulaFrames[0].height * MULA_SCALE;
    float playerX = 180.0f;
    float playerY = CHAO_Y - mulaH;
    float velY = 0.0f;
    float gravidade = 0.75f;
    float velObs = 6.0f;
    int frameAtual = 0;
    int frameCounter = 0;
    int frameSpeed = 8;
    Obstaculo obs[NUM_OBS];
    int obsTipo[NUM_OBS];
    for (int i = 0; i < NUM_OBS; i++)
    {
        int t = i % NUM_OBS;
        float obsH = obsTextures[t].height * OBS_SCALE;
        obs[i].x = screenWidth + 200 + i * 350 + (rand() % 150);
        obs[i].y = CHAO_Y - obsH;
        obsTipo[i] = t;
    }
    TimerBomba timer;
    InitTimerBomba(&timer, TEMPO_PAVIO, TEMPO_BOMBA_FUSE);
    while (timer.fase == FASE_PAVIO && !colisao && !WindowShouldClose())
    {
        UpdateTimerBomba(&timer);
        bgX -= bgVel;
        if (bgX <= -(float)screenWidth)
            bgX = 0.0f;
        if (IsKeyPressed(KEY_SPACE) && noChao)
        {
            velY = -18.0f;
            noChao = 0;
        }
        velY += gravidade;
        playerY += velY;
        if (playerY >= CHAO_Y - mulaH)
        {
            playerY = CHAO_Y - mulaH;
            velY = 0.0f;
            noChao = 1;
        }
        if (++frameCounter >= frameSpeed)
        {
            frameCounter = 0;
            frameAtual = (frameAtual + 1) % 4;
        }
        for (int i = 0; i < NUM_OBS; i++)
        {
            int t = obsTipo[i];
            float obsW = obsTextures[t].width * OBS_SCALE;
            obs[i].x -= velObs;
            if (obs[i].x < -obsW)
            {
                obsTipo[i] = rand() % NUM_OBS;
                t = obsTipo[i];
                float obsH = obsTextures[t].height * OBS_SCALE;
                obs[i].x = screenWidth + 100 + (rand() % 300);
                obs[i].y = CHAO_Y - obsH;
            }
        }
        Rectangle playerRect = {playerX, playerY, mulaW, mulaH};
        for (int i = 0; i < NUM_OBS; i++)
        {
            int t = obsTipo[i];
            float obsW = obsTextures[t].width * OBS_SCALE;
            float obsH = obsTextures[t].height * OBS_SCALE;
            float reduzirW = obsW * 0.25f;
            float reduzirH = obsH * 0.15f;
            Rectangle obsRect = {obs[i].x + reduzirW / 2.0f, obs[i].y + reduzirH, obsW - reduzirW, obsH - reduzirH};
            if (CheckCollisionRecs(playerRect, obsRect))
            {
                colisao = 1;
                break;
            }
        }
        BeginDrawing();
        DrawBgScroll(background, bgX, screenWidth, screenHeight);
        DrawSprite(mulaFrames[frameAtual], playerX, playerY, MULA_SCALE);
        for (int i = 0; i < NUM_OBS; i++)
            DrawSprite(obsTextures[obsTipo[i]], obs[i].x, obs[i].y, OBS_SCALE);
        int lp = MeasureText("PULE!", 30);
        DrawText("PULE!", (screenWidth / 2) - (lp / 2), 30, 30, YELLOW);
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
            bgX -= bgVel;
            if (bgX <= -(float)screenWidth)
                bgX = 0.0f;
            BeginDrawing();
            DrawBgScroll(background, bgX, screenWidth, screenHeight);
            DrawSprite(mulaFrames[frameAtual], playerX, playerY, MULA_SCALE);
            for (int i = 0; i < NUM_OBS; i++)
                DrawSprite(obsTextures[obsTipo[i]], obs[i].x, obs[i].y, OBS_SCALE);
            DrawTimerBomba(&timer, screenWidth, screenHeight);
            DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
            EndDrawing();
        }
    }
    while (frames > 0 && !WindowShouldClose())
    {
        frames--;
        BeginDrawing();
        ClearBackground((Color){10, 10, 30, 255});
        int sz = 40;
        const char *txt = flag ? "Vitoria" : "Derrota";
        Color cor = flag ? GREEN : RED;
        int lw = MeasureText(txt, sz);
        DrawText(txt, (screenWidth / 2) - (lw / 2), (screenHeight / 2) - (sz / 2), sz, cor);
        DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }
    UnloadTexture(borderGame);
    UnloadTexture(background);
    for (int i = 0; i < 4; i++)
        UnloadTexture(mulaFrames[i]);
    for (int i = 0; i < NUM_OBS; i++)
        UnloadTexture(obsTextures[i]);
    UnloadTimerBomba(&timer);
}