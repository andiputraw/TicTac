#include "board.h"
#include "raylib.h"
#include <stdlib.h>
#include <assert.h>

void CreateBoard(Board *b, int mode, Screen *s){
    b->mode = mode;
    b->screen = s;
    switch(mode){
        case BOARD_3_X_3: 
            b->val = (int*)malloc(sizeof(int) * 9);
            b->board_len = 9;
            break;
        case BOARD_PYRAMID: break;
        case BOARD_4_IN_A_ROW: break;
        case BOARD_5_X_5: break;
        default:
            assert("UNREACHABLE" || 0);
    }
}


void DrawBoard(Board *b){
    int w = b->screen->width;
    int h = b->screen->height;
    float s = (w * 0.1);
    float center_x = (w / 2) - (s /  2);
    float center_y = (h / 2) - (s /  2);
    float left_upper_square_x = center_x - s ;
    float left_upper_square_y = center_y - s;
    int offset = 5;
    Rectangle rec;
    Color color = GRAY;
    Color color_hover = RED;
    Vector2 mouse;
    int i = 0, j = 0;

    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            rec = (Rectangle){left_upper_square_x + (i * s) + (offset * i ), left_upper_square_y + (j * s) + (offset * j ) ,s,s}; 
            mouse = GetMousePosition(); 
            if (CheckCollisionPointRec(mouse, rec)){
                DrawRectangleRec(rec, color_hover);
            }else {
                DrawRectangleRec(rec, color);
            } 
        }
    }
}
