#include "pegueAsCapivaras.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>

#define TEMPO_PAVIO 8.0f
#define TEMPO_BOMBA_FUSE 1.5f
#define NUM_BURACOS 6

static const float BURACO_X[NUM_BURACOS] = {148.0f, 587.0f, 1026.0f, 148.0f, 587.0f, 1026.0f};
static const float BURACO_Y[NUM_BURACOS] = {336.0f, 336.0f, 336.0f, 580.0f, 580.0f, 580.0f};
static const float RAIO_BURACO_TELA = 63.0f;

typedef struct
{
    float tempoAtivo;
    int ativa;
} Buraco;

void pegueAsCapivaras()
{
    Texture2D borderGame = LoadTexture("../assets/sprites/border_game.png");
    int screenWidth = 1200;
    int screenHeight = 720;
    SetTargetFPS(60);
    int flag = 0;
    int frames = 120;
    int clicadas = 0;
    int totalApareceu = 0;
    int frameSpawn = 0;
    int intervaloSpawn = 80;
    float tempoVisivel = 1.2f;
    Texture2D texFundo = LoadTexture("../assets/sprites/pegueascapivaras/pegar_capibara.png");
    Texture2D texCapivara = LoadTexture("../assets/sprites/pegueascapivaras/capibara.png");
    Buraco buracos[NUM_BURACOS];
    for (int i = 0; i < NUM_BURACOS; i++)
    {
        buracos[i].ativa = 0;
        buracos[i].tempoAtivo = 0.0f;
    }
    Rectangle destFundo = {0, 0, (float)screenWidth, (float)screenHeight};
    Rectangle srcFundo = {0, 0, (float)texFundo.width, (float)texFundo.height};
    float escCapivara = (RAIO_BURACO_TELA * 2.0f) / (float)texCapivara.width;
    TimerBomba timer;
    InitTimerBomba(&timer, TEMPO_PAVIO, TEMPO_BOMBA_FUSE);
    while (timer.fase == FASE_PAVIO && !WindowShouldClose())
    {
        UpdateTimerBomba(&timer);
        float dt = GetFrameTime();
        frameSpawn++;
        if (frameSpawn >= intervaloSpawn)
        {
            frameSpawn = 0;
            int tentativas = 0;
            int idx = rand() % NUM_BURACOS;
            while (buracos[idx].ativa && tentativas < NUM_BURACOS)
            {
                idx = (idx + 1) % NUM_BURACOS;
                tentativas++;
            }
            if (!buracos[idx].ativa)
            {
                buracos[idx].ativa = 1;
                buracos[idx].tempoAtivo = tempoVisivel;
                totalApareceu++;
            }
        }
        for (int i = 0; i < NUM_BURACOS; i++)
        {
            if (!buracos[i].ativa)
                continue;
            buracos[i].tempoAtivo -= dt;
            if (buracos[i].tempoAtivo <= 0.0f)
                buracos[i].ativa = 0;
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < NUM_BURACOS; i++)
            {
                if (!buracos[i].ativa)
                    continue;
                if (CheckCollisionPointCircle(mouse, (Vector2){BURACO_X[i], BURACO_Y[i]}, RAIO_BURACO_TELA * 0.85f))
                {
                    buracos[i].ativa = 0;
                    clicadas++;
                    break;
                }
            }
        }
        BeginDrawing();
        DrawTexturePro(texFundo, srcFundo, destFundo, (Vector2){0, 0}, 0.0f, WHITE);
        for (int i = 0; i < NUM_BURACOS; i++)
        {
            if (!buracos[i].ativa)
                continue;
            float drawX = BURACO_X[i] - (texCapivara.width * escCapivara) / 2.0f;
            float drawY = BURACO_Y[i] - (texCapivara.height * escCapivara) / 2.0f;
            DrawTextureEx(texCapivara, (Vector2){drawX, drawY}, 0.0f, escCapivara, WHITE);
        }
        int larguraObj = MeasureText("PEGUE AS CAPIVARAS!", 28);
        DrawText("PEGUE AS CAPIVARAS!", (screenWidth / 2) - (larguraObj / 2), 18, 28, DARKBROWN);
        DrawText(TextFormat("Capturadas: %d", clicadas), 20, 18, 24, WHITE);
        DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
        DrawTimerBomba(&timer, screenWidth, screenHeight);
        EndDrawing();
    }
    if (totalApareceu > 0 && clicadas > totalApareceu / 2)
        flag = 1;
    while (!TimerTerminou(&timer) && !WindowShouldClose())
    {
        UpdateTimerBomba(&timer);
        BeginDrawing();
        DrawTexturePro(texFundo, srcFundo, destFundo, (Vector2){0, 0}, 0.0f, WHITE);
        DrawTimerBomba(&timer, screenWidth, screenHeight);
        DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }
    while (frames > 0 && !WindowShouldClose())
    {
        frames--;
        BeginDrawing();
        DrawTexturePro(texFundo, srcFundo, destFundo, (Vector2){0, 0}, 0.0f, WHITE);
        int tamanhoFonte = 40;
        if (flag)
        {
            int larg = MeasureText("Vitoria", tamanhoFonte);
            DrawText("Vitoria", (screenWidth / 2) - (larg / 2), (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, GREEN);
        }
        else
        {
            int larg = MeasureText("Derrota", tamanhoFonte);
            DrawText("Derrota", (screenWidth / 2) - (larg / 2), (screenHeight / 2) - (tamanhoFonte / 2), tamanhoFonte, RED);
        }
        DrawTexturePro(borderGame, (Rectangle){0, 0, (float)borderGame.width, (float)borderGame.height}, (Rectangle){0, 0, 1200, 720}, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }
    UnloadTexture(borderGame);
    UnloadTexture(texFundo);
    UnloadTexture(texCapivara);
    UnloadTimerBomba(&timer);
}