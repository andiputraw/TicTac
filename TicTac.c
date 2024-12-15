#include <stdio.h>
#include "raylib.h"
#include "game.h"
#include "board.h"
#include "game.h"
#include "menu.h"
#include "leaderboard.h"
#include <string.h>

// #define TEST

#ifdef TEST

void test_case()
{
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
    Scene scene;
    Font font;
    Leaderboard leaderboard;
    GameState gameState;
    Timer timer;
    History history;

    char p1_container[255] = {0};
    char p2_container[255] = {0};

#ifdef TEST
    printf("EXECUTING TEST CASE...\n");
    test_case();
    return 0;
#endif // TEST

    gameState = (GameState){.gameStatus = ENDED, .p1 = {.name = {0}, .score = 0}, .p2 = {.name = {0}, .score = 0}, .scene = scene, .vsMode = VSBOT, .botMode = EASY};
    b.turn = FIRST;
    gameState.scene = MAIN_MENU;
    // pre window configuration
    UpdateScreen(&s, 800, 450);
    OpenLeaderboard(&leaderboard);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(s.width, s.height, "Tic Tac Toe - TCC");

    // post window configuration
    font = LoadFontEx("assets/fonts/Poppins/Poppins-Regular.ttf", 96, NULL, 0);
    SetWindowMinSize(800, 450);

    // menu initializing
    CreateMainMenu(&mainMenu, &s, &gameState.scene, font);
    CreateModeSelectMenu(&modeSelectMenu, &gameState, &s, &scene, &b, p1_container, p2_container, font);
    CreateLeaderboardMenu(&leaderBoardMenu, &gameState, &s, &scene, &leaderboard, font);
    CreateTimer(&timer, &gameState, font);
    // printf("HELLO WORLD");

    SetTargetFPS(24);

    CreateBoard(&b, &gameState, BOARD_3_X_3, &s, &timer, font, &leaderboard);

    
    while (!WindowShouldClose())
    {

        UpdateScreen(&s, GetScreenWidth(), GetScreenHeight());

        BeginDrawing();
        ClearBackground(WHITE);
        if (gameState.scene == GAMEPLAY)
        {
            UpdateBoard(&b);
            UpdateTimer(&timer);
            DrawTimer(&s, &timer);
            DrawBoard(&b);
            // if(gameState.gameStatus == ENDED){
            //     DrawGameOverScene(&b);
            //     if(gameState.vsMode == VSPLAYER) {
            //         memcpy(&history.p1, &gameState.p1, sizeof(Player));
            //         memcpy(&history.p2, &gameState.p2, sizeof(Player));
            //         history.game_mode = b.mode;
            //         for(int i = 0; i < 25; i++){
            //             history.BoardState[i] = b.boxes[i].value;
            //         }
            //         WriteHistory(&leaderboard, &history);
            //     }
            // }
        }
        else if (gameState.scene == MAIN_MENU)
        {
            UpdateMainMenu(&mainMenu);
            MainMenuDraw(mainMenu);
        }
        else if (gameState.scene == SELECT_MODES_MENU)
        {
            UpdateModeSelectMenu(&modeSelectMenu);
            ModeSelectMenuDraw(&modeSelectMenu);
        }
        else if (gameState.scene == LEADERBORAD_MENU)
        {
            UpdateLeaderboardMenu(&leaderBoardMenu);
            DrawLeaderboardMenu(&leaderBoardMenu);
        }

        EndDrawing();

        if (gameState.scene == EXIT_GAME)
        {
            break;
        }
    }
    UnloadFont(font);
    CloseWindow();
    CloseLeaderboard(&leaderboard);
    return 0;
}
