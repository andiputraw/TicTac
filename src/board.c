#include <stdlib.h>
#include <assert.h>
#include "board.h"
#include "raylib.h"
#include "math.h"
#include <stdio.h>
#include <string.h>

void CreateBoard(Board *b, GameState *gameState, int mode, Screen *s,Timer *timer, Font font, Leaderboard *leaderboard)
{
    b->mode = mode;
    b->screen = s;
    b->gameState = gameState;
    b->font = font;
    b->turnCount = 1;
    b->timer = timer;
    b->lineCount = 0;
    b->leaderboard = leaderboard;
    b->isResultWritten = false;

    for (int i = 0; i < MAX_BOX_COUNT; i++)
    {
        CreateBox(&b->boxes[i]);
    }
    switch (mode)
    {
    case BOARD_3_X_3:
        b->board_len = 9;
        break;
    case BOARD_5_X_5:
        b->board_len = 25;
        break;
    default:
        assert("UNREACHABLE" || mode);
    }
}

void UpdateBoard(Board *b)
{
    int w = b->screen->width;
    int h = b->screen->height;
    float s;
    float center_x = (w / 2) - (s / 2);
    float center_y = (h / 2) - (s / 2);
    float left_upper_square_x;
    float left_upper_square_y;
    int offset;
    Rectangle rec;
    Vector2 mouse;
    int i = 0, j = 0;
    bool isThinking;
    int index;


    switch (b->mode)
    {
    case BOARD_3_X_3:
        b->board_len = 9;
        break;
    case BOARD_5_X_5:
        b->board_len = 25;
        break;
    default:
        assert("UNREACHABLE" || b->mode);
    }

    if(b->mode==BOARD_3_X_3){
        offset = 5;
        s = (w * 0.1);
        center_x = (w / 2) - (s / 2);
        center_y = (h / 2) - (s / 2);
        left_upper_square_x = center_x - s;
        left_upper_square_y = center_y - s;
    }else if(b->mode==BOARD_5_X_5){
        offset = 3;
        s = (w * 0.06);
        left_upper_square_x = center_x/1.21 - s;
        left_upper_square_y = center_y/1.5 - s;
    }


    for (i = 0; i < sqrt(b->board_len); i++)
    {
        for (j = 0; j < sqrt(b->board_len); j++)
        {
            rec = (Rectangle){left_upper_square_x + (i * s) + (offset * i), left_upper_square_y + (j * s) + (offset * j), s, s};
            index = (j * sqrt(b->board_len)) + i;
            b->boxes[index].rec = rec;
            mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, rec) && b->gameState->gameStatus == PLAYING)
            {
                b->boxes[index].isHover = true;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (b->boxes[index].value == BOX_EMPTY)
                    {
                        if(b->gameState->vsMode == VSBOT){
                            PlayVsBot(b, index);
                        }else{
                            PlayVsPlayer(b,index);
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
    b->lineCount = b->gameState->p1.score + b->gameState->p2.score;
    if(b->gameState->gameStatus == ENDED && IsKeyPressed(KEY_R)){ 
        __RecordResultToFile(b);
        RestartBoard(b);
    }
    if(b->gameState->gameStatus == ENDED && IsKeyPressed(KEY_B)){
        __RecordResultToFile(b);
        BackToMainMenu(b);
    }
    
}

int __CalculateEloWin( int currElo, int botDiff) {
    // base increase = 20
    // formula ini harus nge-nerf hasil dari permainan jika
    // currElo = 300 jika easy, 600 jika medium. 900 jika hard.

    return currElo  + (20 + (botDiff * 20)/ 1 + ((currElo / (botDiff * 300)) ));
}

int __CalculateEloLose( int currElo, int botDiff) {
    return currElo - (20 + (botDiff * 20) / ((botDiff * 300 /( currElo + 1))));
}

void __RecordResultToFile(Board *b) {
    History history;
    PlayerElo playerElo;
    if(!b->isResultWritten){
        if(b->gameState->vsMode == VSPLAYER){
            memcpy(&history.p1, &b->gameState->p1, sizeof(Player));
            memcpy(&history.p2, &b->gameState->p2, sizeof(Player));
            history.game_mode = b->mode;
            for(int i = 0; i < 25; i++){
                history.BoardState[i] = b->boxes[i].value;
            }
            WriteHistory(b->leaderboard, &history);
        }
        if(b->gameState->vsMode == VSBOT){
            if (GetPlayerElo(b->leaderboard, &playerElo, b->gameState->p1.name)){
                if(b->gameState->p1.score != 0){
                    playerElo.elo = __CalculateEloWin(playerElo.elo, b->gameState->botMode + 1);
                }else {
                    playerElo.elo = __CalculateEloLose(playerElo.elo, b->gameState->botMode + 1);
                }
            }else {
                strcpy(&playerElo.name, b->gameState->p1.name);
                playerElo.elo = 100;
            }
            WritePlayerElo(b->leaderboard, &playerElo);
        }
        b->isResultWritten = true;
    }
    
}

void DrawBoard(Board *b)
{
    int i = 0;
    Box box;
    Vector2 center;
    Vector2 startLineLeft, endLineLeft;
    Vector2 startLineRight, endLineRight;
    for (i = 0; i < b->board_len; i++)
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
        // Draw Line
        for(int i = 0; i < b->lineCount;i++){

            DrawLineEx(b->scoreLinePos[i].startPos, b->scoreLinePos[i].endPos, 8.0f,BLACK);
        }
    }

    // Draw Turn
    if(b->gameState->vsMode == VSPLAYER){
        if(b->turn == FIRST){
            Vector2 turnPos = (Vector2){.x = (b->screen->width/2) - MeasureTextEx(b->font,TextFormat("%s's Turn (Circle)",b->gameState->p1.name),25,1).x/2,.y=(b->screen->height/1.2)};
            DrawTextEx(b->font,TextFormat("%s's Turn (Circle)",b->gameState->p1.name),turnPos,25,1, RED);
        }else if(b->turn == SECOND){
            Vector2 turnPos = (Vector2){.x = (b->screen->width/2) - MeasureTextEx(b->font,TextFormat("%s's Turn (Cross)",b->gameState->p2.name),25,1).x/2,.y=(b->screen->height/1.2)};
            DrawTextEx(b->font,TextFormat("%s's Turn (Cross)",b->gameState->p2.name), turnPos,25,1,BLUE);
        }
    }
    //Draw Score
    if(b->mode == BOARD_5_X_5){
        const char* scoreText1;
        const char* scoreText2;
        int fontSize = b->screen->width*0.035;
        Vector2 pos = (Vector2){0,0};
        Vector2 pos2 = (Vector2){0,b->screen->height/9};
        if(b->gameState->vsMode == VSBOT){
            scoreText1 = "Your Score: ";
            scoreText2 = "Bot Score: ";
        }else{
            scoreText1 =TextFormat("%s(Circle) Score: ",b->gameState->p1.name);
            scoreText2 =TextFormat("%s(Cross) Score: ",b->gameState->p2.name);
        }
        DrawTextEx(b->font, TextFormat("%s %d", scoreText1, b->gameState->p1.score),pos, fontSize,1,BLACK);
        DrawTextEx(b->font, TextFormat("%s %d", scoreText2, b->gameState->p2.score),pos2,fontSize, 1,BLACK);
    }


    if(b->gameState->gameStatus == ENDED &&b->gameState->scene == GAMEPLAY){
        DrawGameOverScene(b);
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
    if(row < 0 || col < 0 || col >= maxCol||row >=maxCol){
        return 40;
    }else{
        return (row * maxCol) + col;
    }
}

void __1DTo2D(int index, int maxCol, int *row, int *col)
{
    *row = index / maxCol;
    *col = index % maxCol;
}

//  6 (1D)
//  Row = 6/3 (2)
//  Col = 6%3 (0)
bool __IsScoring(Board *b, int index)
{
    int col;
    int row;
    int currVal = b->boxes[index].value;
    int maxCol = sqrt(b->board_len);
    __1DTo2D(index, maxCol, &row, &col);
        /*----------- Horizontal ---------------*/
    if (
            (b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            (b->boxes[__2Dto1D(maxCol, row, col-1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol, row, col+1)].value == currVal))
        {
            // printf("horiz mid");
            b->scoreCondition = HORIZONTAL_MID;
            return true;
        }else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol, row, col+1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol, row, col+2)].value == currVal)
        {
            // printf("%d %d\n", col+1,col+2);
            // printf("horiz left");

            b->scoreCondition = HORIZONTAL_LEFT;
            return true;  
        }else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol, row, col-1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol, row, col-2)].value == currVal)
        {
                b->scoreCondition = HORIZONTAL_RIGHT;
                return true;
   
        }
        /* -------- Vertical --------- */
        if ((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol, row-1, col)].value == currVal &&
            b->boxes[__2Dto1D(maxCol, row+1, col)].value == currVal)
        {
            // printf("verti mid");

            b->scoreCondition = VERTICAL_MID;
            return true;
        }
        else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol, row+1, col)].value == currVal &&
            b->boxes[__2Dto1D(maxCol, row+2, col)].value == currVal)
        {
            // printf("verti top");

            b->scoreCondition = VERTICAL_TOP;
            return true;
        }
        else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol, row-1, col)].value == currVal &&
            b->boxes[__2Dto1D(maxCol, row-2, col)].value == currVal)
        {
            // printf("verti bot");

            b->scoreCondition = VERTICAL_BOT;
            return true;
        }
        /*-------- Diagonal Top Left to Bottom Right--------*/
        if ((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            (b->boxes[__2Dto1D(maxCol,row+1,col+1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol,row-1,col-1)].value == currVal))
        {
            // printf("dig top L mid");

                b->scoreCondition = DIAGONAL_TOP_LEFT_MID;
                return true;
        }
        else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            (b->boxes[__2Dto1D(maxCol,row+1,col+1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol,row+2,col+2)].value == currVal))
        {
            // printf("dig top L beg");

            b->scoreCondition = DIAGONAL_TOP_LEFT_BEGIN;
            return true;
        }
        else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol,row-1,col-1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol,row-2,col-2)].value == currVal)
        {
            // printf("dig top L end");

            b->scoreCondition = DIAGONAL_TOP_LEFT_END;
            return true;
        }
        /*-------- Diagonal Top Right to Bottom Left--------*/
        if ((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            (b->boxes[__2Dto1D(maxCol,row-1,col+1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol,row+1,col-1)].value == currVal))
        {

            // printf("diagonal top right mid");

                b->scoreCondition = DIAGONAL_TOP_RIGHT_MID;
                return true;
        }
        else if((b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            (b->boxes[__2Dto1D(maxCol,row+1,col-1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol,row+2,col-2)].value == currVal))
        {
            // printf("diagonal top right begin");

            b->scoreCondition = DIAGONAL_TOP_RIGHT_BEGIN;
            return true;
        }
        else if(
            (b->boxes[__2Dto1D(maxCol, row, col)].value == currVal)&&
            b->boxes[__2Dto1D(maxCol,row-1,col+1)].value == currVal &&
            b->boxes[__2Dto1D(maxCol,row-2,col+2)].value == currVal)
        {
            // printf("diagonal top right end");
            b->scoreCondition = DIAGONAL_TOP_RIGHT_END;
            return true;
        }
    // }
    // else
    // {
    //     assert("UNREACHABLE" || false);
    // }
    return false;

}

void SetScoreLine(Board *b, int index){
    int row,col, maxCol;
    maxCol = sqrt(b->board_len);
    __1DTo2D(index,maxCol, &row, &col);

    switch (b->scoreCondition)
    {
    case VERTICAL_MID:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row-1, col);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row+1, col);
        break;
    case VERTICAL_TOP:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row, col);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row+2, col);
        break;
    case VERTICAL_BOT:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row-2, col);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row, col);
        break;
    case HORIZONTAL_MID:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row, col-1);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row, col+1);
        break;
    case HORIZONTAL_LEFT:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row, col);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row, col+2);
        break;
    case HORIZONTAL_RIGHT:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row, col-2);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row, col);
        break;
    case DIAGONAL_TOP_LEFT_MID:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row-1, col-1);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row+1, col+1);
        break;
    case DIAGONAL_TOP_LEFT_BEGIN:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row, col);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row+2, col+2);
        break;
    case DIAGONAL_TOP_LEFT_END:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row-2, col-2);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row, col);
        break;
    case DIAGONAL_TOP_RIGHT_MID:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row-1, col+1);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row+1, col-1);
        break;
    case DIAGONAL_TOP_RIGHT_BEGIN:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row, col);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row+2, col-2);
        break;
    case DIAGONAL_TOP_RIGHT_END:
        b->scoreLinePos[b->lineCount].startIndex = __2Dto1D(maxCol, row-2, col+2);
        b->scoreLinePos[b->lineCount].endIndex = __2Dto1D(maxCol, row, col);
        break;
    default:
        break;
    }
    // printf("start: %d end:%d\n",b->scoreLinePos[b->lineCount].startIndex, b->scoreLinePos[b->lineCount].endIndex);
    if(b->scoreCondition==VERTICAL_MID||b->scoreCondition==VERTICAL_TOP||b->scoreCondition==VERTICAL_BOT){
        b->scoreLinePos[b->lineCount].startPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.x + b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.width/2,.y= b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.y};
        b->scoreLinePos[b->lineCount].endPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.x + b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.width/2,.y= b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.y + b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.height};
    }else if(b->scoreCondition==HORIZONTAL_MID||b->scoreCondition==HORIZONTAL_LEFT||b->scoreCondition==HORIZONTAL_RIGHT){
        b->scoreLinePos[b->lineCount].startPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.x, .y = b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.y+b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.height/2};
        b->scoreLinePos[b->lineCount].endPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.x +  b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.width, .y = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.y+b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.height/2};
    }else if(b->scoreCondition==DIAGONAL_TOP_LEFT_MID||b->scoreCondition==DIAGONAL_TOP_LEFT_BEGIN||b->scoreCondition==DIAGONAL_TOP_LEFT_END){
        b->scoreLinePos[b->lineCount].startPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.x, .y=b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.y};
        b->scoreLinePos[b->lineCount].endPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.x + b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.width, .y = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.y +  b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.width };
    }else if(b->scoreCondition==DIAGONAL_TOP_RIGHT_MID||b->scoreCondition==DIAGONAL_TOP_RIGHT_BEGIN||b->scoreCondition==DIAGONAL_TOP_RIGHT_END){
         b->scoreLinePos[b->lineCount].startPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.x+b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.width, .y=b->boxes[b->scoreLinePos[b->lineCount].startIndex].rec.y};
         b->scoreLinePos[b->lineCount].endPos = (Vector2){.x = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.x, .y = b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.y + b->boxes[b->scoreLinePos[b->lineCount].endIndex].rec.height };
    }

}

void RestartBoard(Board *b){
    for(int i = 0; i < b->board_len; i++){
         b->boxes[i].value = BOX_EMPTY;
    }
    for(int i = 0; i <=b->lineCount; i++){
        b->scoreLinePos[i].startPos = (Vector2) {0,0};
        b->scoreLinePos[i].endPos = (Vector2) {0,0};
    }
    b->turn = FIRST;
    b->gameState->gameStatus = PLAYING;
    b->turnCount = 0;
    b->gameState->p1.score = 0;
    b->gameState->p2.score = 0;
    b->lineCount = 0;
    b->isResultWritten = false;
}
void BackToMainMenu(Board *b){
    for(int i = 0; i < b->board_len; i++){
         b->boxes[i].value = BOX_EMPTY;
    }
    for(int i = 0; i <=b->lineCount; i++){
        b->scoreLinePos[i].startPos = (Vector2) {0,0};
        b->scoreLinePos[i].endPos = (Vector2) {0,0};
    }
    b->turn = FIRST;
    b->turnCount = 0;
    b->gameState->p1.score = 0;
    b->gameState->p2.score = 0;
    b->gameState->scene = MAIN_MENU;
    b->lineCount = 0;
}

void DrawGameOverScene(Board *b){
    Rectangle rec = (Rectangle) {.height = b->screen->height, .width = b->screen->width, .x = 0, .y = 0};
    const char *gameOverTxt, *winnerTxt, *optionTxt;
    int marginTop;
    int fontSize;
    marginTop = b->screen->height * 0.05;
    fontSize = b->screen->height * 0.1;
    gameOverTxt = "GAME OVER";
    if(b->gameState->vsMode == VSPLAYER){
        if(b->turn == SECOND){
            winnerTxt = TextFormat("%s(Circle) WIN!",b->gameState->p1.name);
        }else if(b->turn == FIRST){
            winnerTxt = TextFormat("%s(Cross) WIN!",b->gameState->p2.name);
        }else if(b->turn == NEITHER){
            winnerTxt = "DRAW!";
        }
    }else{
        if(b->turn == FIRST){
            winnerTxt = "YOU WIN!";
        }else if(b->turn == SECOND){
            winnerTxt = "YOU LOSE!";
        }else if(b->turn == NEITHER){
            winnerTxt = "DRAW!";
        }
    }
    optionTxt = "Press 'r' to restart or 'b' to main menu"; 

    DrawRectangleRec(rec, Fade(WHITE, 0.8));
    DrawTextEx(b->font,gameOverTxt, (Vector2){b->screen->width/2 - MeasureTextEx(b->font, gameOverTxt, fontSize,1).x/2, b->screen->height/3},fontSize, 1, DARKGRAY);
    DrawTextEx(b->font,winnerTxt,(Vector2) {b->screen->width/2 - MeasureTextEx(b->font, winnerTxt, fontSize*0.8,1).x/2, (b->screen->height/2 - (fontSize*0.8)/2) + marginTop},fontSize*0.8,1, DARKGRAY);
    DrawTextEx(b->font, optionTxt, (Vector2) {b->screen->width/2 - MeasureTextEx(b->font, optionTxt, fontSize*0.5, 1).x/2, (b->screen->height/2 - (fontSize*0.5)/2) +(marginTop*3)},fontSize*0.5,1, DARKGRAY);

}

int CalculateBotIndex(Board *b, int index){
    if(b->gameState->botMode==EASY){
        return CalculateEasyBot(b);
    }else if(b->gameState->botMode ==MEDIUM){
        return CalculateMediumBot(b,index);
    }else{
        return CalculateHardBot(b,index);
    }
}

void PlayVsBot(Board *b, int index){
    int botIndex;
    b->boxes[index].value = BOX_O;
    printf("%d\n", b->turnCount);
    if (__IsScoring(b, index))
      {
        SetScoreLine(b,index);
        b->gameState->p1.score++;
          if(b->mode == BOARD_3_X_3){
            b->turnCount = 0;
            b->gameState->gameStatus = ENDED;
          }else if(b->mode == BOARD_5_X_5){
            // b->gameState->p1.score++;
            if(b->gameState->p1.score >= 5){
                b->turnCount = 0;
                b->gameState->gameStatus = ENDED;
            }else{
                b->turn = SECOND;
                b->turnCount++;
            }
          }
      }else{
        b->turn = SECOND;
        b->turnCount++;
    }
    if (b->turn == SECOND)
    {
        botIndex = CalculateBotIndex(b, index);
        if(botIndex >= 0 && botIndex <=b->board_len){
            b->boxes[botIndex].value = BOX_X;
            if(__IsScoring(b, botIndex)){
                b->gameState->p2.score++;
                b->lineCount++;
                SetScoreLine(b,botIndex);
                if(b->mode == BOARD_3_X_3){
                    b->turnCount = 0;
                    b->gameState->gameStatus = ENDED;
                }
                else if (b->mode == BOARD_5_X_5)
                {
                    // b->gameState->p2.score++;
                    if(b->gameState->p2.score >= 5){
                        b->turnCount = 0;
                        b->gameState->gameStatus = ENDED;
                    }else{
                        b->turn = FIRST;
                        b->turnCount++;
                    }
                }
            }else{
                b->turn = FIRST;
                b->turnCount++;
            }
        }
      }
    if(b->turnCount > b->board_len){
        if(b->gameState->p1.score ==b->gameState->p2.score){
          b->turn = NEITHER;

        }else if(b->gameState->p1.score > b->gameState->p2.score){
            b->turn =FIRST;

        }else if(b->gameState->p1.score < b->gameState->p2.score){
            b->turn =SECOND;
        }
          b->turnCount = 0;
          b->gameState->gameStatus = ENDED;
    }
}

void PlayVsPlayer(Board *b, int index){
    b->turnCount++;
    printf("%d", b->turnCount);
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
    if (__IsScoring(b, index))
    {
        SetScoreLine(b,index);
        if(b->mode == BOARD_3_X_3){
            if(b->turn == SECOND){
                b->gameState->p1.score++;
            }else{
                b->gameState->p2.score++;

            }
            b->turnCount = 0;
            b->gameState->gameStatus = ENDED;
            if(b->turn == SECOND){
                b->gameState->p1.score++;
            }else{
                b->gameState->p2.score++;
            }
        }else if(b->mode == BOARD_5_X_5){
            if(b->turn == SECOND){
                b->gameState->p1.score++;
                if(b->gameState->p1.score >= 5){
                    b->turnCount = 0;
                    b->gameState->gameStatus = ENDED;
                }
            }else{
                b->gameState->p2.score++;
                if(b->gameState->p2.score >= 5){
                    b->turnCount = 0;
                    b->gameState->gameStatus = ENDED;
                }
            }
        }
    }
    if(b->turnCount >= b->board_len){
        if(b->gameState->p1.score ==b->gameState->p2.score){
          b->turn = NEITHER;

        }else if(b->gameState->p1.score > b->gameState->p2.score){
            b->turn =SECOND;

        }else if(b->gameState->p1.score < b->gameState->p2.score){
            b->turn =FIRST;
        }
          b->turnCount = 0;
          b->gameState->gameStatus = ENDED;
    }
}

int CalculateEasyBot(Board *b){
    int index;
    for(int i = 0; i <= b->board_len; i++){
        index = GetRandomValue(0,b->board_len-1);
        if(b->boxes[index].value == BOX_EMPTY){
            return index;
        }
    }
    return -1;
}
int CalculateMediumBot(Board *b, int index){
    int randIndex;
    // Cek apakah player akan menang
    if(b->turnCount>1){
        for (int i = 0; i < b->board_len; i++)
        {
            if(b->boxes[i].value==BOX_EMPTY){
                b->boxes[i].value = BOX_O;
                if(__IsScoring(b,i)){
                    b->boxes[i].value = BOX_EMPTY;
                    return i;
                }
                b->boxes[i].value = BOX_EMPTY;
            }
        }

    }else{
        for(int i = 0; i < b->board_len; i++){
            randIndex = GetRandomValue(0,b->board_len);
            if(b->boxes[randIndex].value == BOX_EMPTY){
                return randIndex;
            }
        }

    }
    return -1;
}
int CalculateHardBot(Board *b, int index){
    int randValue;
    if(b->turnCount>1){
        // Cek apakah bisa menang
        for(int i = 0; i <= b->board_len; i++){
            if(b->boxes[i].value == BOX_EMPTY){
                    b->boxes[i].value = BOX_X;
                    if(__IsScoring(b,i)){
                        return i;
                    }
                    b->boxes[i].value = BOX_EMPTY;
                }
        }

        // Cek apakah pemain akan menang
        for (int i = 0; i < b->board_len; i++)
        {           
            if(b->boxes[i].value == BOX_EMPTY){
                b->boxes[i].value = BOX_O;
                if(__IsScoring(b,i)){
                    b->boxes[i].value = BOX_EMPTY;
                    return i;
                }
                b->boxes[i].value = BOX_EMPTY;
            }
        } 
            
    }else{
        for (int i = 0; i < b->board_len; i++)
        {
            // Cek corner/edge/center
            randValue = GetRandomValue(0,10);
                if(b->boxes[i].value == BOX_EMPTY){
                if(i%2==0){
                    if(randValue > 5){
                        return i;
                        }
                    }
                }

            }
        }

    return CalculateEasyBot(b);

}

