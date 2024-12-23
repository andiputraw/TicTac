/**
 * board.h
 * Author : Andi Putra Wijaya & Maulana Ishak
 * Created: 15 November 2024
 * 
 * Header file untuk board.c. Yang digunakan untuk menampilkan papan permainan Tic Tac Toe beserta User Interfacenya
 */
#include <stdbool.h>
#include "game.h"
#include "game.h"
#include "raylib.h"
#include "leaderboard.h"

#ifndef _BOARD_H
#define _BOARD_H

#define BOARD_3_X_3 1
#define BOARD_5_X_5 2
#define BOARD_PYRAMID 3

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

typedef struct{
    int startIndex;
    int endIndex;
    Vector2 startPos;
    Vector2 endPos;
} ScoreLinePos;

typedef struct {
    int mode;
    Box boxes[MAX_BOX_COUNT];
    int board_len;
    Screen *screen;
    Turn turn;
    GameState *gameState;
    ScoreLinePos scoreLinePos[10];
    ScoreCondition scoreCondition;
    int lineCount;
    Font font;
    int turnCount;
    Timer *timer;
    Leaderboard *leaderboard;
    bool isResultWritten;
} Board;



void CreateBoard(Board *b, GameState *gameState, int mode, Screen *s,Timer *timer, Font font, Leaderboard *leaderboard);
void DrawBoard(Board *b);
void UpdateBoard(Board *b);
void RestartBoard(Board*b);
void CreateBox(Box *box);

void SetScoreLineIndex(Board *b,int index);
void SetScoreLinePos(Board *b);
void DrawGameOverScene(Board *b);
void PlayVsBot(Board *b, int index);
int CalculateBotIndex(Board *b, int index);
int CalculateEasyBot(Board *b);
int CalculateMediumBot(Board *b, int index);
int CalculateHardBot(Board *b, int index);
void PlayVsPlayer(Board *b , int index);
void BackToMainMenu(Board *b);
void DrawScoreText(Board *b);
void DrawTurnText(Board *b);

int __2Dto1D(int maxCol, int row, int col);
void __1DTo2D(int index, int maxCol, int *row, int *col);
bool __IsScoring(Board *b, int index);
void __RecordResultToFile(Board *b);
int __CalculateEloWin( int currElo, int botDiff);
int __CalculateEloLose( int currElo, int botDiff);

#endif // _BOARD_H