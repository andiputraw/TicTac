#include "game.h"
#include "raylib.h"

void UpdateScreen(Screen *s, int w, int h ) {
    s->height = h;
    s->width = w;
}

// SEMENTARA
void CreateTimer(Timer *timer){
        timer->seconds = 0;
        timer->isRunning = false;
        timer->startTime = 0;
        timer->pauseTime = 0;    

}

//TIMER BELUM BENARRRR!!!!!!!!!!!!!!!!!!!!!
void UpdateTimer(GameState *gameState, Timer *timer){
    if(gameState->gameStatus == PLAYING){

        if(timer->isRunning){
            timer->isRunning = false;
            timer->pauseTime = GetTime() - timer->startTime;
        }else{
            timer->isRunning = true;
            timer->startTime = GetTime() - timer->pauseTime;
        }
    }
    if(gameState->gameStatus != PLAYING){
        timer->seconds = 0;
        timer->isRunning = false;
        timer->startTime = 0;
        timer->pauseTime = 0;
    }
    if(timer->isRunning){
        timer->seconds = ((int)(GetTime() - timer->startTime))%60;
        timer->minutes = (int)((GetTime() - timer->startTime))/60;
    }
}

void DrawTimer(Screen *s, Timer *timer, Font font){
    int xPos, yPos;
    DrawTextEx(font, TextFormat("%02d:%02d",timer->minutes, timer->seconds), (Vector2){s->width/2.85,s->height/5.7},30,1,DARKGRAY);
}