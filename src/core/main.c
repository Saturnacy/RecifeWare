#include "raylib.h"
#include "transition.h"
#include "cozinhaDeTapioca.h"
#include "crabber.h"
#include "escaladaBrenan.h"
#include "escapedotubarao.h"
#include "mulaSemCabeca.h"
#include "pegueascapivaras.h"
#include "player.h"
#include "queue.h"
#include "sort.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
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
    STATE_NAME_INPUT,
    STATE_GAMEPLAY,
    STATE_RANKING,
    STATE_DEBUG
} Gamestate;

typedef enum
{
    LS_ENTERING,
    LS_LOADING,
    LS_EXITING
} LoadingPhase;

static const char *GAME_NAMES[7] = {
    "",
    "Cozinha de Tapioca",
    "Crabber",
    "Escalada do Brenan",
    "Escape do Tubarao",
    "Mula Sem Cabeca",
    "Pegue as Capivaras"};

static void safe_restore_ranking(int mat[11][5])
{
    FILE *f = fopen("rank.txt", "r");
    if (f)
    {
        fclose(f);
        restore_ranking(mat);
    }
}

int main()
{
    int screenWidth = 1200;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "RecifeWare");
    SetTargetFPS(60);

    Gamestate currentState = STATE_SPLASH_FADE_IN;

    int ranking[11][5] = {0};
    safe_restore_ranking(ranking);
    Jogador status = {0, ""};

    char input_nome[4] = {0};
    int input_pos = 0;
    bool name_duplicate = false;
    bool name_too_short = false;

    int game_points[7] = {0};
    int games_played = 0;

    LoadGameFont();
    Font f = GetGameFont();

    Texture2D cesarLogo = LoadTexture("assets/sprites/cesar_logo.png");
    Texture2D borderTs = LoadTexture("assets/sprites/title_screen/border_ts.png");
    Texture2D screenTs = LoadTexture("assets/sprites/title_screen/screen_ts.png");
    Texture2D loadingScreen = LoadTexture("assets/sprites/title_screen/loading_ts.png");
    Texture2D loadingComplete = LoadTexture("assets/sprites/title_screen/loadingcomplete.png");
    Texture2D gameLogo = LoadTexture("assets/sprites/title_screen/game_logo.png");

    Texture2D icons[6];
    icons[0] = LoadTexture("assets/sprites/title_screen/trash_icon.png");
    icons[1] = LoadTexture("assets/sprites/title_screen/mail_icon.png");
    icons[2] = LoadTexture("assets/sprites/title_screen/map_icon.png");
    icons[3] = LoadTexture("assets/sprites/title_screen/pc_icon.png");
    icons[4] = LoadTexture("assets/sprites/title_screen/diskette.png");
    icons[5] = LoadTexture("assets/sprites/title_screen/frevo_icon.png");

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

        case STATE_NAME_INPUT:
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if (input_pos < 3)
                {
                    if (key >= 'a' && key <= 'z')
                        key -= 32;
                    if (key >= 'A' && key <= 'Z')
                    {
                        input_nome[input_pos] = (char)key;
                        input_pos++;
                        name_duplicate = false;
                        name_too_short = false;
                    }
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && input_pos > 0)
            {
                input_pos--;
                input_nome[input_pos] = 0;
                name_duplicate = false;
                name_too_short = false;
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                if (input_pos < 3)
                {
                    name_too_short = true;
                }
                else
                {
                    input_nome[3] = '\0';
                    int valid = validate_name(input_nome, ranking, input_pos);
                    if (valid == 0)
                    {
                        name_duplicate = true;
                    }
                    else
                    {

                        name_duplicate = false;
                        name_too_short = false;
                        strcpy(status.nome_usuario, input_nome);
                        status.ponto = 0;
                        games_played = 0;

                        for (int i = 1; i <= NUM_GAMES; i++)
                            game_points[i] = GetRandomValue(1, 6) * 100;

                        free_queue(&front, &rear);
                        for (int i = 1; i <= NUM_GAMES; i++)
                            enqueue_game(i, 0, &front, &rear);
                        assign_game_weights(&front, NUM_GAMES);
                        insertion_sort_games(&front, &rear);

                        currentState = STATE_GAMEPLAY;
                    }
                }
            }
            if (IsKeyPressed(KEY_ESCAPE))
            {
                input_pos = 0;
                memset(input_nome, 0, sizeof(input_nome));
                name_duplicate = false;
                name_too_short = false;
                currentState = STATE_TITLE;
            }
        }
        break;

        case STATE_GAMEPLAY:
            if (front == NULL)
            {

                int nova[5];
                nova[0] = (int)status.nome_usuario[0];
                nova[1] = (int)status.nome_usuario[1];
                nova[2] = (int)status.nome_usuario[2];
                nova[3] = games_played;
                nova[4] = status.ponto;
                update_ranking(ranking, nova);
                save_ranking(ranking);
                currentState = STATE_RANKING;
            }
            else
            {
                int gid = front->id;
                RunTransitionScreen();

                switch (gid)
                {
                case 1:
                    cozinhaDeTapioca();
                    break;
                case 2:
                    crabber();
                    break;
                case 3:
                    escaladaBrenan();
                    break;
                case 4:
                    escapeDoTubarao(2);
                    break;
                case 5:
                    mulaSemCabeca();
                    break;
                case 6:
                    pegueAsCapivaras();
                    break;
                }

                status.ponto += game_points[gid];
                games_played++;
                dequeue_game(&front, &rear);
            }
            break;

        case STATE_RANKING:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
                currentState = STATE_TITLE;
            break;

        case STATE_DEBUG:
            if (IsKeyPressed(KEY_ONE) && step == 0)
            {
                for (int i = 1; i <= NUM_GAMES; i++)
                    enqueue_game(i, 0, &front, &rear);
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

            const float BTN_X = 735.0f;
            const float BTN_W = 270.0f;
            const float BTN_H = 54.0f;
            Vector2 mousePoint = GetMousePosition();
            Rectangle btnMain = {BTN_X, 268, BTN_W, BTN_H};
            Rectangle btnRanking = {BTN_X, 348, BTN_W, BTN_H};
            Rectangle btnExit = {BTN_X, 428, BTN_W, BTN_H};

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, btnMain))
                {
                    input_pos = 0;
                    memset(input_nome, 0, sizeof(input_nome));
                    name_duplicate = false;
                    name_too_short = false;
                    currentState = STATE_NAME_INPUT;
                }
                else if (CheckCollisionPointRec(mousePoint, btnRanking))
                    currentState = STATE_RANKING;
                else if (CheckCollisionPointRec(mousePoint, btnExit))
                    exitGame = true;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (currentState == STATE_TITLE ||
            currentState == STATE_DEBUG ||
            currentState == STATE_TITLE_ANIM ||
            currentState == STATE_SCREEN_IN ||
            currentState == STATE_NAME_INPUT ||
            currentState == STATE_GAMEPLAY ||
            currentState == STATE_RANKING)
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
                scaledWidth, scaledHeight};
            DrawTexturePro(cesarLogo, sourceRec, destRec, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        }
        break;

        case STATE_TITLE_ANIM:
            break;

        case STATE_TITLE:
        {

            const float SPLIT_X = 600.0f;
            const float LEFT_CX = MON_X + (SPLIT_X - MON_X) / 2.0f;
            const float CONTENT_CY = MON_Y + MON_CONTENT_H / 2.0f;

            const float LOGO_SCALE = 4.0f;
            float logoW = gameLogo.width * LOGO_SCALE;
            float logoH = gameLogo.height * LOGO_SCALE;
            float logoX = LEFT_CX - logoW / 2.0f;
            float logoY = CONTENT_CY - logoH / 2.0f - 20.0f;
            DrawTexturePro(gameLogo,
                           (Rectangle){0, 0, (float)gameLogo.width, (float)gameLogo.height},
                           (Rectangle){logoX, logoY, logoW, logoH},
                           (Vector2){0, 0}, 0.0f, WHITE);

            DrawLineEx(
                (Vector2){SPLIT_X, MON_Y + 24},
                (Vector2){SPLIT_X, MON_Y + MON_CONTENT_H - 24},
                2.0f, Fade(WHITE, 0.35f));

            const float BTN_X = 735.0f;
            const float BTN_W = 270.0f;
            const float BTN_H = 54.0f;
            Rectangle btnMain = {BTN_X, 268, BTN_W, BTN_H};
            Rectangle btnRanking = {BTN_X, 348, BTN_W, BTN_H};
            Rectangle btnExit = {BTN_X, 428, BTN_W, BTN_H};

            Vector2 mouse = GetMousePosition();

            Rectangle btns[3] = {btnMain, btnRanking, btnExit};
            const char *labels[3] = {"Jogar", "Ranking", "Sair"};
            for (int b = 0; b < 3; b++)
            {
                bool hov = CheckCollisionPointRec(mouse, btns[b]);
                Color bgCol = hov ? (Color){255, 255, 255, 40} : (Color){255, 255, 255, 12};
                Color brdCol = hov ? WHITE : Fade(WHITE, 0.50f);
                DrawRectangleRec(btns[b], bgCol);
                DrawRectangleLinesEx(btns[b], 2, brdCol);

                Vector2 tSize = MeasureTextEx(f, labels[b], 30, 1);
                float tx = btns[b].x + (btns[b].width - tSize.x) / 2.0f;
                float ty = btns[b].y + (btns[b].height - tSize.y) / 2.0f;
                DrawTextEx(f, labels[b], (Vector2){tx, ty}, 30, 1, hov ? YELLOW : WHITE);
            }

            Vector2 mSz = MeasureTextEx(f, "MENU", 26, 1);
            DrawTextEx(f, "MENU",
                       (Vector2){BTN_X + (BTN_W - mSz.x) / 2.0f, 220},
                       26, 1, Fade(WHITE, 0.55f));
            DrawLineEx(
                (Vector2){BTN_X, 252},
                (Vector2){BTN_X + BTN_W, 252},
                1.5f, Fade(WHITE, 0.30f));
        }
        break;

        case STATE_NAME_INPUT:
        {
            float cx = screenWidth / 2.0f;
            float cy = screenHeight / 2.0f;

            DrawTextEx(f, "INSIRA SEU NOME",
                       (Vector2){cx - 130, cy - 140}, 36, 1, WHITE);

            DrawTextEx(f, "(3 letras - ENTER para confirmar)",
                       (Vector2){cx - 175, cy - 95}, 22, 1, LIGHTGRAY);

            Rectangle box = {cx - 90, cy - 55, 180, 60};
            DrawRectangleLinesEx(box, 3, WHITE);

            char display[8];
            snprintf(display, sizeof(display), "%s%s",
                     input_nome,
                     (input_pos < 3 && ((int)(GetTime() * 2) % 2 == 0)) ? "_" : " ");
            DrawTextEx(f, display,
                       (Vector2){box.x + 12, box.y + 12}, 40, 2, YELLOW);

            if (name_duplicate)
                DrawTextEx(f, "! Nome ja existe no ranking !",
                           (Vector2){cx - 155, cy + 25}, 24, 1, RED);
            else if (name_too_short)
                DrawTextEx(f, "! Digite 3 letras !",
                           (Vector2){cx - 100, cy + 25}, 24, 1, RED);

            DrawTextEx(f, "ESC para voltar",
                       (Vector2){cx - 80, cy + 80}, 20, 1, DARKGRAY);
        }
        break;

        case STATE_GAMEPLAY:
            DrawTextEx(f, "Preparando proximo jogo...",
                       (Vector2){(float)screenWidth / 2 - 180, (float)screenHeight / 2 - 20},
                       30, 1, WHITE);
            break;

        case STATE_RANKING:
        {

            float rx = MON_X + 60;
            float ry = MON_Y + 25;
            DrawTextEx(f, "RANKING", (Vector2){rx + 280, ry}, 40, 1, YELLOW);

            DrawLineEx(
                (Vector2){rx, ry + 52},
                (Vector2){MON_X + MON_W - 60, ry + 52},
                2, WHITE);

            DrawTextEx(f, "POS", (Vector2){rx, ry + 18}, 24, 1, LIGHTGRAY);
            DrawTextEx(f, "NOME", (Vector2){rx + 80, ry + 18}, 24, 1, LIGHTGRAY);
            DrawTextEx(f, "JOGOS", (Vector2){rx + 220, ry + 18}, 24, 1, LIGHTGRAY);
            DrawTextEx(f, "PONTOS", (Vector2){rx + 380, ry + 18}, 24, 1, LIGHTGRAY);

            for (int r = 0; r < 10; r++)
            {
                float rowY = ry + 60 + r * 48;

                Color rowBg = (r % 2 == 0)
                                  ? (Color){30, 30, 60, 180}
                                  : (Color){20, 20, 40, 180};
                DrawRectangle((int)rx - 8, (int)rowY - 4,
                              (int)(MON_W - 120), 42, rowBg);

                if (ranking[r][0] == (int)status.nome_usuario[0] &&
                    ranking[r][1] == (int)status.nome_usuario[1] &&
                    ranking[r][2] == (int)status.nome_usuario[2] &&
                    ranking[r][4] == status.ponto)
                {
                    DrawRectangleLinesEx(
                        (Rectangle){rx - 8, rowY - 4, MON_W - 120, 42},
                        2, GOLD);
                }

                Color numColor = (r == 0) ? GOLD : (r == 1) ? LIGHTGRAY
                                               : (r == 2)   ? ORANGE
                                                            : WHITE;
                char pos_str[8];
                snprintf(pos_str, sizeof(pos_str), "%d.", r + 1);
                DrawTextEx(f, pos_str, (Vector2){rx, rowY + 2}, 26, 1, numColor);

                if (ranking[r][4] > 0)
                {
                    char nome_str[8];
                    snprintf(nome_str, sizeof(nome_str), "%c%c%c",
                             (char)ranking[r][0],
                             (char)ranking[r][1],
                             (char)ranking[r][2]);
                    DrawTextEx(f, nome_str, (Vector2){rx + 80, rowY + 2}, 26, 1, WHITE);

                    char jogos_str[16];
                    snprintf(jogos_str, sizeof(jogos_str), "%d", ranking[r][3]);
                    DrawTextEx(f, jogos_str, (Vector2){rx + 240, rowY + 2}, 26, 1, WHITE);

                    char pts_str[16];
                    snprintf(pts_str, sizeof(pts_str), "%d", ranking[r][4]);
                    DrawTextEx(f, pts_str, (Vector2){rx + 400, rowY + 2}, 26, 1, numColor);
                }
                else
                {
                    DrawTextEx(f, "---", (Vector2){rx + 80, rowY + 2}, 26, 1, DARKGRAY);
                }
            }

            if (status.ponto > 0)
            {
                char score_str[64];
                snprintf(score_str, sizeof(score_str),
                         "Sua pontuacao: %s  %d pts",
                         status.nome_usuario, status.ponto);
                DrawTextEx(f, score_str,
                           (Vector2){(float)screenWidth / 2 - 160, MON_Y + MON_CONTENT_H - 50},
                           26, 1, GOLD);
            }

            DrawTextEx(f, "ENTER para voltar ao menu",
                       (Vector2){(float)screenWidth / 2 - 160, MON_Y + MON_CONTENT_H - 25},
                       22, 1, LIGHTGRAY);
        }
        break;

        case STATE_DEBUG:
        {
            DrawText("DEBUG", 250, 40, 20, DARKGRAY);
            DrawText("Teclas: [1] Criar Nos e Pesos | [2] Insertion Sort | [ESPACO] Desenfileirar | [B] Voltar",
                     150, 80, 18, BLUE);
            DrawText("FILA DINAMICA NA MEMORIA:", 50, 250, 20, BLACK);

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
                    DrawText(TextFormat("ID: %d", curr->id), blockX + 30, blockY + 15, 20, WHITE);
                    DrawText(TextFormat("W: %d", curr->weight), blockX + 20, blockY + 50, 18, BLACK);
                    DrawText(GAME_NAMES[curr->id], blockX + 4, blockY + 78, 10, WHITE);
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

        if (currentState == STATE_TITLE_ANIM)
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
                DrawTexturePro(icons[idx],
                               (Rectangle){0, 0, (float)icons[idx].width, (float)icons[idx].height},
                               (Rectangle){MON_X + MON_W - iw - EDGE_MARGIN, posY, iw, ih},
                               (Vector2){0, 0}, 0.0f, WHITE);
            }
        }

        if (currentState == STATE_TITLE_ANIM)
        {
            float scaledW = loadingScreen.width * LS_SCALE;
            float scaledH = loadingScreen.height * LS_SCALE;
            DrawTexturePro(loadingScreen,
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

        if (currentState == STATE_TITLE_ANIM || currentState == STATE_TITLE ||
            currentState == STATE_MONITOR_ON || currentState == STATE_DEBUG ||
            currentState == STATE_SCREEN_IN || currentState == STATE_NAME_INPUT ||
            currentState == STATE_GAMEPLAY || currentState == STATE_RANKING)
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
            bool isSplash = (currentState == STATE_SPLASH_FADE_IN ||
                             currentState == STATE_SPLASH_CESAR ||
                             currentState == STATE_FADE_OUT ||
                             currentState == STATE_WAIT_BORDER ||
                             currentState == STATE_BORDER_IN);
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