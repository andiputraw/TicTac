#ifndef _SCREEN_H
#define _SCREEN_H

typedef struct {
  int width;
  int height;
} Screen;

void UpdateScreen(Screen *s, int w, int h );

#endif // _SCREEN_H