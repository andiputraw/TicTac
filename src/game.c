/**
 * game.c
 * Author : Andi Putra Wijaya & Maulana Ishak
 * Created: 15 November 2024
 * 
 * Source code implementasi game.h. Yang memiliki definisi struct dan enum yang digunakan di modul lain dan utilitas timer
 */

#include "game.h"
#include "raylib.h"

void UpdateScreen(Screen *s, int w, int h ) {
    s->height = h;
    s->width = w;
}

// SEMENTARA
void CreateTimer(Timer *timer, GameState *gameState, Font font){
        timer->seconds = 0;
        timer->minutes = 0;
        timer->timeAmount = 0;
        timer->isRunning = false;
        timer->elapsedTime = 0; 
        timer->font = font;
        timer->gameState = gameState;

}

void UpdateTimer(Timer *timer){
    if(timer->gameState->gameStatus != PLAYING){
        timer->timeAmount = 0;
        timer->elapsedTime = 0;
        timer->isRunning = false;

    }else{
        timer->isRunning = true;
    }

    if(timer->isRunning){
        timer->elapsedTime += GetFrameTime();
        if (timer->elapsedTime >= 1.0f) {
                timer->timeAmount++;
                timer->elapsedTime -= 1.0f;
        }
    }

    timer->minutes = timer->timeAmount / 60;
    timer->seconds = timer->timeAmount % 60;
}

void DrawTimer(Screen *s, Timer *timer){
    DrawTextEx(timer->font, TextFormat("%02d:%02d",timer->minutes, timer->seconds), (Vector2){s->width/2.85,s->height/5.7},30,1,DARKGRAY);
}