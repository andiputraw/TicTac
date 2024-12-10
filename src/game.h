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
  SECOND
} Turn;

typedef enum
{
  MAIN_MENU = 0,
  SELECT_MODES_MENU,
  GAMEPLAY
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
  HORIZONTAL,
  VERTICAL,
  DIAGONAL_TOP_LEFT,
  DIAGONAL_TOP_RIGHT,
} WinCondition;

typedef struct{
  Scene scene;
  GameStatus gameStatus;
  VsMode vsMode;
  Player p1;
  Player p2;

} GameState;

// Sementara
typedef struct{
    int seconds;
    int minutes;
    bool isRunning;
    double startTime;    
    double pauseTime; 
} Timer;


void UpdateScreen(Screen *s, int w, int h);

// Sementara
void CreateTimer(Timer *timer);
void UpdateTimer(GameState *gameState, Timer *timer);
void DrawTimer(Screen *screen, Timer *timer, Font font);


#endif // _SCREEN_H