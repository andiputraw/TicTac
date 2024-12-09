#ifndef _SCREEN_H
#define _SCREEN_H

#include "player.h"

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
  GAMEPLAY,
  LEADERBORAD_MENU
} Scene;

typedef enum
{
  PLAYING = 0,
  PAUSED,
  ENDED
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


void UpdateScreen(Screen *s, int w, int h);


#endif // _SCREEN_H