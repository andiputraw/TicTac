#include <stdio.h>
#include "raylib.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "menu.h"
#include "leaderboard.h"

// #define TEST

#ifdef TEST

void test_case(){
    printf("EXECUTING HISTORY TEST CASE\n");
    printf("for whatever reason, test history is not executed\n");
    TestHistory();
    printf("HISTORY TEST CASE COMPLETED\n");
}

#endif // TEST

int main(void)
{
    Screen s;
    Board b;
    MainMenu mainMenu;
    ModeSelectMenu modeSelectMenu;
    LeaderboardMenu leaderBoardMenu;
    Color skyBlue = CLITERAL(Color){102, 191, 255, 255};
    Scene scene;
    Font font;
    Leaderboard leaderboard;
    GameState gameState = (GameState){.gameStatus=ENDED, .p1={.name = {0}, .score = 0}, .p2={.name = {0}, .score = 0}, .scene = scene, .vsMode = VSPLAYER};

    char p1_container[255] = {0};
    char p2_container[255] = {0};

    #ifdef TEST
    printf("EXECUTING TEST CASE...\n");
    test_case();
    return 0;
    #endif // TEST

    // pre window configuration
    UpdateScreen(&s, 800, 450);
    OpenLeaderboard(&leaderboard);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);

    InitWindow(s.width, s.height, "Tic Tac Toe - TCC");

    // post window configuration
    font = LoadFontEx("assets/fonts/Poppins/Poppins-Regular.ttf", 96, NULL, 0);
    SetWindowMinSize(800, 450);

    // menu initializing
    CreateMainMenu(&mainMenu, &s, &scene, font);
    CreateModeSelectMenu(&modeSelectMenu, &gameState, &s, &scene, p1_container, p2_container, font);
    CreateLeaderboardMenu(&leaderBoardMenu, &gameState, &s, &scene, &leaderboard, font);
    // printf("HELLO WORLD");

    SetTargetFPS(24);

    CreateBoard(&b,&gameState, BOARD_3_X_3, &s, font);
    b.turn = FIRST;
    scene = LEADERBORAD_MENU;
    while (!WindowShouldClose())
    {
        UpdateScreen(&s, GetScreenWidth(), GetScreenHeight());

        BeginDrawing();
        ClearBackground(WHITE);
        if (scene == GAMEPLAY)
        {

            UpdateBoard(&b);
            DrawBoard(&b);
        }
        else if(scene == MAIN_MENU)
        {
            UpdateMainMenu(&mainMenu);
            MainMenuDraw(mainMenu);
        }else if(scene == SELECT_MODES_MENU){
            UpdateModeSelectMenu(&modeSelectMenu);
            ModeSelectMenuDraw(modeSelectMenu);
        }else if(scene == LEADERBORAD_MENU){
            UpdateLeaderboardMenu(&leaderBoardMenu);
            DrawLeaderboardMenu(&leaderBoardMenu);
        }

        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    CloseLeaderboard(&leaderboard);
    return 0;
}
