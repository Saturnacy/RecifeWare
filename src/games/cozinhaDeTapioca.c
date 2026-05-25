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
    Color cor;
    int colocado;
} Ingrediente;

void cozinhaDeTapioca() {
    int screenWidth=1200;
    int screenHeight=720;

    SetTargetFPS(60);

    int flag=0;
    int errou=0;
    int frames=120;

    Ingrediente ingredientes[6]={
        {900,150,900,150,160,60,"Queijo",YELLOW,0},
        {900,240,900,240,160,60,"Presunto",(Color){255,150,150,255},0},
        {900,330,900,330,160,60,"Coco",(Color){240,240,220,255},0},
        {900,420,900,420,160,60,"Banana",(Color){255,230,80,255},0},
        {900,510,900,510,160,60,"Frango",ORANGE,0},
        {900,600,900,600,160,60,"Doce de leite",(Color){180,100,40,255},0}
    };

    int totalIngredientes=6;
    int pedido[2];
    pedido[0]=rand()%totalIngredientes;
    do {
        pedido[1]=rand()%totalIngredientes;
    } while (pedido[1]==pedido[0]);

    int acertos=0;

    Rectangle tapioca={350,220,300,220};
    int arrastando=-1;
    float dragOffsetX=0.0f;
    float dragOffsetY=0.0f;

    TimerBomba timer;
    InitTimerBomba(&timer,TEMPO_PAVIO,TEMPO_BOMBA_FUSE);

    while (timer.fase==FASE_PAVIO && !errou && acertos<2 && !WindowShouldClose()) {
        UpdateTimerBomba(&timer);

        Vector2 mouse=GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i=0;i<totalIngredientes;i++) {
                if (ingredientes[i].colocado) continue;
                Rectangle rect={ingredientes[i].x,ingredientes[i].y,ingredientes[i].w,ingredientes[i].h};
                if (CheckCollisionPointRec(mouse,rect)) {
                    arrastando=i;
                    
                    dragOffsetX=mouse.x-ingredientes[i].x;
                    dragOffsetY=mouse.y-ingredientes[i].y;
                    break;
                }
            }
        }

        if (arrastando>=0) {
            ingredientes[arrastando].x=mouse.x-dragOffsetX;
            ingredientes[arrastando].y=mouse.y-dragOffsetY;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && arrastando>=0) {
            Rectangle rectIng={ingredientes[arrastando].x,ingredientes[arrastando].y,ingredientes[arrastando].w,ingredientes[arrastando].h};

            if (CheckCollisionRecs(rectIng,tapioca)) {
                int correto=(arrastando==pedido[0]||arrastando==pedido[1]);
                if (correto) {
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
            ClearBackground((Color){255,245,220,255});
            DrawRectangle(20,20,300,110,(Color){255,220,150,255});

            DrawRectangleLines(20,20,300,110,BROWN);
            DrawText("Pedido:",35,35,22,BROWN);

            DrawText(ingredientes[pedido[0]].nome,35,65,20,DARKBROWN);
            DrawText(ingredientes[pedido[1]].nome,35,90,20,DARKBROWN);

            DrawEllipse((int)(tapioca.x+tapioca.width/2.0f),(int)(tapioca.y+tapioca.height/2.0f),(int)(tapioca.width/2.0f),(int)(tapioca.height/2.0f),(Color){245,240,220,255});
            DrawEllipseLines((int)(tapioca.x+tapioca.width/2.0f),(int)(tapioca.y+tapioca.height/2.0f),(int)(tapioca.width/2.0f),(int)(tapioca.height/2.0f),LIGHTGRAY);
            
            int larguraMassa=MeasureText("Massa",20);
            DrawText("Massa",(int)(tapioca.x+tapioca.width/2.0f)-larguraMassa/2,(int)(tapioca.y+tapioca.height/2.0f)-10,20,LIGHTGRAY);
            int offsetColocado=0;

            for (int i=0;i<totalIngredientes;i++) {
                if (!ingredientes[i].colocado) continue;
                DrawText(ingredientes[i].nome,(int)(tapioca.x+tapioca.width/2.0f)-MeasureText(ingredientes[i].nome,20)/2,(int)(tapioca.y+tapioca.height/2.0f)+offsetColocado,20,ingredientes[i].cor);
                offsetColocado+=25;
            }
            for (int i=0;i<totalIngredientes;i++) {
                if (ingredientes[i].colocado) continue;
                if (i==arrastando) continue;
                DrawRectangleRounded((Rectangle){ingredientes[i].x,ingredientes[i].y,ingredientes[i].w,ingredientes[i].h},0.3f,4,ingredientes[i].cor);
                DrawRectangleRoundedLines((Rectangle){ingredientes[i].x,ingredientes[i].y,ingredientes[i].w,ingredientes[i].h},0.3f,4,DARKGRAY);
                int larguraNome=MeasureText(ingredientes[i].nome,18);
                DrawText(ingredientes[i].nome,(int)(ingredientes[i].x+ingredientes[i].w/2.0f)-larguraNome/2,(int)(ingredientes[i].y+ingredientes[i].h/2.0f)-9,18,BLACK);
            }
            if (arrastando>=0) {
                DrawRectangleRounded((Rectangle){ingredientes[arrastando].x,ingredientes[arrastando].y,ingredientes[arrastando].w,ingredientes[arrastando].h},0.3f,4,ingredientes[arrastando].cor);
                DrawRectangleRoundedLines((Rectangle){ingredientes[arrastando].x,ingredientes[arrastando].y,ingredientes[arrastando].w,ingredientes[arrastando].h},0.3f,4,DARKGRAY);
                int larguraNome=MeasureText(ingredientes[arrastando].nome,18);
                DrawText(ingredientes[arrastando].nome,(int)(ingredientes[arrastando].x+ingredientes[arrastando].w/2.0f)-larguraNome/2,(int)(ingredientes[arrastando].y+ingredientes[arrastando].h/2.0f)-9,18,BLACK);
            }
            DrawTimerBomba(&timer,screenWidth,screenHeight);
        EndDrawing();
    }

    if (acertos>=2) {
        flag=1;
    }
    if (!errou && !flag) {
        while (!TimerTerminou(&timer) && !WindowShouldClose()) {
            UpdateTimerBomba(&timer);
            BeginDrawing();
                ClearBackground((Color){255,245,220,255});
                DrawTimerBomba(&timer,screenWidth,screenHeight);
            EndDrawing();
        }
    }

    if (flag) {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){255,245,220,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Vitoria",tamanhoFonte);
                DrawText("Vitoria",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,GREEN);
            EndDrawing();
        }
    } else {
        while (frames>0 && !WindowShouldClose()) {
            frames--;
            BeginDrawing();
                ClearBackground((Color){255,245,220,255});
                int tamanhoFonte=40;
                int larguraTexto=MeasureText("Derrota",tamanhoFonte);
                DrawText("Derrota",(screenWidth/2)-(larguraTexto/2),(screenHeight/2)-(tamanhoFonte/2),tamanhoFonte,RED);
            EndDrawing();
        }
    }
    UnloadTimerBomba(&timer);
}