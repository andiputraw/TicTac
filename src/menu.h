#ifndef MENU_H
#define MENU_H

#include "control.h"
#include "game.h"

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
    Button pyramidModeBtn;
    Button extendedModeBtn;
    Button easyBotBtn;
    Button mediumBotBtn;
    Button hardBotBtn;
    Screen *screen;
    Scene *scene;
    GameState *gameState;
    VsMode *vsMode;
} ModeSelectMenu;

void CreateMainMenu(MainMenu *mainmenu, Screen *screen, Scene *scene, Font font);
void UpdateMainMenu(MainMenu *mainMenu);
void MainMenuDraw(MainMenu mainMenu);

void CreateModeSelectMenu (ModeSelectMenu *menu, Screen *screen, Scene *scene, char input_p1[255], char input_p2[255], Font font);
void UpdateModeSelectMenu(ModeSelectMenu *selectMenu);
void ModeSelectMenuDraw(ModeSelectMenu selectMenu);

#endif //! MENU_H
