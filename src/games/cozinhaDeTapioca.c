#include "cozinhaDeTapioca.h"
#include "timer_bomba.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

#define TEMPO_PAVIO      3.0f
#define TEMPO_BOMBA_FUSE 1.5f

typedef struct {
    float origX, origY;
    float x, y;
    float w, h;
    char nome[32];
    int colocado;
    Texture2D tex;
} Ingrediente;

static void DrawIngrediente(Ingrediente *ing) {
    DrawRectangleRounded((Rectangle){ing->x-6,ing->y-6,ing->w+12,ing->h+12},0.3f,4,(Color){255,245,220,160});
    DrawTexturePro(ing->tex,(Rectangle){0,0,(float)ing->tex.width,(float)ing->tex.height},(Rectangle){ing->x,ing->y,ing->w,ing->h},(Vector2){0,0},0.0f,WHITE);
    int lw=MeasureText(ing->nome,14);
    DrawText(ing->nome,(int)(ing->x+ing->w/2.0f)-lw/2,(int)(ing->y+ing->h+5),14,DARKBROWN);
}

void cozinhaDeTapioca() {
    int screenWidth=1200;
    int screenHeight=720;
    SetTargetFPS(60);
    int flag=0;
    int errou=0;
    int frames=120;
    Texture2D texFundo=LoadTexture("../assets/sprites/cozinhaDeTapioca/Tapioca_com_tapioca.png");
    Texture2D texTapioca=LoadTexture("../assets/sprites/cozinhaDeTapioca/tapioca_ingrediente.png");
    Texture2D texCarne=LoadTexture("../assets/sprites/cozinhaDeTapioca/carne.png");
    Texture2D texPeixe=LoadTexture("../assets/sprites/cozinhaDeTapioca/peixe.png");
    Texture2D texAlface=LoadTexture("../assets/sprites/cozinhaDeTapioca/alface.png");
    Rectangle srcFundo={0,0,(float)texFundo.width,(float)texFundo.height};
    Rectangle dstFundo={0,0,(float)screenWidth,(float)screenHeight};
    float tapScale=3.5f;
    float tapW=texTapioca.width*tapScale;
    float tapH=texTapioca.height*tapScale;
    float tapX=screenWidth*0.58f;
    float tapY=(screenHeight-tapH)/2.0f;
    float reduzirW=tapW*0.20f;
    float reduzirH=tapH*0.20f;
    Rectangle tapiocaColisor={tapX+reduzirW/2.0f,tapY+reduzirH/2.0f,tapW-reduzirW,tapH-reduzirH};
    float ingScale=2.0f;
    float cW=texCarne.width*ingScale;
    float cH=texCarne.height*ingScale;
    float pW=texPeixe.width*ingScale;
    float pH=texPeixe.height*ingScale;
    float aW=texAlface.width*ingScale;
    float aH=texAlface.height*ingScale;
    float tabulaX=163.0f;
    float tabulaW=598.0f;
    float tabulaY=134.0f;
    float tabulaH=454.0f;
    float margem=20.0f;
    float ws[3]={cW,pW,aW};
    float hs[3]={cH,pH,aH};
    float posX[3],posY[3];
    for (int i=0;i<3;i++) {
        int tentativas=0;
        int ok=0;
        while (!ok && tentativas<200) {
            tentativas++;
            float rx=tabulaX+margem+(float)(rand()%(int)(tabulaW-ws[i]-margem*2));
            float ry=tabulaY+margem+(float)(rand()%(int)(tabulaH-hs[i]-margem*2));
            ok=1;
            for (int j=0;j<i;j++) {
                if (rx<posX[j]+ws[j]+margem && rx+ws[i]>posX[j]-margem && ry<posY[j]+hs[j]+margem && ry+hs[i]>posY[j]-margem) {
                    ok=0;
                    break;
                }
            }
            if (ok) { posX[i]=rx; posY[i]=ry; }
        }
    }
    Ingrediente ingredientes[3]={
        {posX[0],posY[0],posX[0],posY[0],cW,cH,"Carne",0,texCarne},
        {posX[1],posY[1],posX[1],posY[1],pW,pH,"Peixe",0,texPeixe},
        {posX[2],posY[2],posX[2],posY[2],aW,aH,"Alface",0,texAlface},
    };
    int totalIngredientes=3;
    int pedido=rand()%totalIngredientes;
    int acertos=0;
    int arrastando=-1;
    float dragOffX=0.0f;
    float dragOffY=0.0f;
    TimerBomba timer;
    InitTimerBomba(&timer,TEMPO_PAVIO,TEMPO_BOMBA_FUSE);
    while (timer.fase==FASE_PAVIO && !errou && acertos<1 && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);
        Vector2 mouse=GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i=0;i<totalIngredientes;i++) {
                if (ingredientes[i].colocado) continue;
                Rectangle r={ingredientes[i].x,ingredientes[i].y,ingredientes[i].w,ingredientes[i].h};
                if (CheckCollisionPointRec(mouse,r)) {
                    arrastando=i;
                    dragOffX=mouse.x-ingredientes[i].x;
                    dragOffY=mouse.y-ingredientes[i].y;
                    break;
                }
            }
        }
        if (arrastando>=0) {
            ingredientes[arrastando].x=mouse.x-dragOffX;
            ingredientes[arrastando].y=mouse.y-dragOffY;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && arrastando>=0) {
            Rectangle rIng={ingredientes[arrastando].x,ingredientes[arrastando].y,ingredientes[arrastando].w,ingredientes[arrastando].h};
            if (CheckCollisionRecs(rIng,tapiocaColisor)) {
                if (arrastando==pedido) {
                    ingredientes[arrastando].colocado=1;
                    acertos++;
                } else {
                    errou=1;
                }
            } else {
                ingredientes[arrastando].x=ingredientes[arrastando].origX;
                ingredientes[arrastando].y=ingredientes[arrastando].origY;
            }
            arrastando=-1;
        }
        BeginDrawing();
            DrawTexturePro(texFundo,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
            DrawRectangleRounded((Rectangle){20,20,260,90},0.2f,4,(Color){0,0,0,140});
            DrawText("Pedido:",35,32,22,WHITE);
            DrawText(ingredientes[pedido].nome,35,62,22,YELLOW);
            DrawTexturePro(texTapioca,(Rectangle){0,0,(float)texTapioca.width,(float)texTapioca.height},(Rectangle){tapX,tapY,tapW,tapH},(Vector2){0,0},0.0f,WHITE);
            for (int i=0;i<totalIngredientes;i++) {
                if (ingredientes[i].colocado) continue;
                if (i==arrastando) continue;
                DrawIngrediente(&ingredientes[i]);
            }
            if (arrastando>=0) DrawIngrediente(&ingredientes[arrastando]);
            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }
    if (acertos>=1) flag=1;
    if (!errou && !flag) {
        while (!TimerTerminou(&timer) && !WindowShouldClose()) {
            UpdateTimerBomba(&timer);
            BeginDrawing();
                DrawTexturePro(texFundo,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
                DrawTimerBomba(&timer,screenWidth,screenHeight);
            EndDrawing();
        }
    }
    if (flag) {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                DrawTexturePro(texFundo,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
                int ts=40,lw=MeasureText("Vitoria",ts);
                DrawText("Vitoria",(screenWidth/2)-(lw/2),(screenHeight/2)-(ts/2),ts,GREEN);
            EndDrawing();
        }
    } else {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                DrawTexturePro(texFundo,srcFundo,dstFundo,(Vector2){0,0},0.0f,WHITE);
                int ts=40,lw=MeasureText("Derrota",ts);
                DrawText("Derrota",(screenWidth/2)-(lw/2),(screenHeight/2)-(ts/2),ts,RED);
            EndDrawing();
        }
    }
    UnloadTexture(texFundo);
    UnloadTexture(texTapioca);
    UnloadTexture(texCarne);
    UnloadTexture(texPeixe);
    UnloadTexture(texAlface);
    UnloadTimerBomba(&timer);
}