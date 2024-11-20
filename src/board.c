#include <stdlib.h>
#include <assert.h>
#include "board.h"
#include "raylib.h"



void CreateBoard(Board *b, int mode, Screen *s){
    b->mode = mode;
    b->screen = s;
    for(int i = 0; i < MAX_BOX_COUNT; i++){
        CreateBox(&b->boxes[i]);
    }
    switch(mode){
        case BOARD_3_X_3: 
            b->board_len = 9;
            break;
        case BOARD_PYRAMID: break;
        case BOARD_4_IN_A_ROW: break;
        case BOARD_5_X_5: break;
        default:
            assert("UNREACHABLE" || mode);
    }
}

void UpdateBoard(Board *b, Player p){
    int w = b->screen->width;
    int h = b->screen->height;
    float s = (w * 0.1);
    float center_x = (w / 2) - (s /  2);
    float center_y = (h / 2) - (s /  2);
    float left_upper_square_x = center_x - s ;
    float left_upper_square_y = center_y - s;
    int offset = 5;
    Rectangle rec;
    Vector2 mouse;
    int i = 0, j = 0;
    int index;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            rec = (Rectangle){left_upper_square_x + (i * s) + (offset * i ), left_upper_square_y + (j * s) + (offset * j ) ,s,s}; 
            index = (i * 3) + j;
            b->boxes[index].rec = rec;
            mouse = GetMousePosition(); 
            if (CheckCollisionPointRec(mouse, rec)){
                UpdateTurn(&p);
                b->boxes[index].isHover = true;
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                    if(b->boxes[index].value == BOX_EMPTY){
                        if(p.turn==FIRST_TURN){
                            b->boxes[index].value = BOX_O;
                        }else{
                            b->boxes[index].value = BOX_X;
                        }
                    }
                    b->boxes[index].isClicked = true;
                }else {
                    b->boxes[index].isClicked = false;
                }
            }else {
                b->boxes[index].isHover = false;
            } 
 
        }
    }
}


void DrawBoard(Board *b){
    int i = 0;
    Box box;
    for(i = 0; i < 9; i++) {
        box = b->boxes[i];
        if(box.isHover){
            DrawRectangleRec(box.rec, RED);    
        }else{
            DrawRectangleRec(box.rec, GRAY);
        }
    
        switch(box.value){
            case BOX_X: 
                DrawText("X",box.rec.x + (box.rec.width / 2)-20,box.rec.y + (box.rec.height / 2)-25, 50, BLACK);
                break;
            case BOX_O:
                DrawCircle((box.rec.x + (box.rec.width / 2)), (box.rec.y + (box.rec.height / 2)) , box.rec.width / 2, BLACK); 
                break;
            case BOX_EMPTY: 
                break;
            default:
                assert("UNREACHABLE" || box.value);
                break;
        }
    }
}

void CreateBox(Box *box){
    box->value = BOX_EMPTY;
    box->isHover = false;
    box->isClicked = false;
    box->rec = (Rectangle){0, 0, 0 ,0};
}
