#ifndef _SCREEN_H
#define _SCREEN_H

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
  PLAYING,
  PAUSED,
  ENDED
} GameState;

void UpdateScreen(Screen *s, int w, int h);


#endif // _SCREEN_H