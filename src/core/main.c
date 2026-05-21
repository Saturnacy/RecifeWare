#include "raylib.h"
#include "escaladaBrenan.h"
#include "player.h"
#include "queue.h"
#include "sort.h"
#include <stdbool.h>
#include <stdio.h>

#define NUM_GAMES 6

typedef enum {
    STATE_SPLASH_FADE_IN,
    STATE_SPLASH_CESAR,
    STATE_FADE_OUT,
    STATE_TITLE,
    STATE_DEBUG
} Gamestate;

int main() {
    int screenWidth = 1200;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "RecifeWare");
    SetTargetFPS(60);
    
    Gamestate currentState = STATE_SPLASH_FADE_IN; 
    int ranking[11][5]={0};
    int cont_caracter=0;
    Jogador status={ 0, "" };
    
    Texture2D cesarLogo = LoadTexture("../assets/sprites/cesar_logo.png");
    float CESARlogoscale = 2.0f;

    float fadeAlpha = 255.0f;
    int frameCounter = 0;
    bool exitGame = false;

    node_q *front = NULL;
    node_q *rear = NULL;
    int step = 0; 

    while (!WindowShouldClose() && !exitGame) {
        
        switch (currentState) {
            case STATE_SPLASH_FADE_IN:
                fadeAlpha -= 8.0f; 
                if (fadeAlpha <= 0) {
                    fadeAlpha = 0;
                    currentState = STATE_SPLASH_CESAR;
                }
                break;
                
            case STATE_SPLASH_CESAR:
                frameCounter++;
                if (frameCounter > 120) { 
                    currentState = STATE_FADE_OUT;
                    frameCounter = 0;
                }
                break;

            case STATE_FADE_OUT:
                fadeAlpha += 8.0f;
                if (fadeAlpha >= 255.0f) {
                    fadeAlpha = 255.0f;
                    currentState = STATE_TITLE; 
                }
                break;

            // ==========================================
            // LÓGICA DE CONTROLE DO TESTE
            // ==========================================
            case STATE_DEBUG:
                if (IsKeyPressed(KEY_ONE) && step == 0) {
                    for (int i = 1; i <= NUM_GAMES; i++) {
                        enqueue_game(i, 0, &front, &rear);
                    }
                    assign_game_weights(&front, NUM_GAMES);
                    step = 1;
                }

                if (IsKeyPressed(KEY_TWO) && step == 1) {
                    insertion_sort_games(&front, &rear);
                    step = 2;
                }
                
                if (IsKeyPressed(KEY_SPACE) && step >= 1) {
                    dequeue_game(&front, &rear);
                    if (front == NULL) step = 0; 
                }
                
                if (IsKeyPressed(KEY_B)) { 
                    free_queue(&front, &rear);
                    step = 0;
                    currentState = STATE_TITLE;
                }
                break;

            default:
                break;
        }

        if (currentState == STATE_TITLE) {
            
            if (fadeAlpha > 0) fadeAlpha -= 8.0f; 

            Vector2 mousePoint = GetMousePosition();
            Rectangle btnMain    = { screenWidth/2 - 100, 300, 200, 40 };
            Rectangle btnArcade  = { screenWidth/2 - 100, 360, 200, 40 };
            Rectangle btnOptions = { screenWidth/2 - 100, 420, 200, 40 };
            Rectangle btnExit    = { screenWidth/2 - 100, 480, 200, 40 };

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, btnMain)) {
                    escaladaBrenan(); 
                } else if (CheckCollisionPointRec(mousePoint, btnArcade)) {
                    currentState = STATE_DEBUG; 
                } else if (CheckCollisionPointRec(mousePoint, btnOptions)) {
                    // Futura lógica de Opções
                } else if (CheckCollisionPointRec(mousePoint, btnExit)) {
                    exitGame = true; 
                }
            }
        }

        BeginDrawing();
            
            if (currentState == STATE_TITLE || currentState == STATE_DEBUG) {
                ClearBackground(RAYWHITE);
            } else {
                ClearBackground(BLACK);
            }

            switch (currentState) {
                
                case STATE_SPLASH_FADE_IN:
                case STATE_SPLASH_CESAR:
                case STATE_FADE_OUT:
                {
                    Rectangle sourceRec = { 0.0f, 0.0f, (float)cesarLogo.width, (float)cesarLogo.height };
                    float scaledWidth = (float)cesarLogo.width * CESARlogoscale;
                    float scaledHeight = (float)cesarLogo.height * CESARlogoscale;
                    Rectangle destRec = {
                        (screenWidth - scaledWidth) / 2.0f,
                        (screenHeight - scaledHeight) / 2.0f,
                        scaledWidth,
                        scaledHeight
                    };
                    Vector2 origin = { 0.0f, 0.0f };
                    DrawTexturePro(cesarLogo, sourceRec, destRec, origin, 0.0f, WHITE);
                } break;

                case STATE_TITLE: {
                    int titleWidth = MeasureText("RECIFEWARE", 60);
                    DrawText("RECIFEWARE", (screenWidth - titleWidth) / 2, 150, 60, BLACK);

                    Vector2 mousePoint = GetMousePosition();
                    Rectangle btnMain    = { screenWidth/2 - 100, 300, 200, 40 };
                    Rectangle btnArcade  = { screenWidth/2 - 100, 360, 200, 40 };
                    Rectangle btnOptions = { screenWidth/2 - 100, 420, 200, 40 };
                    Rectangle btnExit    = { screenWidth/2 - 100, 480, 200, 40 };

                    DrawText("Main Game", btnMain.x + 40, btnMain.y + 10, 20, CheckCollisionPointRec(mousePoint, btnMain) ? BLUE : DARKGRAY);
                    DrawText("Testar AED", btnArcade.x + 40, btnArcade.y + 10, 20, CheckCollisionPointRec(mousePoint, btnArcade) ? RED : DARKGRAY);
                    DrawText("Options", btnOptions.x + 60, btnOptions.y + 10, 20, CheckCollisionPointRec(mousePoint, btnOptions) ? BLUE : DARKGRAY);
                    DrawText("Exit", btnExit.x + 80, btnExit.y + 10, 20, CheckCollisionPointRec(mousePoint, btnExit) ? BLUE : DARKGRAY);
                } break;

                // ==========================================
                // DEBUG
                // ==========================================
                case STATE_DEBUG: {
                    DrawText("DEBUG", 250, 40, 20, DARKGRAY);
                    DrawText("Teclas: [1] Criar Nós e Pesos | [2] Insertion Sort de Ponteiros | [ESPAÇO] Desenfileirar", 150, 80, 20, BLUE);
                    DrawText("FILA DINÂMICA NA MEMÓRIA:", 50, 250, 20, BLACK);
                    if (step >= 1) {
                        node_q *curr = front;
                        int idx = 0;
                        
                        while (curr != NULL) {
                            int blockX = 50 + (idx * 160);
                            int blockY = 300;

                            Color blockColor = (curr->weight == 9999) ? RED : ORANGE;

                            DrawRectangle(blockX, blockY, 120, 100, blockColor);
                            DrawRectangleLines(blockX, blockY, 120, 100, MAROON);
                            
                            DrawText(TextFormat("ID: %d", curr->id), blockX + 30, blockY + 20, 20, WHITE);
                            DrawText(TextFormat("W: %d", curr->weight), blockX + 25, blockY + 60, 20, BLACK);

                            if (curr->next != NULL) {
                                DrawLineEx((Vector2){(float)blockX + 120, (float)blockY + 50}, (Vector2){(float)blockX + 160, (float)blockY + 50}, 4.0f, BLACK);
                                DrawTriangle((Vector2){(float)blockX + 160, (float)blockY + 50}, (Vector2){(float)blockX + 150, (float)blockY + 40}, (Vector2){(float)blockX + 150, (float)blockY + 60}, BLACK);
                            }

                            curr = curr->next;
                            idx++;
                        }

                        if (front != NULL) DrawText("FRONT", 50, 420, 18, MAROON);
                        if (rear != NULL) DrawText("REAR", 50 + ((idx - 1) * 160), 420, 18, MAROON);
                    }
                } break;

                default: break;
            }
            
            if (fadeAlpha > 0 && currentState != STATE_DEBUG) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, fadeAlpha / 255.0f));
            }
            
        EndDrawing();
    }

    free_queue(&front, &rear);
    UnloadTexture(cesarLogo);
    CloseWindow();

    return 0;
}