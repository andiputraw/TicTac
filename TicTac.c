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
    Color skyBlue = CLITERAL(Color){102, 191, 255, 255};
    Scene scene = MAIN_MENU;

    UpdateScreen(&s, 800, 450);
    CreateMainMenu(&mainMenu, &s, &scene);
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
        
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
