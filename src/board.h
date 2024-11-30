#include <stdbool.h>
#include "game.h"
#include "player.h"
#include "raylib.h"

#ifndef _BOARD_H
#define _BOARD_H

#define BOARD_3_X_3 1
#define BOARD_PYRAMID 2
#define BOARD_5_X_5 4

#define BOX_DRAW_SIZE 45
#define MAX_BOX_COUNT 42

#define BOX_EMPTY 0 
#define BOX_X 1
#define BOX_O 2


typedef struct {
    Rectangle rec;
    int value;
    bool isHover;
    bool isClicked;
} Box;

typedef struct {
    int mode;
    Box boxes[MAX_BOX_COUNT];
    int board_len;
    Screen *screen;
    Turn turn;
} Board;



void CreateBoard(Board *b, int mode, Screen *s);
void DrawBoard(Board *b);
void UpdateBoard(Board *b);
void ResetBoard(Board*b);
void CreateBox(Box *box);


int __2Dto1D(int maxCol, int row, int col);
void __1DTo2D(int index, int maxCol, int *row, int *col);
bool __IsWin(Board *b, int index);

#endif // _BOARD_H