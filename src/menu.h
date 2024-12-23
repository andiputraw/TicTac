/**
 * menu.h
 * Author : Andi Putra Wijaya & Maulana Ishak
 * Created: 30 November 2024
 * 
 * Header file untuk menu.c. Yang digunakan untuk mengupdate tampilan main menu, mode selection menu dan leaderboard menu.
 */
#ifndef MENU_H
#define MENU_H

#include "control.h"
#include "game.h"
#include "leaderboard.h"
#include "board.h"
typedef struct
{
    Button historyBtn;
    Button playBtn;
    Button exitBtn;
    Screen *screen;
    Scene *scene;
} MainMenu;

typedef struct
{
    Button vsBotBtn;
    Button vsPlayerBtn;
    InputText InputP1Name;
    InputText InputP2Name;
    Button classicModeBtn;
    Button extendedModeBtn;
    Button easyBotBtn;
    Button mediumBotBtn;
    Button hardBotBtn;
    Screen *screen;
    Scene *scene;
    GameState *gameState;
    Board *b;
} ModeSelectMenu;

typedef enum {
    LeaderboardMenuLeaderboard = 0,
    LeaderboardMenuHistory,
} LeaderboardMenuScene;

typedef struct{
    Button nextBtn;
    Button backBtn;
    Button nextHistory;
    Button prevHistory;

    LeaderboardMenuScene menuScene;
    Screen *screen;
    Scene *scene;
    GameState *gameState;
    Leaderboard *leaderboard;
    Font font;
    bool isLeaderboardUpdated ;
    PlayerElo Top5Leaderboard[5];
    int page;
    int len;
    int max_page;
    History CurrentlyShowedHistory[5];
} LeaderboardMenu;

void CreateMainMenu(MainMenu *mainmenu, Screen *screen, Scene *scene, Font font);
void UpdateMainMenu(MainMenu *mainMenu);
void MainMenuDraw(MainMenu mainMenu);

void CreateModeSelectMenu (ModeSelectMenu *menu, GameState *gameState, Screen *screen, Scene *scene, Board *b, char input_p1[255], char input_p2[255], Font font);
void UpdateModeSelectMenu(ModeSelectMenu *selectMenu);
void ModeSelectMenuDraw(ModeSelectMenu *selectMenu);

void CreateLeaderboardMenu(LeaderboardMenu *menu, GameState *gameState, Screen *screen, Scene *scene, Leaderboard *l, Font font);
void UpdateLeaderboardMenu(LeaderboardMenu *menu);
void DrawLeaderboardMenu(LeaderboardMenu *menu);

ButtonStyle __BetterMenuButtonStyle(Font font, Screen screen);
int __ResponsiveFontSize(Screen screen, int min, int mid, int max);
int __CalculateMenuButtonFontSize(Screen screen);
Rectangle __MeasureRectangleModeSelectMenu(Screen *screen, int index, int btn_height, int btn_width, int margin_r, int len, float h_loc);
InputTextStyle __MainMenuInputTextStyle(Font font, int fontSize);
ButtonStyle __MainMenuButtonStyle(Font font, int fontSize);

#endif //! MENU_H
