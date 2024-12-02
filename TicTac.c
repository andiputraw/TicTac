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
    
    Scene scene = SELECT_MODES_MENU;
    char p1_container[255] = {0};
    char p2_container[255] = {0};

    UpdateScreen(&s, 800, 450);
    CreateMainMenu(&mainMenu, &s, &scene);
    CreateModeSelectMenu(&modeSelectMenu, &s, &scene, p1_container, p2_container);
    // printf("HELLO WORLD");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
    InitWindow(s.width, s.height, "Tic Tac Toe - TCC");
    SetWindowMinSize(800, 450); 
    SetTargetFPS(24);

    CreateBoard(&b, BOARD_3_X_3, &s);
    b.turn = FIRST;
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
    CloseWindow();

    return 0;
}
