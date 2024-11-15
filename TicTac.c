#include <stdio.h>
#include "raylib.h"
#include "screen.h"
#include "board.h"
int main(void)
{
    Screen s;
    Board b;

    Color skyBlue = CLITERAL(Color){ 102, 191, 255, 255 };
    UpdateScreen(&s, 800, 450);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(s.width, s.height, "Tic Tac Toe - TCC");

    SetTargetFPS(24);

    CreateBoard(&b, BOARD_3_X_3, &s);
 
    while (!WindowShouldClose())    
    {
        UpdateScreen(&s, GetScreenWidth(),GetScreenHeight());
    
        BeginDrawing();
            ClearBackground(WHITE);
            DrawBoard(&b);
        EndDrawing();
       
    }

   
    CloseWindow();

    return 0;
}

