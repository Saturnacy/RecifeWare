#include "raylib.h"
#include "player.h"

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
    
    Gamestate currentState = STATE_TITLE; 
    int ranking[11][5]={0};
    int cont_caracter=0;
    Jogador status={ 0, "" };

    while (!WindowShouldClose()) {
        if(currentState==STATE_TITLE){
            int caractere=GetCharPressed();
            while (caractere>0)
            {
                if(caractere>=97 && caractere<=122){
                    caractere-=32;
                }
                if(caractere>=65 && caractere<=90 && cont_caracter<3){
                    status.nome_usuario[cont_caracter]=(char)caractere;
                    cont_caracter++;
                    status.nome_usuario[cont_caracter]='\0';
                }
                caractere = GetCharPressed();
            }
            
            if(IsKeyPressed(KEY_BACKSPACE) && cont_caracter >0){
                    cont_caracter--;
                    status.nome_usuario[cont_caracter]='\0';
            }

            if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && validate_name(status.nome_usuario,ranking,cont_caracter)==1) {
                
                currentState = STATE_SPLASH_CESAR;

            } else if(validate_name(status.nome_usuario,ranking,cont_caracter)==0){
                DrawText("Nome Invalido", 210, 160, 20, MAROON);
            }

        

        }

       BeginDrawing();
            ClearBackground(RAYWHITE);

            switch (currentState) {
                case STATE_TITLE: {
                    Color corStatus = (validate_name(status.nome_usuario,ranking,cont_caracter) == 0) ? RED : (validate_name(status.nome_usuario,ranking,cont_caracter) == 1 ? GREEN : DARKGRAY);

                    DrawRectangleLines((1200/2)-80, (720/2) - 50, 80, 40, corStatus);
                    DrawText(status.nome_usuario, (1200/2)-75, (720/2) - 45, 30, corStatus);
                    DrawText("RECIFEWARE", 1200/2 - 100, (720/2)-200, 20, BLUE);

                    DrawText("Pressione ENTER ou SPACE para iniciar", 1200/2 - 180, 720/2 + 30, 20, DARKGRAY);
                    DrawText("Use WASD para mover, E para atirar, ESC para voltar", 1200/2 - 220, 720/2 + 60, 18, DARKGRAY);
                    
                    if (validate_name(status.nome_usuario,ranking,cont_caracter) == 0) {
                        DrawText("NOME INVÁLIDO OU JÁ EXISTENTE", 1200/2 - 130, 720/2 - 80, 15, RED);
                    }
                } break;

                case STATE_SPLASH_CESAR:
                    DrawText("LOGOMARCA CESAR", 1200/2 - 100, 720/2, 20, BLUE);
                    DrawText("Pressione ESC para voltar", 10, 10, 20, LIGHTGRAY);
                    break;

                case STATE_INTRO:
                    DrawText("HISTORIA DO JOGO...", 1200/2 - 100, 720/2, 20, BLACK);
                    break;
                
                default: break;
            }
        EndDrawing();
    
    }

    CloseWindow();

    return 0;
}