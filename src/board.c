#include <stdlib.h>
#include <assert.h>
#include "board.h"
#include "raylib.h"
#include <stdio.h>

void CreateBoard(Board *b, GameState *gameState, int mode, Screen *s, Font font)
{
    b->mode = mode;
    b->screen = s;
    b->gameState = gameState;
    b->font = font;

    for (int i = 0; i < MAX_BOX_COUNT; i++)
    {
        CreateBox(&b->boxes[i]);
    }
    switch (mode)
    {
    case BOARD_3_X_3:
        b->board_len = 9;
        break;
    case BOARD_PYRAMID:
        break;
    case BOARD_5_X_5:
        break;
    default:
        assert("UNREACHABLE" || mode);
    }
}

void UpdateBoard(Board *b)
{
    int w = b->screen->width;
    int h = b->screen->height;
    float s = (w * 0.1);
    float center_x = (w / 2) - (s / 2);
    float center_y = (h / 2) - (s / 2);
    float left_upper_square_x = center_x - s;
    float left_upper_square_y = center_y - s;
    int offset = 5;
    Rectangle rec;
    Vector2 mouse;
    int i = 0, j = 0;

    int index;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            rec = (Rectangle){left_upper_square_x + (i * s) + (offset * i), left_upper_square_y + (j * s) + (offset * j), s, s};
            index = (j * 3) + i;
            b->boxes[index].rec = rec;
            mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, rec))
            {
                b->boxes[index].isHover = true;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    printf("%d", index);
                    if (b->boxes[index].value == BOX_EMPTY)
                    {
                        if (b->turn == FIRST)
                        {
                            b->boxes[index].value = BOX_O;
                            b->turn = SECOND;
                        }
                        else if (b->turn == SECOND)
                        {
                            b->boxes[index].value = BOX_X;
                            b->turn = FIRST;
                        }
                        if (__IsWin(b, index))
                        {
                            SetScoreLine(b,index);
                        }
                    }
                    b->boxes[index].isClicked = true;
                }
                else
                {
                    b->boxes[index].isClicked = false;
                }
            }
            else
            {
                b->boxes[index].isHover = false;
            }
        }
    }
    if(IsKeyPressed(KEY_R)){
        ResetBoard(b);
    }
}

void DrawBoard(Board *b)
{
    int i = 0;
    Box box;
    Vector2 center;
    Vector2 startLineLeft, endLineLeft;
    Vector2 startLineRight, endLineRight;
    for (i = 0; i < 9; i++)
    {
        box = b->boxes[i];
        if (box.isHover)
        {
            DrawRectangleLinesEx(box.rec, 4, RED);
        }
        else
        {
            DrawRectangleLinesEx(box.rec, 4, GRAY);
        }

        center = (Vector2){.x = (box.rec.x + (box.rec.width / 2)), .y = (box.rec.y + (box.rec.height / 2))};
        startLineLeft = (Vector2){.x = box.rec.x + box.rec.width * 0.1, .y = box.rec.y + box.rec.height * 0.1};
        endLineLeft = (Vector2){.x = box.rec.x + box.rec.width * 0.9, .y = box.rec.y + box.rec.height * 0.9};

        startLineRight = (Vector2){.x = box.rec.x + box.rec.width * 0.9, .y = box.rec.y + box.rec.height * 0.1};
        endLineRight = (Vector2){.x = box.rec.x + box.rec.width * 0.1, .y = box.rec.y + box.rec.height * 0.9};

        switch (box.value)
        {
        case BOX_X:
            // DrawText("X",box.rec.x + (box.rec.width / 2)-20,box.rec.y + (box.rec.height / 2)-25, 50, BLACK);
            DrawLineEx(startLineLeft, endLineLeft, box.rec.width / 8, BLUE);
            DrawLineEx(startLineRight, endLineRight, box.rec.width / 8, BLUE);
            // DrawRing(center, box.rec.width / 3.5 , box.rec.width / 2.5 , 0,360, 0 , BLUE);
            break;
        case BOX_O:
            DrawRing(center, box.rec.width / 3.5, box.rec.width / 2.5, 0, 360, 0, RED);
            break;
        case BOX_EMPTY:
            break;
        default:
            assert("UNREACHABLE" || box.value);
            break;
        }
    }
    if(__IsWin(b,i)){
        DrawLineEx(b->scoreLinePos.startPos, b->scoreLinePos.endPos, 8.0f,BLACK);
    }
    if(b->turn == FIRST){
        DrawTextEx(b->font,TextFormat("%s's Turn",b->gameState->p1.name),(Vector2){10,10},25,1, BLACK);
    }else if(b->turn == SECOND){
        DrawTextEx(b->font,TextFormat("%s's Turn",b->gameState->p2.name), (Vector2) {10,10},25,1,BLACK);
    }
}

void CreateBox(Box *box)
{
    box->value = BOX_EMPTY;
    box->isHover = false;
    box->isClicked = false;
    box->rec = (Rectangle){0, 0, 0, 0};
}

// int __GetBoxIndexFromCoordinate(int maxBoard, int x, int y){
//     return
// }

int __2Dto1D(int maxCol, int row, int col)
{
    return (row * maxCol) + col;
}

void __1DTo2D(int index, int maxCol, int *row, int *col)
{
    *row = index / maxCol;
    *col = index % maxCol;
}

//  6 (1D)
//  Row = 6/3 (2)
//  Col = 6%3 (0)
bool __IsWin(Board *b, int index)
{
    int col;
    int row;
    int currVal = b->boxes[index].value;

    if (b->mode == BOARD_3_X_3)
    {
        __1DTo2D(index, 3, &row, &col);
        // Horizontal
        if (
            b->boxes[__2Dto1D(3, row, 0)].value == currVal &&
            b->boxes[__2Dto1D(3, row, 1)].value == currVal &&
            b->boxes[__2Dto1D(3, row, 2)].value == currVal)
        {
            b->winCondition = HORIZONTAL;
            return true;
        }
        // Vertical
        if (
            b->boxes[__2Dto1D(3, 0, col)].value == currVal &&
            b->boxes[__2Dto1D(3, 1, col)].value == currVal &&
            b->boxes[__2Dto1D(3, 2, col)].value == currVal)
        {
            b->winCondition = VERTICAL;
            return true;
        }
        // Diagonal top left
        if ((col == 0 && row == 0) || (col == 2 && row == 2) || (col == 1 && row == 1))
        {
            if (
                b->boxes[__2Dto1D(3,0,0)].value == currVal &&
                b->boxes[__2Dto1D(3,1,1)].value == currVal &&
                b->boxes[__2Dto1D(3,2,2)].value == currVal)
            {
                b->winCondition = DIAGONAL_TOP_LEFT;
                return true;
            }
        }
        //Diagonal top right
        if ((col == 2 && row == 0) || (col == 0 && row == 2) || (col == 1 && row == 1))
        {
            if (
                b->boxes[__2Dto1D(3, 0,2)].value == currVal &&
                b->boxes[__2Dto1D(3, 1,1)].value == currVal &&
                b->boxes[__2Dto1D(3, 2,0)].value == currVal)
            {
                b->winCondition = DIAGONAL_TOP_RIGHT;
                return true;
            }
        }

        
        return false;
    }
    else if (b -> mode == BOARD_5_X_5){
        // Cara cek garis diagonal.

        // Jika kotak berada di kolom ke-1
        //
        // Cek ke atas, atau ke bawah, bisa dapet 2 kotak. 
        // kalau ke atas bisa. berarti arahnya BOT_LEFT to TOP RIGHT
        // kalau ke bawah bisa. berarti arahnya TOP_LEFT to BOT RIGHT
        // misal. row 2. bisa ke atas row 0 dan row 1. bisa ke bawah, row 4, row 5. bisa dua duanya
        //        row 4. bisa ke atas row 3 dan row 2. Hanya bisa ke bawah row 1. hanya bisa BOT_LEFT to TOP_RIGHT

        // Kondisi dibalik, jika kotak berada di kolom ke-4.
    }
    else
    {
        assert("UNREACHABLE" || false);
    }

}

void SetScoreLine(Board *b, int index){
    int row,col;
    __1DTo2D(index,3, &row, &col);
    if(b->winCondition == VERTICAL){
    b->scoreLinePos.startPos = (Vector2){.x = b->boxes[__2Dto1D(3, 0, col)].rec.x + b->boxes[__2Dto1D(3, 0, col)].rec.width/2,.y= b->boxes[__2Dto1D(3, 0, col)].rec.y};
    b->scoreLinePos.endPos = (Vector2){.x = b->boxes[__2Dto1D(3, 2, col)].rec.x + b->boxes[__2Dto1D(3, 0, col)].rec.width/2,.y= b->boxes[__2Dto1D(3, 2, col)].rec.y + b->boxes[__2Dto1D(3, 2, col)].rec.height};
    }else if(b->winCondition == HORIZONTAL){
        b->scoreLinePos.startPos = (Vector2){.x = b->boxes[__2Dto1D(3, row, 0)].rec.x, .y = b->boxes[__2Dto1D(3, row, 0)].rec.y+b->boxes[__2Dto1D(3, row, 0)].rec.height/2};
        b->scoreLinePos.endPos = (Vector2){.x = b->boxes[__2Dto1D(3, row, 2)].rec.x +  b->boxes[__2Dto1D(3, row, 2)].rec.width, .y = b->boxes[__2Dto1D(3, row, 2)].rec.y+b->boxes[__2Dto1D(3, row, 0)].rec.height/2};
    }else if(b->winCondition == DIAGONAL_TOP_LEFT){
        b->scoreLinePos.startPos = (Vector2){.x = b->boxes[__2Dto1D(3, 0, 0)].rec.x, .y=b->boxes[__2Dto1D(3, 0, 0)].rec.y};
        b->scoreLinePos.endPos = (Vector2){.x = b->boxes[__2Dto1D(3, 2, 2)].rec.x + b->boxes[__2Dto1D(3, 2, 2)].rec.width, .y = b->boxes[__2Dto1D(3, 2, 2)].rec.y +  b->boxes[__2Dto1D(3, 2, 2)].rec.width };
    }else if(b->winCondition == DIAGONAL_TOP_RIGHT){
        b->scoreLinePos.startPos = (Vector2){.x = b->boxes[__2Dto1D(3, 0, 2)].rec.x+b->boxes[__2Dto1D(3, 0, 2)].rec.width, .y=b->boxes[__2Dto1D(3, 0, 2)].rec.y};
        b->scoreLinePos.endPos = (Vector2){.x = b->boxes[__2Dto1D(3, 2, 0)].rec.x, .y = b->boxes[__2Dto1D(3, 2, 2)].rec.y + b->boxes[__2Dto1D(3, 2, 2)].rec.height };

    }
}

void ResetBoard(Board *b){
    for(int i = 0; i < 9; i++){
         b->boxes[i].value = BOX_EMPTY;
    }
    b->scoreLinePos.startPos = (Vector2) {0,0};
    b->scoreLinePos.endPos = (Vector2) {0,0};
    b->turn = FIRST;
}
