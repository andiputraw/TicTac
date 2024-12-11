#include "menu.h"
#include "control.h"
#include "game.h"
#include "raylib.h"
#include <stdio.h>

Rectangle __CalculateButtonRect(Screen *screen, int index, int btnWidth, int btnHeight, int btnMarginTop){
    int firstMenuUpperX = screen->width / 2  - btnWidth / 2;
    int firstMenuUpperY = screen->height / 2 - btnHeight / 2;

    return (Rectangle) {
        .x = firstMenuUpperX,
        .y = firstMenuUpperY,
        .height = btnHeight + (btnMarginTop * index),
        .width = btnWidth,
    };
}

ButtonStyle MainMenuButtonStyle(Font font, int fontSize){
    return CreateButtonStyle(GRAY, WHITE, font, fontSize);
}

InputTextStyle MainMenuInputTextStyle(Font font, int fontSize) {
    return CreateInputTextStyle(GRAY, WHITE, font, fontSize);
}

void CreateMainMenu(MainMenu *mainMenu, Screen *screen, Scene *scene, Font font){
    int btn_width = screen->width*0.15;
    int btn_height = screen->height*0.1;
    int first_menu_upper_x = screen->width/2 - btn_width/2;
    int first_menu_upper_y = screen->height/2 - btn_height/2;
    int btn_margin_top = screen->width/16;
    int menu_font_size = screen->width*0.03;

    char *playText[3] = {"PLAY", "HISTORY", "EXIT"};

    Button *buttons[3] = { &mainMenu->playBtn, &mainMenu->historyBtn, &mainMenu->exitBtn };

    for(int i = 0; i < 3; i ++){
        CreateButton(buttons[i], __CalculateButtonRect(screen, i, btn_width, btn_height, btn_margin_top), playText[i], MainMenuButtonStyle( font, menu_font_size));
    }
    *mainMenu = (MainMenu) {.playBtn = *buttons[0], .historyBtn = *buttons[1], .exitBtn = *buttons[2], .screen = screen, .scene = scene};
}

void UpdateMainMenu(MainMenu *mainMenu){
    int btn_width = mainMenu->screen->width*0.15;
    int btn_height = mainMenu->screen->height*0.1;
    int first_menu_upper_x = mainMenu->screen->width/2 - btn_width/2;
    int first_menu_upper_y = mainMenu->screen->height/2 - btn_height/2;
    int btn_margin_top = mainMenu->screen->width/16;
    int menu_font_size = mainMenu->screen->width*0.03;


    Rectangle playRect = (Rectangle) {.x = first_menu_upper_x, .y = first_menu_upper_y, .width=btn_width, .height=btn_height};
    Rectangle historyRect = (Rectangle) {.x = first_menu_upper_x, .y = first_menu_upper_y+btn_margin_top, .width=btn_width, .height=btn_height};
    Rectangle exitRect = (Rectangle) {.x = first_menu_upper_x, .y = first_menu_upper_y+(btn_margin_top*2), .width=btn_width, .height=btn_height};

    // char *playText[3] = {"PLAY", "HISTORY", "EXIT"};

    // Button *buttons[3] = { &mainMenu->playBtn, &mainMenu->historyBtn, &mainMenu->exitBtn };

    // for(int i = 0; i < 3; i ++){
    //     buttons
    // }

    mainMenu->playBtn.rect = playRect;
    mainMenu->historyBtn.rect = historyRect;
    mainMenu->exitBtn.rect = exitRect;

    mainMenu->playBtn.style.fontSize = menu_font_size;
    mainMenu->historyBtn.style.fontSize = menu_font_size;
    mainMenu->exitBtn.style.fontSize = menu_font_size;


    UpdateButton(&mainMenu->historyBtn);
    UpdateButton(&mainMenu->exitBtn);
    UpdateButton(&mainMenu->playBtn);

    if(mainMenu->playBtn.isClicked){
        *mainMenu->scene = SELECT_MODES_MENU;
    }
    if(mainMenu->historyBtn.isClicked) {
        *mainMenu->scene = LEADERBORAD_MENU;
    }
}

void MainMenuDraw(MainMenu mainMenu){
    int title_font_size = mainMenu.screen->width*0.04;
    DrawText("Main Menu",mainMenu.screen->width/2  - MeasureText("Main Menu",title_font_size)/2,mainMenu.screen->height/4, title_font_size, BLACK);
    DrawButton(&mainMenu.playBtn);
    DrawButton(&mainMenu.historyBtn);
    DrawButton(&mainMenu.exitBtn);
}

Rectangle __MeasureRectangleModeSelectMenu(Screen *screen, int index, int btn_height, int btn_width, int margin_r, int len, float h_loc){
    int firstMenuUpperX;
    int firstMenuUpperY = (screen->height * h_loc);

    if(len % 2 == 0){
        firstMenuUpperX = (screen->width / len) - (btn_width);
    }else {
        firstMenuUpperX = (screen->width / len) - (btn_width / 2);
    }

    return (Rectangle) {
        .x = firstMenuUpperX + (index * (btn_width) + index * (margin_r)),
        .y = firstMenuUpperY,
        .height = btn_height,
        .width = btn_width,
    };
}

void CreateModeSelectMenu(ModeSelectMenu *menu,GameState *gameState, Screen *screen, Scene *scene, char input_p1[255], char input_p2[255], Font font){
    int btn_width = screen->width*0.15;
    int btn_height = screen->height*0.1;
    int btn_margin_right = btn_width * 0.1;
    int menu_font_size = screen->width*0.03;
    Rectangle rec;
    Button *buttons[3] = {&menu->classicModeBtn, &menu->extendedModeBtn, &menu->pyramidModeBtn};
    char *btn_text[3] = {"Classic", "Extended", "Pyramid"};
    for(int i = 0; i < 3; i++) {
        rec = __MeasureRectangleModeSelectMenu(screen, i, btn_height, btn_width, btn_margin_right, 3 , 0.8);
        CreateButton(buttons[i], rec, btn_text[i], MainMenuButtonStyle(font, menu_font_size));
    }

    int input_width = screen->width * 0.4;
    int input_height = screen->height * 0.1;
    int input_margin_right = input_width * 0.1;
    InputTextStyle style = MainMenuInputTextStyle(font, menu_font_size) ;
    SetInputTextStylePlaceholder(&style, "Enter your name..");
    CreateInputText(&menu->InputP1Name, __MeasureRectangleModeSelectMenu(screen, 0, input_height, input_width, input_margin_right, 2, 0.2), input_p1,  style);
    CreateInputText(&menu->InputP2Name, __MeasureRectangleModeSelectMenu(screen, 1, input_height, input_width, input_margin_right, 2, 0.2), input_p2, MainMenuInputTextStyle(font, menu_font_size) );


    Button *vs_btn[2] = {&menu->vsPlayerBtn, &menu->vsBotBtn};
    char *vs_btn_text[2] = {"VS PLAYER", "VS BOT"};

    for( int i = 0; i < 2; i++){
        rec = __MeasureRectangleModeSelectMenu(screen, i, btn_height, btn_width, btn_margin_right, 2, 0.4);
        CreateButton(vs_btn[i], rec, vs_btn_text[i], MainMenuButtonStyle(font, menu_font_size));
    }

    Button *diff_btn[3] = {&menu->easyBotBtn, &menu->mediumBotBtn, &menu->hardBotBtn};
    char *diff_btn_text[3] = {"EASY", "MEDIUM", "HARD"};

    for(int i = 0; i < 3; i++){
        rec = __MeasureRectangleModeSelectMenu(screen, i, btn_height, btn_width, btn_margin_right, 3, 0.6);
        CreateButton(diff_btn[i], rec, diff_btn_text[i], MainMenuButtonStyle(font, menu_font_size));
    }

    *menu = (ModeSelectMenu){
        .classicModeBtn = menu->classicModeBtn,
        .extendedModeBtn = menu->extendedModeBtn,
        .pyramidModeBtn = menu->pyramidModeBtn,
        .screen = screen,
        .InputP1Name = menu->InputP1Name,
        .InputP2Name = menu->InputP2Name,
        .vsBotBtn = menu->vsBotBtn,
        .vsPlayerBtn = menu->vsPlayerBtn,
        .easyBotBtn = menu->easyBotBtn,
        .mediumBotBtn = menu->mediumBotBtn,
        .hardBotBtn = menu->hardBotBtn,
        .scene = scene,
        .gameState= gameState
    };

}

void UpdateModeSelectMenu(ModeSelectMenu *menu) {
    int btn_width = menu->screen->width*0.15;
    int btn_height = menu->screen->height*0.1;
    int btn_margin_right = btn_width * 0.2;
    int menu_font_size = menu->screen->width*0.03;
    Rectangle rec;
    Button *buttons[3] = {&menu->classicModeBtn, &menu->extendedModeBtn, &menu->pyramidModeBtn};
    char *btn_text[3] = {"Classic", "Extended", "Pyramid"};
    for(int i = 0; i < 3; i++) {
        rec = __MeasureRectangleModeSelectMenu(menu->screen, i, btn_height, btn_width, btn_margin_right, 3 , 0.8);
        buttons[i]->rect = rec;
        buttons[i]->style.fontSize = menu_font_size;
        UpdateButton(buttons[i]);
        if(buttons[i]->isClicked){
            *menu->scene = GAMEPLAY;
            menu->gameState->gameStatus = PLAYING;
            printf("%d",menu->gameState->gameStatus);
        }
    }


    int input_width = menu->screen->width * 0.3;
    int input_height = menu->screen->height * 0.1;
    int input_margin_right = input_width * 0.1;

    menu->InputP1Name.rect = __MeasureRectangleModeSelectMenu(menu->screen, 0, input_height, input_width, input_margin_right, 2, 0.2);
    menu->InputP2Name.rect = __MeasureRectangleModeSelectMenu(menu->screen, 1, input_height, input_width, input_margin_right, 2, 0.2);
    menu->InputP1Name.style.fontSize = menu_font_size;
    menu->InputP2Name.style.fontSize = menu_font_size;

    UpdateInputText(&menu->InputP1Name);
    UpdateInputText(&menu->InputP2Name);

    for(int i = 0; i<20; i++){
        menu->gameState->p1.name[i] = menu->InputP1Name.value[i];
        menu->gameState->p2.name[i] = menu->InputP2Name.value[i];
    }


    Button *vs_btn[2] = {&menu->vsPlayerBtn, &menu->vsBotBtn};
    char *vs_btn_text[2] = {"VS PLAYER", "VS BOT"};

    for( int i = 0; i < 2; i++){
        rec = __MeasureRectangleModeSelectMenu(menu->screen, i, btn_height, btn_width + 30, btn_margin_right, 2, 0.4);
        vs_btn[i]->rect = rec;
        vs_btn[i]->style.fontSize = menu_font_size;
        UpdateButton(vs_btn[i]);
    }

    Button *diff_btn[3] = {&menu->easyBotBtn, &menu->mediumBotBtn, &menu->hardBotBtn};

    for(int i = 0; i < 3; i++){
        rec = __MeasureRectangleModeSelectMenu(menu->screen, i, btn_height, btn_width, btn_margin_right, 3, 0.6);
        diff_btn[i]->rect = rec;
        diff_btn[i]->style.fontSize = menu_font_size;
        UpdateButton(diff_btn[i]);
    }
}

void ModeSelectMenuDraw(ModeSelectMenu selectMenu){
    DrawButton(&selectMenu.classicModeBtn);
    DrawButton(&selectMenu.extendedModeBtn);
    DrawButton(&selectMenu.pyramidModeBtn);

    DrawInputText(&selectMenu.InputP1Name);
    DrawInputText(&selectMenu.InputP2Name);

    DrawButton(&selectMenu.vsPlayerBtn);
    DrawButton(&selectMenu.vsBotBtn);

    DrawButton(&selectMenu.easyBotBtn);
    DrawButton(&selectMenu.mediumBotBtn);
    DrawButton(&selectMenu.hardBotBtn);
}

int __CalculateMenuButtonFontSize(Screen screen){
    int fontSize = 48;
    if(screen.width <= 1024){
        fontSize = 24;
    }else if(screen.width <= 1360){
        fontSize = 32;
    }
    return fontSize;
}

int __ResponsiveFontSize(Screen screen, int min, int mid, int max){
    int fontSize = max;
    if(screen.width <= 1024){
        fontSize = min;
    }else if(screen.width <= 1360){
        fontSize = mid;
    }
    return fontSize;
}

ButtonStyle BetterMenuButtonStyle(Font font, Screen screen){
    int fontSize = __CalculateMenuButtonFontSize(screen);
    return CreateButtonStyle(WHITE, BLACK, font, fontSize);
}


void CreateLeaderboardMenu(LeaderboardMenu *menu, GameState *gameState, Screen *screen, Scene *scene, Leaderboard *l, Font font){
    menu->gameState = gameState;
    menu->leaderboard = l;
    menu->scene = scene;
    menu->screen = screen;
    menu->font = font;
    menu->menuScene = LeaderboardMenuLeaderboard;
    CreateButton(&menu->nextBtn, NO_RECT, "History", BetterMenuButtonStyle(font, *screen));
    CreateButton(&menu->backBtn, NO_RECT, "Back", BetterMenuButtonStyle(font, *screen));
}

void UpdateLeaderboardMenu(LeaderboardMenu *menu){

    int width = menu->screen->width * 0.2 > 240 ? 240 : menu->screen->width * 0.2;
    int height = menu->screen->height * 0.1 > 60 ? 60 : menu->screen->height * 0.1;

    menu->nextBtn.style.fontSize = __CalculateMenuButtonFontSize(*menu->screen);
    menu->backBtn.style.fontSize = __CalculateMenuButtonFontSize(*menu->screen);
    menu->backBtn.rect = (Rectangle){
        .x = menu->screen->width * 0.05,
        .y = menu->screen->height * 0.1,
        .width = width,
        .height = height,
    };
    menu->nextBtn.rect = (Rectangle){
        .x = menu->screen->width * 0.8,
        .y = menu->screen->height * 0.8,
        .width = width,
        .height = height,
    };
    UpdateButton(&menu->nextBtn);
    UpdateButton(&menu->backBtn);

    if(menu->nextBtn.isClicked){
        if(menu->menuScene == LeaderboardMenuLeaderboard){
            menu->menuScene = LeaderboardMenuHistory;
        }else {
            menu->menuScene = LeaderboardMenuLeaderboard;
        }
    }

    if(menu->backBtn.isClicked) {
        *menu->scene = MAIN_MENU;
    }
}

void DrawLeaderboardMenu(LeaderboardMenu *menu){
    int titleFont = __ResponsiveFontSize(*menu->screen, 48,64,80);
    int contentFontSize = __ResponsiveFontSize(*menu->screen, 32, 48, 64);
    Vector2 textLoc = (Vector2){
        .x = (menu->screen->width / 2),
        .y = (menu->screen->height * 0.1)
    };
    Vector2 StartMidVerticalTable = (Vector2){
        .x = (menu->screen->width / 2),
        .y = (menu->screen->height * 0.3),
    };
    Vector2 EndMidVerticalTable = (Vector2){
        .x = (menu->screen->width / 2),
        .y = (menu->screen->height * 0.9),
    };
    Vector2 StartHorizontal, EndHorizontal;
    float element_height = menu->screen->height * 0.1;
    Vector2 contentPos;

    DrawButton(&menu->nextBtn);
    DrawButton(&menu->backBtn);
    if(menu->menuScene == LeaderboardMenuLeaderboard){
        textLoc.x -= MeasureTextEx(menu->font, "Leaderboard", titleFont, 1).x / 2;
        DrawTextEx(menu->font, "Leaderboard", textLoc, titleFont, 1, BLACK);
    }else {
        textLoc.x -= MeasureTextEx(menu->font, "History", titleFont, 1).x / 2;
        DrawTextEx(menu->font, "History", textLoc, titleFont, 1, BLACK);
    }

    // Table
    DrawLineEx(StartMidVerticalTable, EndMidVerticalTable, 4.0, BLACK);
    for(int i = 0; i < 5; i++){
        StartHorizontal.x = menu->screen->width * 0.3;
        StartHorizontal.y = (menu->screen->height * 0.4) + ( element_height * i);

        EndHorizontal.x = menu->screen->width * 0.7;
        EndHorizontal.y = (menu->screen->height * 0.4) + ( element_height * i);

        DrawLineEx(StartHorizontal, EndHorizontal, 4.0, BLACK);
    }
    for(int i = 0; i < 6; i++){
        if(i == 0 ){
            // Draw Header Table
            contentPos.x = StartHorizontal.x + (EndHorizontal.x - StartHorizontal.x) / 4 - MeasureTextEx(menu->font, "Name", contentFontSize, 1).x / 2;
            contentPos.y = (menu->screen->height * 0.4) + ( element_height * (i - 1) + element_height / 4) ;
            DrawTextEx(menu->font, "Name", contentPos, contentFontSize, 1, BLACK);

            contentPos.x = StartHorizontal.x + (EndHorizontal.x - StartHorizontal.x) * 3 / 4 - MeasureTextEx(menu->font, "Elo", contentFontSize, 1).x / 2;
            DrawTextEx(menu->font, "Elo", contentPos, contentFontSize, 1, BLACK);
        }else {
            // Draw Header Table
            contentPos.x = StartHorizontal.x + (EndHorizontal.x - StartHorizontal.x) / 4 - MeasureTextEx(menu->font, "Nama 1", contentFontSize, 1).x / 2;
            contentPos.y = (menu->screen->height * 0.4) + ( element_height * (i - 1) + element_height / 4) ;
            DrawTextEx(menu->font, "Nama 1", contentPos, contentFontSize, 1, GRAY);

            contentPos.x = StartHorizontal.x + (EndHorizontal.x - StartHorizontal.x) * 3 / 4 - MeasureTextEx(menu->font, "999", contentFontSize, 1).x / 2;
            DrawTextEx(menu->font, "999", contentPos, contentFontSize, 1, GRAY);
        }
    }
}