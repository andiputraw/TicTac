#include "control.h"
#include <raylib.h>
#include "string.h"

void CreateButton(Button *btn, Rectangle rec, char text[20], Color color, Color textColor, int fontSize){
    btn->rect = rec;
    strncpy(btn->text, text, 20);
    btn->color = color;
    btn->isClicked = false;
    btn->isHover = false;
    btn->fontSize = fontSize;
    btn->textColor = textColor;
}

void UpdateButton(Button *this) {
    Vector2 mousePos = GetMousePosition();

    if(CheckCollisionPointRec(mousePos, this->rect)){
        this->isHover = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            this->isClicked = true;
        }else {
            this->isClicked = false;
        }
    }else {
        this->isHover = false;
    }
}

void DrawButton(Button *btn) {
    int textX = btn->rect.x + ( btn->rect.width/2  - MeasureText(btn->text,btn->fontSize)/2);
    int textY = btn->rect.y + (btn->rect.height/2 - btn->fontSize/2);
    Color hoverColor = __DarkenColor(btn->color, 0.2);

    if(btn->isHover){
        DrawRectangleRec(btn->rect, hoverColor);
    }else {
        DrawRectangleRec(btn->rect, btn->color);
    }
    DrawText(btn->text, textX, textY, btn->fontSize, btn->textColor);
}

void CreateInputText(InputText *inputText, Rectangle rec, char inputValueContainer[255]){
    inputText->rect = rec;
    inputText->value = &(inputValueContainer[0]);
    inputText->isFocus = false;
    inputText->valueLen = 0;
}

void UpdateInputText(InputText *inputText){
    Vector2 mousePos = GetMousePosition();

    if(CheckCollisionPointRec(mousePos, inputText->rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        inputText->isFocus = true;
    }

    if(!CheckCollisionPointRec(mousePos, inputText->rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        inputText->isFocus = false;
    }

    if(inputText->isFocus){
        __InputKeyToValue(inputText);
    }
}

void DrawInputText(InputText *this){
    int textXPos = this->rect.x + 20;
    int textYPos = this->rect.y + (this->rect.height / 4);
    // https://colorhunt.co/palette/a59d84c1baa1d7d3bfecebde
    Color normalColor = (Color){0xEC, 0xEB, 0xDE, 0xFF};
    // D7D3BF
    Color focusColor = (Color){0xD7, 0xD3, 0xBF, 0xFF};

    if(this->isFocus){
        DrawRectangleRec(this->rect, focusColor);
        DrawRectangleLinesEx(this->rect, 2, BLACK);
    }else {
        DrawRectangleRec(this->rect, normalColor);
    }
    DrawText(this->value, textXPos, textYPos, 16, BLACK);
}


void __InputKeyToValue(InputText *text){
    // TODO: Hentikan saat value_len 255
    for(int c = 'A'; c < 'Z'; c++) {
        if(IsKeyPressed(c)){
            // printf("ISINYA %s", text->value);
            text->valueLen += 1;
            text->value[text->valueLen - 1] = c;
        }
    }
    if(IsKeyPressed(KEY_BACKSPACE)){
        printf("ISINYA %s", text->value);
        text->value[text->valueLen - 1] = '\0';
        text->valueLen -= 1;
    }
    if(IsKeyPressed(KEY_SPACE)){
        text->value[text->valueLen] = ' ';
        text->valueLen += 1;
    }
}

Color __DarkenColor(Color color, float factor) {
    if (factor < 0.0f) factor = 0.0f; 
    if (factor > 1.0f) factor = 1.0f; 

    return (Color) {
        (unsigned char)(color.r * (1.0f - factor)),
        (unsigned char)(color.g * (1.0f - factor)),
        (unsigned char)(color.b * (1.0f - factor)),
        color.a
    };
}
