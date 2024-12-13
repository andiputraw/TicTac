#ifndef _SCREEN_H
#define _SCREEN_H

#include "player.h"
#include "raylib.h"
#include <stdbool.h>

typedef struct
{
  int width;
  int height;
} Screen;


typedef enum{
  FIRST,
  SECOND,
  NEITHER
} Turn;

typedef enum Scene
{
  MAIN_MENU = 0,
  SELECT_MODES_MENU,
  GAMEPLAY,
  LEADERBORAD_MENU
} Scene;

typedef enum
{
  PLAYING = 0,
  PAUSED,
  ENDED,
} GameStatus;

typedef enum{
  VSBOT = 0,
  VSPLAYER
} VsMode;

typedef enum{
  EASY,
  MEDIUM,
  HARD,
  NOTBOT,
} BotMode;

typedef enum{
  HORIZONTAL_MID,
  HORIZONTAL_LEFT,
  HORIZONTAL_RIGHT,
  VERTICAL_MID,
  VERTICAL_TOP,
  VERTICAL_BOT,
  DIAGONAL_TOP_LEFT_BEGIN,
  DIAGONAL_TOP_LEFT_MID,
  DIAGONAL_TOP_LEFT_END,
  DIAGONAL_TOP_RIGHT_BEGIN,
  DIAGONAL_TOP_RIGHT_MID,
  DIAGONAL_TOP_RIGHT_END
} ScoreCondition;

typedef struct{
  Scene scene;
  GameStatus gameStatus;
  VsMode vsMode;
  BotMode botMode;
  Player p1;
  Player p2;
} GameState;

typedef struct{
    int timeAmount;
    bool isRunning;
    float elapsedTime;
    int seconds;
    int minutes;
    Font font;
    GameState *gameState;
} Timer;


void UpdateScreen(Screen *s, int w, int h);

// Sementara
void CreateTimer(Timer *timer,GameState *gameState, Font font);
void UpdateTimer(Timer *timer);
void DrawTimer(Screen *screen, Timer *timer);


#endif // _SCREEN_H