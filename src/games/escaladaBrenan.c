#include "player.h"
#include "escaladaBrenan.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void escaladaBrenan(){
    int y=500;
    int altura=0,flag=0;
    
    int screenWidth = 1200;
    int screenHeight = 720;

    SetTargetFPS(60);
    int frames = 300;
    Texture2D background = LoadTexture("../assets/sprites/escaleBrenan/brenan.png");
    Texture2D player = LoadTexture("../assets/sprites/escaleBrenan/player.png");

    while(frames> 0 && !WindowShouldClose()){
        if(IsKeyPressed(KEY_SPACE)){
            y-=15;
            altura+=5;
        }
        
        if(altura>=150){
            flag=1;
            break;
        }
        frames--;

        
       BeginDrawing();
            ClearBackground(RAYWHITE);
            
            Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height };
            float scaledWidth = (float)background.width ;
            float scaledHeight = (float)background.height  ;
            Rectangle destRec = {
                (screenWidth - scaledWidth) / 2.0f,
                (screenHeight - scaledHeight) / 2.0f,
                scaledWidth,
                scaledHeight
            };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);

            float PLAYERscale = 0.2f;
    
            Rectangle sourceRecPlayer = { 0.0f, 0.0f, (float)player.width, (float)player.height };
            float scaledWidthPlayer = (float)player.width * PLAYERscale;
            float scaledHeightPlayer = (float)player.height * PLAYERscale;
            
            Rectangle destRecPlayer = {
                (screenWidth / 2.0f) - (scaledWidthPlayer / 2.0f), 
                (float)y,
                scaledWidthPlayer,
                scaledHeightPlayer
            };
            Vector2 originPlayer = { 0.0f, 0.0f };
            
            DrawTexturePro(player, sourceRecPlayer, destRecPlayer, originPlayer, 0.0f, WHITE);

            DrawText(TextFormat("Altura: %d/150", altura), 20, 20, 20, RED);
            DrawText(TextFormat("Tempo Restante: %d", frames / 60), 20, 50, 20, RED);
                
        EndDrawing();
    }
    frames = 120;

    if(flag){
        while(frames>0 && !WindowShouldClose()){
            frames--;
            BeginDrawing();
                ClearBackground(RAYWHITE);
                
                Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height };
                float scaledWidth = (float)background.width ;
                float scaledHeight = (float)background.height  ;
                Rectangle destRec = {
                    (screenWidth - scaledWidth) / 2.0f,
                    (screenHeight - scaledHeight) / 2.0f,
                    scaledWidth,
                    scaledHeight
                };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);

                float PLAYERscale = 0.2f;
        
                Rectangle sourceRecPlayer = { 0.0f, 0.0f, (float)player.width, (float)player.height };
                float scaledWidthPlayer = (float)player.width * PLAYERscale;
                float scaledHeightPlayer = (float)player.height * PLAYERscale;
                
                Rectangle destRecPlayer = {
                    (screenWidth / 2.0f) - (scaledWidthPlayer / 2.0f), 
                    (float)y,
                    scaledWidthPlayer,
                    scaledHeightPlayer
                };
                Vector2 originPlayer = { 0.0f, 0.0f };
                
                DrawTexturePro(player, sourceRecPlayer, destRecPlayer, originPlayer, 0.0f, WHITE);

                int tamanhoFonte = 40;
                int larguraTexto = MeasureText("Vitoria", tamanhoFonte);

                DrawText(
                    "Vitoria", 
                    (1200/2)-(larguraTexto / 2), 
                    (720/2)-(tamanhoFonte /2), 
                    tamanhoFonte, 
                    RED
                );
                    
            EndDrawing();
        }
    }else{
        while(frames>0 && !WindowShouldClose()){
            frames--;
            BeginDrawing();
                ClearBackground(RAYWHITE);
                
                Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height };
                float scaledWidth = (float)background.width ;
                float scaledHeight = (float)background.height  ;
                Rectangle destRec = {
                    (screenWidth - scaledWidth) / 2.0f,
                    (screenHeight - scaledHeight) / 2.0f,
                    scaledWidth,
                    scaledHeight
                };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);

                float PLAYERscale = 0.2f;
        
                Rectangle sourceRecPlayer = { 0.0f, 0.0f, (float)player.width, (float)player.height };
                float scaledWidthPlayer = (float)player.width * PLAYERscale;
                float scaledHeightPlayer = (float)player.height * PLAYERscale;
                
                Rectangle destRecPlayer = {
                    (screenWidth / 2.0f) - (scaledWidthPlayer / 2.0f), 
                    (float)y,
                    scaledWidthPlayer,
                    scaledHeightPlayer
                };
                Vector2 originPlayer = { 0.0f, 0.0f };
                
                DrawTexturePro(player, sourceRecPlayer, destRecPlayer, originPlayer, 0.0f, WHITE);

                int tamanhoFonte = 40;
                int larguraTexto = MeasureText("Derrota", tamanhoFonte);

                DrawText(
                    "Derrota", 
                    (1200/2)-(larguraTexto / 2), 
                    (720/2)-(tamanhoFonte /2), 
                    tamanhoFonte, 
                    RED
                );
                    
            EndDrawing();
        }
    }
    
    UnloadTexture(background);
    UnloadTexture(player);
}