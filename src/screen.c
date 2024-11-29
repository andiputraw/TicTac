#include "screen.h"
#include "raylib.h"

void UpdateScreen(Screen *s, int w, int h ) {
    s->height = h;
    s->width = w;
}

// void MainMenuDraw(Screen s){
//     int title_font_size = s.width*0.04;
//     int menu_font_size = s.width*0.03;
//     int btn_width = s.width*0.15;
//     int btn_height = s.height*0.1;
//     DrawText("Main Menu", s.width/2  - MeasureText("Main Menu",title_font_size)/2, s.height/4, title_font_size, BLACK);
//     DrawRectangle(s.width/2 - btn_width/2 ,s.height/2 - btn_height/2 ,btn_width,btn_height, LIGHTGRAY);
//     DrawText("Play", s.width/2  - MeasureText("Play",menu_font_size)/2, s.height/2 - menu_font_size/2, menu_font_size, BLACK);
    
// }
