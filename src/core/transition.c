#include "transition.h"
#include "raylib.h"

#define DISC_FRAME_COUNT 5
static const int DISC_SEQ[]={0,1,2,3,4,4,4,4,3,2,1,0,0,0,0,0};
#define DISC_SEQ_LEN 16
#define DISC_FRAME_TIME    0.18f
#define DISC_LOOPS_TO_EXIT 1
#define SPRITE_SCALE       4.0f

void RunTransitionScreen(void) {
    int W=1200,H=720;
    SetTargetFPS(60);
    Texture2D texMonitor=LoadTexture("../assets/sprites/monitor.png");
    Texture2D texKeyboard=LoadTexture("../assets/sprites/keyboard.png");
    Texture2D texPC=LoadTexture("../assets/sprites/pc.png");
    Texture2D discFrames[DISC_FRAME_COUNT];
    discFrames[0]=LoadTexture("../assets/sprites/discreader/0.png");
    discFrames[1]=LoadTexture("../assets/sprites/discreader/1.png");
    discFrames[2]=LoadTexture("../assets/sprites/discreader/2.png");
    discFrames[3]=LoadTexture("../assets/sprites/discreader/3.png");
    discFrames[4]=LoadTexture("../assets/sprites/discreader/4.png");
    float SC=SPRITE_SCALE;
    float pcW=texPC.width*SC,pcH=texPC.height*SC;
    float monW=texMonitor.width*SC,monH=texMonitor.height*SC;
    float kbW=texKeyboard.width*SC,kbH=texKeyboard.height*SC;
    float midGroupH=monH+8.0f+kbH;
    float midGroupY=(H-midGroupH)/2.0f;
    float monX=(W-monW)/2.0f;
    float monY=midGroupY;
    float kbX=monX;
    float kbY=midGroupY+monH+8.0f;
    float pcX=W*0.78f-pcW/2.0f;
    float pcY=midGroupY+(monH-pcH)/2.0f;
    float discFrameW=64.0f*SC;
    float discFrameH=79.0f*SC;
    float discAnchorX=W*0.10f;
    float discAnchorY=H/2.0f+(monH/2.0f);
    int discSeqIdx=0;
    float discTimer=0.0f;
    int discLoops=0;
    int phase=0;
    float alpha=0.0f;
    float fadeSpd=1.5f;
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        switch (phase) {
            case 0:
                alpha+=fadeSpd*dt;
                if (alpha>=1.0f) { alpha=1.0f; phase=1; }
                break;
            case 1:
                discTimer+=dt;
                if (discTimer>=DISC_FRAME_TIME) {
                    discTimer-=DISC_FRAME_TIME;
                    discSeqIdx++;
                    if (discSeqIdx>=DISC_SEQ_LEN) {
                        discSeqIdx=0;
                        discLoops++;
                    }
                }
                if (discLoops>=DISC_LOOPS_TO_EXIT || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    phase=2;
                }
                break;
            case 2:
                alpha-=fadeSpd*dt;
                if (alpha<=0.0f) { alpha=0.0f; phase=3; }
                break;
            default:
                goto done;
        }
        Color tint={255,255,255,(unsigned char)(alpha*255.0f)};
        BeginDrawing();
        ClearBackground(BLACK);
        {
            Texture2D *fr=&discFrames[DISC_SEQ[discSeqIdx]];
            DrawTexturePro(*fr,(Rectangle){0,0,(float)fr->width,(float)fr->height},(Rectangle){discAnchorX,discAnchorY-discFrameH,discFrameW,discFrameH},(Vector2){0,0},0.0f,tint);
        }
        DrawTexturePro(texPC,(Rectangle){0,0,(float)texPC.width,(float)texPC.height},(Rectangle){pcX,pcY,pcW,pcH},(Vector2){0,0},0.0f,tint);
        DrawTexturePro(texMonitor,(Rectangle){0,0,(float)texMonitor.width,(float)texMonitor.height},(Rectangle){monX,monY,monW,monH},(Vector2){0,0},0.0f,tint);
        DrawTexturePro(texKeyboard,(Rectangle){0,0,(float)texKeyboard.width,(float)texKeyboard.height},(Rectangle){kbX,kbY,kbW,kbH},(Vector2){0,0},0.0f,tint);
        EndDrawing();
    }
done:
    for (int i=0;i<DISC_FRAME_COUNT;i++) UnloadTexture(discFrames[i]);
    UnloadTexture(texMonitor);
    UnloadTexture(texKeyboard);
    UnloadTexture(texPC);
}