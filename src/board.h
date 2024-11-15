#include "screen.h"

#ifndef _BOARD_H
#define _BOARD_H

#define BOARD_3_X_3 1
#define BOARD_PYRAMID 2
#define BOARD_4_IN_A_ROW 3
#define BOARD_5_X_5 4

#define BOX_DRAW_SIZE 45

typedef struct {
    int mode;
    int *val;
    int board_len;
    Screen *screen;
} Board;

void CreateBoard(Board *b, int mode, Screen *s);
void DrawBoard(Board *b);
void UpdateBoard(Board *b);
void ResetBoard(Board*b);

#endif // _BOARD_H