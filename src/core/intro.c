#include "intro.h"
#include "font.h"
#include "raylib.h"
#include <math.h>

#define INTRO_FPS        60
#define FADE_SPEED       4.0f
#define FRAME_HOLD       2.0f
#define STATIC_HOLD      3.0f

#define EASE_OUT(t) (1.0f - powf(1.0f - (t), 3.0f))
#define EASE_IN(t) ((t) * (t) * (t))

typedef enum {
    INTRO_FADE_IN,
    INTRO_HOLD,
    INTRO_FADE_OUT,
    INTRO_NEXT
} IntroPhase;

void RunIntro(void) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    Texture2D bg = LoadTexture("assets/sprites/intro/intro1.png");
    Texture2D sheet = LoadTexture("assets/sprites/intro/intro2.png");
    Texture2D tv = LoadTexture("assets/sprites/intro/intro3.png");
    Texture2D bg5 = LoadTexture("assets/sprites/intro/fundo_intro5.png");
    Texture2D cap = LoadTexture("assets/sprites/intro/cap1.png");
    Texture2D cap2 = LoadTexture("assets/sprites/intro/cap2.png");
    Texture2D severino = LoadTexture("assets/sprites/intro/severino.png");
    Texture2D dudu = LoadTexture("assets/sprites/intro/dudu.png");
    Texture2D meia = LoadTexture("assets/sprites/intro/meia.png");
    Texture2D lua = LoadTexture("assets/sprites/intro/lua.png");
    Texture2D ubee = LoadTexture("assets/sprites/intro/ubee.png");
    Texture2D galafuz = LoadTexture("assets/sprites/intro/galafuz.png");
    Texture2D cobalto = LoadTexture("assets/sprites/intro/cobalto.png");

    int totalFrames = 7;
    int frameW = sheet.width;
    int frameH = sheet.height / totalFrames;

    int tvTotalFrames = 2;
    int tvFrameW = tv.width;
    int tvFrameH = tv.height / tvTotalFrames;

    int scene = 0;
    int frameIdx = 0;
    int tvFrameIdx = 0;

    IntroPhase phase = INTRO_FADE_IN;
    float fadeAlpha = 255.0f;
    float holdTimer = 0.0f;

    int currentText = 0;
    int charCount = 0;
    float textTimer = 0.0f;
    float animTimer = 0.0f;
    
    const char* lines[6] = {
        "Impressionante como essa cidade está tão deserta...",
        "se apenas algo legal acontecesse...",
        "E agora, com a notícia de hoje!",
        "A criançada adora, e esses números comprovam!\nO mercado de videogames movimenta milhoes de reais na\ncidade do Recife, e agora introduz a cidade aos\nestrangeiros no formato de entretenimento!",
        "Será que este mercado vai continuar ganhando força nessa cidade?",
        "Video...games! É isso! Mas como eu\nposso usar isso para a minha vantagem...?"
    };

    float barHeight = sh * 0.15f;

    SetTargetFPS(INTRO_FPS);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            break;
        }

        switch (phase) {
            case INTRO_FADE_IN:
                fadeAlpha -= FADE_SPEED * (255.0f / INTRO_FPS);
                if (fadeAlpha <= 0.0f) {
                    fadeAlpha = 0.0f;
                    phase = INTRO_HOLD;
                    holdTimer = 0.0f;
                }
                break;

            case INTRO_HOLD:
                if (scene == 0 || scene == 1 || scene == 3 || scene == 6 || scene == 7 || scene == 8) {
                    holdTimer += dt;
                    float limit = 0.0f;
                    if (scene == 0) limit = STATIC_HOLD;
                    else if (scene == 6) limit = 1.0f;
                    else if (scene == 7) limit = 4.0f;
                    else if (scene == 8) limit = 12.5f;
                    else limit = FRAME_HOLD;

                    if (holdTimer >= limit) {
                        phase = INTRO_FADE_OUT;
                    }
                } else if (scene == 2 || scene == 4 || scene == 5) {
                    int maxChars = TextLength(lines[currentText]);
                    if (charCount < maxChars) {
                        textTimer += dt;
                        animTimer += dt;
                        if (animTimer > 0.2f) {
                            if (scene == 2) frameIdx = (frameIdx == 2) ? 3 : 2;
                            if (scene == 4) tvFrameIdx = (tvFrameIdx == 0) ? 1 : 0;
                            animTimer = 0.0f;
                        }
                        if (textTimer > 0.05f) {
                            charCount++;
                            textTimer = 0.0f;
                        }
                    } else {
                        if (scene == 2) frameIdx = 2;
                        if (scene == 4) tvFrameIdx = 0;
                        
                        holdTimer += dt;
                        if (holdTimer >= 2.0f) {
                            if (scene == 2 && currentText == 0) {
                                currentText = 1;
                                charCount = 0;
                                holdTimer = 0.0f;
                            } else if (scene == 4 && currentText < 4) {
                                currentText++;
                                charCount = 0;
                                holdTimer = 0.0f;
                            } else if (scene == 5) {
                                scene = 6;
                                frameIdx = 6;
                                holdTimer = 0.0f;
                            } else {
                                phase = INTRO_FADE_OUT;
                            }
                        }
                    }
                }
                break;

            case INTRO_FADE_OUT:
                fadeAlpha += FADE_SPEED * (255.0f / INTRO_FPS);
                if (fadeAlpha >= 255.0f) {
                    fadeAlpha = 255.0f;
                    phase = INTRO_NEXT;
                }
                break;

            case INTRO_NEXT:
                if (scene == 0) {
                    scene = 1;
                    frameIdx = 0;
                } else if (scene == 1) {
                    frameIdx++;
                    if (frameIdx > 1) {
                        scene = 2;
                        frameIdx = 2;
                        currentText = 0;
                        charCount = 0;
                        textTimer = 0.0f;
                        animTimer = 0.0f;
                    }
                } else if (scene == 2) {
                    scene = 3;
                    frameIdx = 4;
                } else if (scene == 3) {
                    scene = 4;
                    tvFrameIdx = 0;
                    currentText = 2;
                    charCount = 0;
                    textTimer = 0.0f;
                    animTimer = 0.0f;
                } else if (scene == 4) {
                    scene = 5;
                    frameIdx = 5;
                    currentText = 5;
                    charCount = 0;
                    textTimer = 0.0f;
                    animTimer = 0.0f;
                } else if (scene == 6) {
                    scene = 7;
                } else if (scene == 7) {
                    scene = 8;
                } else if (scene == 8) {
                    goto intro_done;
                }
                phase = INTRO_FADE_IN;
                fadeAlpha = 255.0f;
                holdTimer = 0.0f;
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Rectangle dest = {0, barHeight, (float)sw, (float)sh - 2.0f * barHeight};

        if (scene == 0) {
            DrawTexturePro(bg, (Rectangle){0, 0, (float)bg.width, (float)bg.height}, dest, (Vector2){0, 0}, 0.0f, WHITE);
        } else if (scene == 1 || scene == 2 || scene == 3 || scene == 5 || scene == 6) {
            Rectangle src = { 0, (float)(frameIdx * frameH), (float)frameW, (float)frameH };
            DrawTexturePro(sheet, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        } else if (scene == 4) {
            Rectangle src = { 0, (float)(tvFrameIdx * tvFrameH), (float)tvFrameW, (float)tvFrameH };
            DrawTexturePro(tv, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        } else if (scene == 7) {
            DrawTexturePro(bg5, (Rectangle){0, 0, (float)bg5.width, (float)bg5.height}, (Rectangle){0, 0, (float)sw, (float)sh}, (Vector2){0, 0}, 0.0f, WHITE);

            float capScale = (sh * 0.9f) / cap.height;
            float cw = cap.width * capScale;
            float ch = cap.height * capScale;
            
            float capCenterX = (sw - cw) / 2.0f;
            float capRightX = sw - cw + 150.0f;

            Font font = GetGameFont();
            const char* animTxt = "EU VOU CHAMAR OS MEUS\nAMIGOS PARA FAZER GAMES!";
            Vector2 tSize = MeasureTextEx(font, animTxt, 45, 2);

            float txtStartX = -tSize.x - 50.0f;
            float txtEndX = 80.0f;

            float cx = capCenterX;
            float tx = txtStartX;

            if (holdTimer >= 1.0f) {
                float t = (holdTimer - 1.0f) / 0.5f;
                if (t > 1.0f) t = 1.0f;
                cx = capCenterX + (capRightX - capCenterX) * EASE_OUT(t);
                tx = txtStartX + (txtEndX - txtStartX) * EASE_OUT(t);
            }

            DrawTexturePro(cap, (Rectangle){0, 0, (float)cap.width, (float)cap.height}, (Rectangle){cx, sh - ch, cw, ch}, (Vector2){0, 0}, 0.0f, WHITE);
            DrawTextEx(font, animTxt, (Vector2){tx, (sh - tSize.y) / 2.0f}, 45, 2, WHITE);
        } else if (scene == 8) {
            DrawTexturePro(bg5, (Rectangle){0, 0, (float)bg5.width, (float)bg5.height}, (Rectangle){0, 0, (float)sw, (float)sh}, (Vector2){0, 0}, 0.0f, WHITE);

            int aFrame = (int)(GetTime() * 10.0f) % 2;
            Font iFont = GetIntroFont();

            float c2w = (cap2.width / 2.0f) * ((sh * 0.85f) / cap2.height);
            float c2h = cap2.height * ((sh * 0.85f) / cap2.height);
            float svw = (severino.width / 2.0f) * ((sh * 0.85f) / severino.height);
            float svh = severino.height * ((sh * 0.85f) / severino.height);
            float ddw = (dudu.width / 2.0f) * ((sh * 0.85f) / dudu.height);
            float ddh = dudu.height * ((sh * 0.85f) / dudu.height);
            float mw = (meia.width / 2.0f) * ((sh * 0.85f) / meia.height);
            float mh = meia.height * ((sh * 0.85f) / meia.height);
            float lw = (lua.width / 2.0f) * ((sh * 0.85f) / lua.height);
            float lh = lua.height * ((sh * 0.85f) / lua.height);
            float uw = (ubee.width / 2.0f) * ((sh * 0.85f) / ubee.height);
            float uh = ubee.height * ((sh * 0.85f) / ubee.height);
            float gw = (galafuz.width / 2.0f) * ((sh * 0.85f) / galafuz.height);
            float gh = galafuz.height * ((sh * 0.85f) / galafuz.height);
            float cbw = (cobalto.width / 2.0f) * ((sh * 0.85f) / cobalto.height);
            float cbh = cobalto.height * ((sh * 0.85f) / cobalto.height);

            float capDestX = sw * 0.35f - c2w / 2.0f;
            float sevDestX = sw * 0.70f - svw / 2.0f;
            float duduDestX = sw * 0.35f - ddw / 2.0f;
            float meiaDestX = sw * 0.40f - mw / 2.0f;
            float luaDestX = sw * 0.60f - lw / 2.0f;
            float ubeeDestX = sw * 0.50f - uw / 2.0f;
            float galaDestX = sw * 0.35f - gw / 2.0f;
            float cobaDestX = sw * 0.70f - cbw / 2.0f;

            if (holdTimer < 3.0f) {
                float cX = -c2w, sX = sw;
                if (holdTimer < 0.5f) cX = -c2w + (capDestX - (-c2w)) * EASE_OUT(holdTimer / 0.5f);
                else if (holdTimer < 2.5f) cX = capDestX;
                else cX = capDestX + (sw - capDestX) * EASE_IN((holdTimer - 2.5f) / 0.5f);
                
                if (holdTimer >= 0.5f) {
                    if (holdTimer < 1.0f) sX = sw + (sevDestX - sw) * EASE_OUT((holdTimer - 0.5f) / 0.5f);
                    else if (holdTimer < 2.5f) sX = sevDestX;
                    else sX = sevDestX + (sw - sevDestX) * EASE_IN((holdTimer - 2.5f) / 0.5f);
                }

                DrawTexturePro(cap2, (Rectangle){aFrame * (cap2.width / 2.0f), 0, cap2.width / 2.0f, cap2.height}, (Rectangle){cX, sh - c2h, c2w, c2h}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTextEx(iFont, "CAP-YBARA", (Vector2){cX + c2w / 2.0f - MeasureTextEx(iFont, "CAP-YBARA", 60, 2).x / 2.0f, sh - c2h - 80.0f}, 60, 2, WHITE);

                if (holdTimer >= 0.5f) {
                    DrawTexturePro(severino, (Rectangle){aFrame * (severino.width / 2.0f), 0, severino.width / 2.0f, severino.height}, (Rectangle){sX, sh - svh, svw, svh}, (Vector2){0, 0}, 0.0f, WHITE);
                    DrawTextEx(iFont, "SEVERINO", (Vector2){sX + svw / 2.0f - MeasureTextEx(iFont, "SEVERINO", 60, 2).x / 2.0f, sh - svh - 80.0f}, 60, 2, WHITE);
                }
            }

            if (holdTimer >= 3.0f && holdTimer < 5.5f) {
                float t = holdTimer - 3.0f;
                float dX = -ddw;
                if (t < 0.5f) dX = -ddw + (duduDestX - (-ddw)) * EASE_OUT(t / 0.5f);
                else if (t < 2.0f) dX = duduDestX;
                else dX = duduDestX + (-ddw - duduDestX) * EASE_IN((t - 2.0f) / 0.5f);

                DrawTexturePro(dudu, (Rectangle){aFrame * (dudu.width / 2.0f), 0, dudu.width / 2.0f, dudu.height}, (Rectangle){dX, sh - ddh, ddw, ddh}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTextEx(iFont, "DUDU", (Vector2){dX + ddw / 2.0f - MeasureTextEx(iFont, "DUDU", 60, 2).x / 2.0f, sh - ddh - 80.0f}, 60, 2, WHITE);
            }

            if (holdTimer >= 5.5f && holdTimer < 8.0f) {
                float t = holdTimer - 5.5f;
                float mX = sw, lX = sw + (luaDestX - meiaDestX);
                if (t < 0.5f) {
                    mX = sw + (meiaDestX - sw) * EASE_OUT(t / 0.5f);
                    lX = sw + (luaDestX - sw) * EASE_OUT(t / 0.5f);
                } else if (t < 2.0f) {
                    mX = meiaDestX;
                    lX = luaDestX;
                } else {
                    mX = meiaDestX + (sw - meiaDestX) * EASE_IN((t - 2.0f) / 0.5f);
                    lX = luaDestX + (sw - luaDestX) * EASE_IN((t - 2.0f) / 0.5f);
                }

                DrawTexturePro(meia, (Rectangle){aFrame * (meia.width / 2.0f), 0, meia.width / 2.0f, meia.height}, (Rectangle){mX, sh - mh, mw, mh}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTexturePro(lua, (Rectangle){aFrame * (lua.width / 2.0f), 0, lua.width / 2.0f, lua.height}, (Rectangle){lX, sh - lh, lw, lh}, (Vector2){0, 0}, 0.0f, WHITE);
                
                float centerML = mX + (lX + lw - mX) / 2.0f;
                DrawTextEx(iFont, "MEIA E LUA", (Vector2){centerML - MeasureTextEx(iFont, "MEIA E LUA", 60, 2).x / 2.0f, sh - mh - 80.0f}, 60, 2, WHITE);
            }

            if (holdTimer >= 8.0f && holdTimer < 10.5f) {
                float t = holdTimer - 8.0f;
                float uX = sw;
                if (t < 0.5f) uX = sw + (ubeeDestX - sw) * EASE_OUT(t / 0.5f);
                else if (t < 2.0f) uX = ubeeDestX;
                else uX = ubeeDestX + (sw - ubeeDestX) * EASE_IN((t - 2.0f) / 0.5f);

                DrawTexturePro(ubee, (Rectangle){aFrame * (ubee.width / 2.0f), 0, ubee.width / 2.0f, ubee.height}, (Rectangle){uX, sh - uh, uw, uh}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTextEx(iFont, "U-BEE", (Vector2){uX + uw / 2.0f - MeasureTextEx(iFont, "U-BEE", 60, 2).x / 2.0f, sh - uh - 80.0f}, 60, 2, WHITE);
            }

            if (holdTimer >= 10.5f) {
                float t = holdTimer - 10.5f;
                float gX = -gw, cX = sw;
                if (t < 0.5f) {
                    gX = -gw + (galaDestX - (-gw)) * EASE_OUT(t / 0.5f);
                    cX = sw + (cobaDestX - sw) * EASE_OUT(t / 0.5f);
                } else {
                    gX = galaDestX;
                    cX = cobaDestX;
                }

                DrawTexturePro(galafuz, (Rectangle){aFrame * (galafuz.width / 2.0f), 0, galafuz.width / 2.0f, galafuz.height}, (Rectangle){gX, sh - gh, gw, gh}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTexturePro(cobalto, (Rectangle){aFrame * (cobalto.width / 2.0f), 0, cobalto.width / 2.0f, cobalto.height}, (Rectangle){cX, sh - cbh, cbw, cbh}, (Vector2){0, 0}, 0.0f, WHITE);
                
                DrawTextEx(iFont, "GALAFUZ", (Vector2){gX + gw / 2.0f - MeasureTextEx(iFont, "GALAFUZ", 60, 2).x / 2.0f, sh - gh - 80.0f}, 60, 2, WHITE);
                DrawTextEx(iFont, "COBALTO", (Vector2){cX + cbw / 2.0f - MeasureTextEx(iFont, "COBALTO", 60, 2).x / 2.0f, sh - cbh - 80.0f}, 60, 2, WHITE);
            }
        }

        if (scene == 2 || scene == 4 || scene == 5) {
            Font font = GetGameFont();
            const char* currentStr = TextSubtext(lines[currentText], 0, charCount);
            float fSize = (scene == 4) ? 24.0f : 30.0f;
            Vector2 textSize = MeasureTextEx(font, currentStr, fSize, 2);
            float textY = sh - barHeight + (barHeight - textSize.y) / 2.0f;
            DrawTextEx(font, currentStr, (Vector2){ (sw - textSize.x) / 2.0f, textY }, fSize, 2, WHITE);
        }

        if (phase == INTRO_FADE_OUT && scene == 8) {
            DrawRectangle(0, 0, sw, sh, Fade(WHITE, fadeAlpha / 255.0f));
        } else if (fadeAlpha > 0.0f) {
            DrawRectangle(0, 0, sw, sh, Fade(BLACK, fadeAlpha / 255.0f));
        }

        EndDrawing();
    }

intro_done:
    UnloadTexture(bg);
    UnloadTexture(sheet);
    UnloadTexture(tv);
    UnloadTexture(bg5);
    UnloadTexture(cap);
    UnloadTexture(cap2);
    UnloadTexture(severino);
    UnloadTexture(dudu);
    UnloadTexture(meia);
    UnloadTexture(lua);
    UnloadTexture(ubee);
    UnloadTexture(galafuz);
    UnloadTexture(cobalto);
}