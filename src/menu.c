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

void CreateMainMenu(MainMenu *mainMenu, Screen *screen, Scene *scene){
    int btn_width = screen->width*0.15;
    int btn_height = screen->height*0.1;
    int first_menu_upper_x = screen->width/2 - btn_width/2;
    int first_menu_upper_y = screen->height/2 - btn_height/2;
    int btn_margin_top = screen->width/16;
    int menu_font_size = screen->width*0.03;

    char *playText[3] = {"PLAY", "HISTORY", "EXIT"};

    Button *buttons[3] = { &mainMenu->playBtn, &mainMenu->historyBtn, &mainMenu->exitBtn };

    for(int i = 0; i < 3; i ++){
        CreateButton(buttons[i], __CalculateButtonRect(screen, i, btn_width, btn_height, btn_margin_top), playText[i], DARKGRAY, WHITE, menu_font_size);
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

    mainMenu->playBtn.fontSize = menu_font_size;
    mainMenu->historyBtn.fontSize = menu_font_size;
    mainMenu->exitBtn.fontSize = menu_font_size;
    
    
    UpdateButton(&mainMenu->historyBtn);
    UpdateButton(&mainMenu->exitBtn);
    UpdateButton(&mainMenu->playBtn);

    if(mainMenu->playBtn.isClicked){
        *mainMenu->scene = GAMEPLAY;
    }
}

void MainMenuDraw(MainMenu mainMenu){
    int title_font_size = mainMenu.screen->width*0.04;
    DrawText("Main Menu",mainMenu.screen->width/2  - MeasureText("Main Menu",title_font_size)/2,mainMenu.screen->height/4, title_font_size, BLACK);
    DrawButton(&mainMenu.playBtn);
    DrawButton(&mainMenu.historyBtn);
    DrawButton(&mainMenu.exitBtn);
}

void ModeSelectMenuDraw(ModeSelectMenu selectMenu){
    
}