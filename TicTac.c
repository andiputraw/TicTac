#include <stdio.h>
#include "raylib.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "menu.h"

int main(void)
{
    Screen s;
    Board b;
    MainMenu mainMenu;
    ModeSelectMenu modeSelectMenu;
    Color skyBlue = CLITERAL(Color){102, 191, 255, 255};
    Scene scene = MAIN_MENU;
    Font font;
    GameState gameState = (GameState){.gameStatus=ENDED, .p1={.name = {0}, .score = 0}, .p2={.name = {0}, .score = 0}, .scene = scene, .vsMode = VSPLAYER};

    char p1_container[255] = {0};
    char p2_container[255] = {0};

    UpdateScreen(&s, 800, 450);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
    InitWindow(s.width, s.height, "Tic Tac Toe - TCC");

    font = LoadFont("assets/fonts/Poppins/Poppins-Regular.ttf");
    SetWindowMinSize(800, 450); 

    CreateMainMenu(&mainMenu, &s, &scene, font);
    CreateModeSelectMenu(&modeSelectMenu, &gameState, &s, &scene, p1_container, p2_container, font);
    // printf("HELLO WORLD");

    SetTargetFPS(24);

    CreateBoard(&b,&gameState, BOARD_3_X_3, &s, font);
    b.turn = FIRST;
    scene = MAIN_MENU;
    while (!WindowShouldClose())
    {
        UpdateScreen(&s, GetScreenWidth(), GetScreenHeight());
        if (scene == GAMEPLAY)
        {

            UpdateBoard(&b);
        }
        else if(scene == MAIN_MENU)
        {
            UpdateMainMenu(&mainMenu);
        }else if(scene == SELECT_MODES_MENU){
            // printf("WE ARE ON A LOOP");
            UpdateModeSelectMenu(&modeSelectMenu);
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if (scene == GAMEPLAY)
        {
            DrawBoard(&b);
        }
        else if(scene == MAIN_MENU)
        {
            MainMenuDraw(mainMenu);
        }
        else if(scene == SELECT_MODES_MENU){
            ModeSelectMenuDraw(modeSelectMenu);
        }
        
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();

    return 0;
}
