#include "transition.h"
#include "raylib.h"
#include <math.h>

// ── Sprite sheet do leitor de disco ─────────────────────────────────────────
// discreader.png: 5 frames lado a lado, cada frame 59×79 px
#define DISC_FRAME_W     59
#define DISC_FRAME_H     79
#define DISC_FRAME_COUNT  5

// Sequência: abre → segura aberto → fecha → pausa fechado (índices dos frames)
static const int DISC_SEQ[] = { 0, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 0, 0, 0, 0, 0 };
#define DISC_SEQ_LEN 16

// Duração de cada passo da sequência (segundos)
#define DISC_FRAME_TIME  0.18f

// Quantos ciclos completos antes de iniciar o fade-out
#define DISC_LOOPS_TO_EXIT 1

// Escala dos sprites (pixels nativos → pixels de tela)
#define SPRITE_SCALE 4.0f

void RunTransitionScreen(void) {
    int W = 1200, H = 720;
    SetTargetFPS(60);

    // ── Carrega texturas ───────────────────────────────────────────────────────
    Texture2D texMonitor  = LoadTexture("../assets/sprites/monitor.png");
    Texture2D texKeyboard = LoadTexture("../assets/sprites/keyboard.png");
    Texture2D texDisc     = LoadTexture("../assets/sprites/discreader.png");
    Texture2D texPC       = LoadTexture("../assets/sprites/pc.png");

    // ── Layout ────────────────────────────────────────────────────────────────
    // Leitor  →  esquerda  (1/4 da tela)
    // Monitor →  centro    (1/2 da tela)
    // Teclado →  abaixo do monitor (centro)
    // Torre   →  direita   (3/4 da tela)
    float SC = SPRITE_SCALE;

    float pcW  = texPC.width      * SC,  pcH  = texPC.height      * SC;
    float monW = texMonitor.width  * SC,  monH = texMonitor.height  * SC;
    float kbW  = texKeyboard.width * SC,  kbH  = texKeyboard.height * SC;
    float dfW  = DISC_FRAME_W     * SC,  dfH  = DISC_FRAME_H      * SC;

    // Altura do bloco monitor+teclado para centralizar verticalmente
    float midGroupH = monH + 8.0f + kbH;
    float midGroupY = (H - midGroupH) / 2.0f;

    // Leitor — quarto esquerdo da tela
    float discX = W * 0.25f - dfW / 2.0f;
    float discY = H / 2.0f  - dfH / 2.0f;

    // Monitor — centro exato
    float monX = (W - monW) / 2.0f;
    float monY = midGroupY;

    // Teclado — abaixo do monitor, alinhado à esquerda dele
    float kbX  = monX;
    float kbY  = midGroupY + monH + 8.0f;

    // Torre — quarto direito da tela, alinhada verticalmente ao monitor
    float pcX  = W * 0.75f - pcW / 2.0f;
    float pcY  = midGroupY + (monH - pcH) / 2.0f;

    // ── Estado da animação ────────────────────────────────────────────────────
    int   discSeqIdx  = 0;
    float discTimer   = 0.0f;
    int   discLoops   = 0;

    // Fases: 0 = fade in | 1 = tocando | 2 = fade out | 3 = fim
    int   phase    = 0;
    float alpha    = 0.0f;
    float fadeSpd  = 1.5f;   // unidades por segundo (1.0 = ~0.67 s de fade)

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ── Atualização ───────────────────────────────────────────────────────
        switch (phase) {
            case 0:  // fade in
                alpha += fadeSpd * dt;
                if (alpha >= 1.0f) { alpha = 1.0f; phase = 1; }
                break;

            case 1:  // animação do leitor
                discTimer += dt;
                if (discTimer >= DISC_FRAME_TIME) {
                    discTimer -= DISC_FRAME_TIME;
                    discSeqIdx++;
                    if (discSeqIdx >= DISC_SEQ_LEN) {
                        discSeqIdx = 0;
                        discLoops++;
                    }
                }
                // Tecla qualquer adianta; após N ciclos avança sozinho
                if (discLoops >= DISC_LOOPS_TO_EXIT || IsKeyPressed(KEY_SPACE) ||
                    IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    phase = 2;
                }
                break;

            case 2:  // fade out
                alpha -= fadeSpd * dt;
                if (alpha <= 0.0f) { alpha = 0.0f; phase = 3; }
                break;

            default:
                goto done;  // sai do loop
        }

        // ── Desenho ───────────────────────────────────────────────────────────
        Color tint = { 255, 255, 255, (unsigned char)(alpha * 255.0f) };

        BeginDrawing();
        ClearBackground(BLACK);

        // Leitor animado (esquerda)
        int frameIdx = DISC_SEQ[discSeqIdx];
        Rectangle srcDisc = {
            (float)(frameIdx * DISC_FRAME_W), 0.0f,
            (float)DISC_FRAME_W, (float)DISC_FRAME_H
        };
        DrawTexturePro(texDisc, srcDisc,
                       (Rectangle){ discX, discY, dfW, dfH },
                       (Vector2){ 0, 0 }, 0.0f, tint);

        // Torre PC (estático)
        DrawTexturePro(texPC,
                       (Rectangle){ 0, 0, (float)texPC.width, (float)texPC.height },
                       (Rectangle){ pcX, pcY, pcW, pcH },
                       (Vector2){ 0, 0 }, 0.0f, tint);

        // Monitor (estático)
        DrawTexturePro(texMonitor,
                       (Rectangle){ 0, 0, (float)texMonitor.width, (float)texMonitor.height },
                       (Rectangle){ monX, monY, monW, monH },
                       (Vector2){ 0, 0 }, 0.0f, tint);

        // Teclado (estático)
        DrawTexturePro(texKeyboard,
                       (Rectangle){ 0, 0, (float)texKeyboard.width, (float)texKeyboard.height },
                       (Rectangle){ kbX, kbY, kbW, kbH },
                       (Vector2){ 0, 0 }, 0.0f, tint);

        EndDrawing();
    }

done:
    UnloadTexture(texMonitor);
    UnloadTexture(texKeyboard);
    UnloadTexture(texDisc);
    UnloadTexture(texPC);
}