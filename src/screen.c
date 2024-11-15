#include "screen.h"

void UpdateScreen(Screen *s, int w, int h ) {
    s->height = h;
    s->width = w;
}