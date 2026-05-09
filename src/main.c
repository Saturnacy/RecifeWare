#include "raylib.h"

typedef enum {
    STATE_SPLASH_FADE_IN,
    STATE_SPLASH_CESAR,
    STATE_FADE_OUT,
    STATE_INTRO,
    STATE_TITLE
} Gamestate;

int main() {
    int screenWidth = 1200;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "RecifeWare");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Placeholder", 600, 300, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}