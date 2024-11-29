#ifndef _SCREEN_H
#define _SCREEN_H

typedef struct {
  int width;
  int height;
} Screen;

typedef enum{
  MAIN_MENU,
  SELECT_MODES_MENU,
  GAMEPLAY
}Scene;

typedef enum{
  PLAYING,
  PAUSED,
  ENDED
}GameState;

void UpdateScreen(Screen *s, int w, int h );
// void MainMenuDraw(Screen s);

#endif // _SCREEN_H