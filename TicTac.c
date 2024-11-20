#include <stdio.h>
#include "raylib.h"
#include "screen.h"
#include "board.h"
#include "player.h"
int main(void)
{
    Screen s;
    Board b;
    Player player;
  

    Color skyBlue = CLITERAL(Color){ 102, 191, 255, 255 };
    UpdateScreen(&s, 800, 450);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(s.width, s.height, "Tic Tac Toe - TCC");

    player.turn = 0;

    SetTargetFPS(24);

    CreateBoard(&b, BOARD_3_X_3, &s);
 
    while (!WindowShouldClose())    
    {
        UpdateScreen(&s, GetScreenWidth(),GetScreenHeight());
        UpdateBoard(&b,player);
        
        BeginDrawing();
            ClearBackground(WHITE);
            DrawBoard(&b);
        EndDrawing();
       
    }

   
    CloseWindow();

    return 0;
}

