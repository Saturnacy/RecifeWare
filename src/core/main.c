#include "raylib.h"
#include "escaladaBrenan.h"
#include "player.h"
#include "queue.h"
#include "sort.h"
#include <stdbool.h>
#include <stdio.h>
#include "font.h"
#include <math.h>

#define NUM_GAMES 6

typedef enum
{
    STATE_SPLASH_FADE_IN,
    STATE_SPLASH_CESAR,
    STATE_FADE_OUT,
    STATE_WAIT_BORDER,
    STATE_BORDER_IN,
    STATE_SCREEN_IN,
    STATE_MONITOR_ON,
    STATE_TITLE_ANIM,
    STATE_TITLE,
    STATE_DEBUG
} Gamestate;

typedef enum
{
    LS_ENTERING,
    LS_LOADING,
    LS_EXITING
} LoadingPhase;

int main()
{
    int screenWidth = 1200;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "RecifeWare");
    SetTargetFPS(60);

    Gamestate currentState = STATE_SPLASH_FADE_IN;
    int ranking[11][5] = {0};
    int cont_caracter = 0;
    Jogador status = {0, ""};

    LoadGameFont();

    Texture2D cesarLogo = LoadTexture("../assets/sprites/cesar_logo.png");
    Texture2D borderTs = LoadTexture("../assets/sprites/title_screen/border_ts.png");
    Texture2D screenTs = LoadTexture("../assets/sprites/title_screen/screen_ts.png");
    Texture2D loadingScreen = LoadTexture("../assets/sprites/title_screen/loading_ts.png");
    Texture2D loadingComplete = LoadTexture("../assets/sprites/title_screen/loadingcomplete.png");
    Texture2D gameLogo = LoadTexture("../assets/sprites/title_screen/game_logo.png");

    Texture2D icons[6];
    icons[0] = LoadTexture("../assets/sprites/title_screen/trash_icon.png");
    icons[1] = LoadTexture("../assets/sprites/title_screen/mail_icon.png");
    icons[2] = LoadTexture("../assets/sprites/title_screen/map_icon.png");
    icons[3] = LoadTexture("../assets/sprites/title_screen/pc_icon.png");
    icons[4] = LoadTexture("../assets/sprites/title_screen/diskette.png");
    icons[5] = LoadTexture("../assets/sprites/title_screen/frevo_icon.png");

    float CESARlogoscale = 2.0f;
    float monitorAlpha = 255.0f;
    float borderAlpha = 0.0f;
    float screenAlpha = 0.0f;
    float waitTimer = 0.0f;
    float fadeAlpha = 255.0f;
    int frameCounter = 0;
    bool exitGame = false;

    const float MON_X = 0.05f * screenWidth;
    const float MON_Y = 0.075f * screenHeight;
    const float MON_W = 0.90f * screenWidth;
    const float MON_H = 0.85f * screenHeight;
    const float MON_CONTENT_H = (128.0f / 136.0f) * MON_H;

    const float LS_SCALE = 4.0f;
    const float LS_TARGET_X = MON_X + 80.0f;
    const float LS_TARGET_Y = MON_Y + 60.0f;
    const float LS_OFFSCREEN = -(loadingScreen.width * LS_SCALE + 20.0f);
    const Rectangle BAR_SRC = {7, 28, 88, 7};

    const float ICON_SCALE = 4.5f;
    const float ICON_MARGIN = 80.0f;
    const float TASKBAR_Y = MON_Y + MON_CONTENT_H;

    float lsX = LS_OFFSCREEN;
    float lsProgress = 0.0f;
    float effectTimer = 0.0f;
    float exitWaitTimer = 0.0f;
    LoadingPhase lsPhase = LS_ENTERING;

    node_q *front = NULL;
    node_q *rear = NULL;
    int step = 0;

    while (!WindowShouldClose() && !exitGame)
    {
        float dt = GetFrameTime();

        switch (currentState)
        {
        case STATE_SPLASH_FADE_IN:
            fadeAlpha -= 8.0f;
            if (fadeAlpha <= 0)
            {
                fadeAlpha = 0;
                currentState = STATE_SPLASH_CESAR;
            }
            break;

        case STATE_SPLASH_CESAR:
            frameCounter++;
            if (frameCounter > 120)
            {
                currentState = STATE_FADE_OUT;
                frameCounter = 0;
            }
            break;

        case STATE_FADE_OUT:
            fadeAlpha += 8.0f;
            if (fadeAlpha >= 255.0f)
            {
                fadeAlpha = 255.0f;
                waitTimer = 0.0f;
                currentState = STATE_WAIT_BORDER;
            }
            break;

        case STATE_WAIT_BORDER:
            waitTimer += dt;
            if (waitTimer >= 1.0f)
            {
                borderAlpha = 0.0f;
                fadeAlpha = 255.0f;
                currentState = STATE_BORDER_IN;
            }
            break;

        case STATE_BORDER_IN:
            borderAlpha += 4.0f;
            fadeAlpha -= 4.0f;
            if (fadeAlpha < 0.0f)
                fadeAlpha = 0.0f;
            if (borderAlpha >= 255.0f)
            {
                borderAlpha = 255.0f;
                fadeAlpha = 0.0f;
                screenAlpha = 0.0f;
                currentState = STATE_SCREEN_IN;
            }
            break;

        case STATE_SCREEN_IN:
            screenAlpha += 4.0f;
            if (screenAlpha >= 255.0f)
            {
                screenAlpha = 255.0f;
                monitorAlpha = 0.0f;
                lsPhase = LS_ENTERING;
                lsX = LS_OFFSCREEN;
                lsProgress = 0.0f;
                exitWaitTimer = 0.0f;
                currentState = STATE_TITLE_ANIM;
            }
            break;

        case STATE_MONITOR_ON:
            break;

        case STATE_TITLE_ANIM:
            if (fadeAlpha > 0)
                fadeAlpha -= 8.0f;

            if (lsPhase == LS_ENTERING)
            {
                lsX += (LS_TARGET_X - lsX) * 10.0f * dt;
                if (lsX >= LS_TARGET_X - 1.0f)
                {
                    lsX = LS_TARGET_X;
                    lsPhase = LS_LOADING;
                }
            }
            else if (lsPhase == LS_LOADING)
            {
                effectTimer += dt;
                lsProgress += dt / 5.0f;
                if (lsProgress >= 1.0f)
                {
                    lsProgress = 1.0f;
                    lsPhase = LS_EXITING;
                }
            }
            else if (lsPhase == LS_EXITING)
            {
                effectTimer += dt;
                exitWaitTimer += dt;
                if (exitWaitTimer >= 2.0f)
                {
                    lsX += (LS_OFFSCREEN - lsX) * 8.0f * dt;
                    if (lsX <= LS_OFFSCREEN + 1.0f)
                    {
                        lsX = LS_OFFSCREEN;
                        currentState = STATE_TITLE;
                        fadeAlpha = 0.0f;
                    }
                }
            }
            break;

        case STATE_DEBUG:
            if (IsKeyPressed(KEY_ONE) && step == 0)
            {
                for (int i = 1; i <= NUM_GAMES; i++)
                {
                    enqueue_game(i, 0, &front, &rear);
                }
                assign_game_weights(&front, NUM_GAMES);
                step = 1;
            }

            if (IsKeyPressed(KEY_TWO) && step == 1)
            {
                insertion_sort_games(&front, &rear);
                step = 2;
            }

            if (IsKeyPressed(KEY_SPACE) && step >= 1)
            {
                dequeue_game(&front, &rear);
                if (front == NULL)
                    step = 0;
            }

            if (IsKeyPressed(KEY_B))
            {
                free_queue(&front, &rear);
                step = 0;
                currentState = STATE_TITLE;
            }
            break;

        default:
            break;
        }

        if (currentState == STATE_TITLE)
        {
            if (fadeAlpha > 0)
                fadeAlpha -= 8.0f;

            Vector2 mousePoint = GetMousePosition();
            Rectangle btnMain = {screenWidth / 2 - 100, 300, 200, 40};
            Rectangle btnArcade = {screenWidth / 2 - 100, 360, 200, 40};
            Rectangle btnOptions = {screenWidth / 2 - 100, 420, 200, 40};
            Rectangle btnExit = {screenWidth / 2 - 100, 480, 200, 40};

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, btnMain))
                {
                    escaladaBrenan();
                }
                else if (CheckCollisionPointRec(mousePoint, btnArcade))
                {
                    currentState = STATE_DEBUG;
                }
                else if (CheckCollisionPointRec(mousePoint, btnOptions))
                {
                }
                else if (CheckCollisionPointRec(mousePoint, btnExit))
                {
                    exitGame = true;
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (currentState == STATE_TITLE || currentState == STATE_DEBUG || currentState == STATE_TITLE_ANIM || currentState == STATE_SCREEN_IN)
        {
            DrawTexturePro(screenTs,
                           (Rectangle){0, 0, (float)screenTs.width, (float)screenTs.height},
                           (Rectangle){MON_X, MON_Y, MON_W, MON_H},
                           (Vector2){0, 0}, 0.0f,
                           currentState == STATE_SCREEN_IN ? Fade(WHITE, screenAlpha / 255.0f) : WHITE);
        }

        BeginScissorMode((int)MON_X, (int)MON_Y, (int)MON_W, (int)MON_CONTENT_H);

        switch (currentState)
        {

        case STATE_SPLASH_FADE_IN:
        case STATE_SPLASH_CESAR:
        case STATE_FADE_OUT:
        case STATE_WAIT_BORDER:
        {
            Rectangle sourceRec = {0.0f, 0.0f, (float)cesarLogo.width, (float)cesarLogo.height};
            float scaledWidth = (float)cesarLogo.width * CESARlogoscale;
            float scaledHeight = (float)cesarLogo.height * CESARlogoscale;
            Rectangle destRec = {
                (screenWidth - scaledWidth) / 2.0f,
                (screenHeight - scaledHeight) / 2.0f,
                scaledWidth,
                scaledHeight};
            DrawTexturePro(cesarLogo, sourceRec, destRec, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        }
        break;

        case STATE_TITLE_ANIM:
            break;

        case STATE_TITLE:
        {
            Font f = GetGameFont();
            const float LOGO_SCALE = 4.0f;
            float logoW = gameLogo.width * LOGO_SCALE;
            float logoH = gameLogo.height * LOGO_SCALE;
            DrawTexturePro(gameLogo,
                           (Rectangle){0, 0, (float)gameLogo.width, (float)gameLogo.height},
                           (Rectangle){(screenWidth - logoW) / 2.0f, 80, logoW, logoH},
                           (Vector2){0, 0}, 0.0f, WHITE);

            Vector2 mousePoint = GetMousePosition();
            Rectangle btnMain = {screenWidth / 2 - 100, 300, 200, 40};
            Rectangle btnArcade = {screenWidth / 2 - 100, 360, 200, 40};
            Rectangle btnOptions = {screenWidth / 2 - 100, 420, 200, 40};
            Rectangle btnExit = {screenWidth / 2 - 100, 480, 200, 40};

            DrawTextEx(f, "Main Game", (Vector2){btnMain.x + 40, btnMain.y + 10}, 30, 1, WHITE);
            DrawTextEx(f, "Testar AED", (Vector2){btnArcade.x + 40, btnArcade.y + 10}, 30, 1, WHITE);
            DrawTextEx(f, "Options", (Vector2){btnOptions.x + 60, btnOptions.y + 10}, 30, 1, WHITE);
            DrawTextEx(f, "Exit", (Vector2){btnExit.x + 80, btnExit.y + 10}, 30, 1, WHITE);
        }
        break;

        case STATE_DEBUG:
        {
            DrawText("DEBUG", 250, 40, 20, DARKGRAY);
            DrawText("Teclas: [1] Criar Nós e Pesos | [2] Insertion Sort de Ponteiros | [ESPAÇO] Desenfileirar", 150, 80, 20, BLUE);
            DrawText("FILA DINÂMICA NA MEMÓRIA:", 50, 250, 20, BLACK);

            if (step >= 1)
            {
                node_q *curr = front;
                int idx = 0;

                while (curr != NULL)
                {
                    int blockX = 50 + (idx * 160);
                    int blockY = 300;

                    Color blockColor = (curr->weight == 9999) ? RED : ORANGE;

                    DrawRectangle(blockX, blockY, 120, 100, blockColor);
                    DrawRectangleLines(blockX, blockY, 120, 100, MAROON);
                    DrawText(TextFormat("ID: %d", curr->id), blockX + 30, blockY + 20, 20, WHITE);
                    DrawText(TextFormat("W: %d", curr->weight), blockX + 25, blockY + 60, 20, BLACK);

                    if (curr->next != NULL)
                    {
                        DrawLineEx(
                            (Vector2){(float)blockX + 120, (float)blockY + 50},
                            (Vector2){(float)blockX + 160, (float)blockY + 50},
                            4.0f, BLACK);
                        DrawTriangle(
                            (Vector2){(float)blockX + 160, (float)blockY + 50},
                            (Vector2){(float)blockX + 150, (float)blockY + 40},
                            (Vector2){(float)blockX + 150, (float)blockY + 60},
                            BLACK);
                    }

                    curr = curr->next;
                    idx++;
                }

                if (front != NULL)
                    DrawText("FRONT", 50, 420, 18, MAROON);
                if (rear != NULL)
                    DrawText("REAR", 50 + ((idx - 1) * 160), 420, 18, MAROON);
            }
        }
        break;

        default:
            break;
        }

        if (currentState == STATE_TITLE_ANIM || currentState == STATE_TITLE)
        {
            float iw, ih;
            const float EDGE_MARGIN = 20.0f;
            float availH = TASKBAR_Y - MON_Y;

            int leftOrder[4] = {3, 1, 0, 4};
            for (int i = 0; i < 4; i++)
            {
                int idx = leftOrder[i];
                if (currentState == STATE_TITLE_ANIM && lsProgress < (i + 1) / 6.0f)
                    break;
                iw = icons[idx].width * ICON_SCALE;
                ih = icons[idx].height * ICON_SCALE;
                float slot = availH / 4.0f;
                float posY = MON_Y + slot * i + (slot - ih) / 2.0f;
                DrawTexturePro(icons[idx],
                               (Rectangle){0, 0, (float)icons[idx].width, (float)icons[idx].height},
                               (Rectangle){MON_X + EDGE_MARGIN, posY, iw, ih},
                               (Vector2){0, 0}, 0.0f, WHITE);
            }

            int rightOrder[2] = {2, 5};
            for (int i = 0; i < 2; i++)
            {
                int idx = rightOrder[i];
                if (currentState == STATE_TITLE_ANIM && lsProgress < (i + 5) / 6.0f)
                    break;
                iw = icons[idx].width * ICON_SCALE;
                ih = icons[idx].height * ICON_SCALE;
                float slot = availH / 2.0f;
                float posY = MON_Y + slot * i + (slot - ih) / 2.0f;
                DrawTexturePro(
                    icons[idx],
                    (Rectangle){0, 0, (float)icons[idx].width, (float)icons[idx].height},
                    (Rectangle){MON_X + MON_W - iw - EDGE_MARGIN, posY, iw, ih},
                    (Vector2){0, 0}, 0.0f, WHITE);
            }
        }

        if (currentState == STATE_TITLE_ANIM)
        {
            float scaledW = loadingScreen.width * LS_SCALE;
            float scaledH = loadingScreen.height * LS_SCALE;

            DrawTexturePro(
                loadingScreen,
                (Rectangle){0, 0, (float)loadingScreen.width, (float)loadingScreen.height},
                (Rectangle){lsX, LS_TARGET_Y, scaledW, scaledH},
                (Vector2){0, 0}, 0.0f, WHITE);

            float barX = lsX + BAR_SRC.x * LS_SCALE;
            float barY = LS_TARGET_Y + BAR_SRC.y * LS_SCALE;
            float barW = BAR_SRC.width * LS_SCALE * lsProgress;
            float barH = BAR_SRC.height * LS_SCALE;

            if (barW > 0)
            {
                BeginScissorMode((int)barX, (int)barY, (int)barW, (int)barH);

                float fullBarW = BAR_SRC.width * LS_SCALE;
                float speed = 5.0f;
                Color glowColors[3] = {
                    {60, 100, 255, 255},
                    {220, 30, 30, 255},
                    {255, 255, 255, 255},
                };
                float offset = fmodf(effectTimer * speed, 3.0f);
                int c0 = (int)offset % 3;
                int c1 = (c0 + 1) % 3;
                float tA = offset - (int)offset;
                Color colA = {
                    (unsigned char)(glowColors[c0].r + (glowColors[c1].r - glowColors[c0].r) * tA),
                    (unsigned char)(glowColors[c0].g + (glowColors[c1].g - glowColors[c0].g) * tA),
                    (unsigned char)(glowColors[c0].b + (glowColors[c1].b - glowColors[c0].b) * tA),
                    255,
                };
                float offsetB = fmodf(offset + 1.5f, 3.0f);
                int c2 = (int)offsetB % 3;
                int c3 = (c2 + 1) % 3;
                float tB = offsetB - (int)offsetB;
                Color colB = {
                    (unsigned char)(glowColors[c2].r + (glowColors[c3].r - glowColors[c2].r) * tB),
                    (unsigned char)(glowColors[c2].g + (glowColors[c3].g - glowColors[c2].g) * tB),
                    (unsigned char)(glowColors[c2].b + (glowColors[c3].b - glowColors[c2].b) * tB),
                    255,
                };
                DrawRectangleGradientH((int)barX, (int)barY, (int)fullBarW, (int)barH, colA, colB);

                EndScissorMode();
            }
        }

        EndScissorMode();

        if (currentState == STATE_TITLE_ANIM || currentState == STATE_TITLE || currentState == STATE_MONITOR_ON || currentState == STATE_DEBUG || currentState == STATE_SCREEN_IN)
        {
            DrawTexturePro(borderTs,
                           (Rectangle){0, 0, (float)borderTs.width, (float)borderTs.height},
                           (Rectangle){0, 0, (float)screenWidth, (float)screenHeight},
                           (Vector2){0, 0}, 0.0f, WHITE);
        }
        else if (currentState == STATE_BORDER_IN)
        {
            DrawTexturePro(borderTs,
                           (Rectangle){0, 0, (float)borderTs.width, (float)borderTs.height},
                           (Rectangle){0, 0, (float)screenWidth, (float)screenHeight},
                           (Vector2){0, 0}, 0.0f, Fade(WHITE, borderAlpha / 255.0f));
        }

        if (fadeAlpha > 0 && currentState != STATE_DEBUG)
        {
            bool isSplash = (currentState == STATE_SPLASH_FADE_IN || currentState == STATE_SPLASH_CESAR || currentState == STATE_FADE_OUT || currentState == STATE_WAIT_BORDER || currentState == STATE_BORDER_IN);
            if (isSplash)
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, fadeAlpha / 255.0f));
            else
                DrawRectangle((int)MON_X, (int)MON_Y, (int)MON_W, (int)MON_H, Fade(BLACK, fadeAlpha / 255.0f));
        }

        EndDrawing();
    }

    free_queue(&front, &rear);
    UnloadTexture(cesarLogo);
    UnloadTexture(borderTs);
    UnloadTexture(screenTs);
    UnloadTexture(loadingScreen);
    UnloadTexture(loadingComplete);
    UnloadTexture(gameLogo);
    for (int i = 0; i < 6; i++)
        UnloadTexture(icons[i]);
    UnloadGameFont();
    CloseWindow();

    return 0;
}