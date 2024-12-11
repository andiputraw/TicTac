#include "control.h"
#include <raylib.h>
#include "string.h"

ButtonStyle CreateButtonStyle(Color bgColor, Color textColor, Font font, int fontSize){
    return (ButtonStyle) {
        .bgColor = bgColor,
        .textColor = textColor,
        .fontSize = fontSize,
        .disabled = false,
        .font = font,
    };
}

void CreateButton(Button *btn, Rectangle rec, char text[20], ButtonStyle style){
    btn->rect = rec;
    strncpy(btn->text, text, 20);
    btn->isClicked = false;
    btn->isHover = false;
    btn->style = style;
}

void UpdateButton(Button *this) {
    if(this->style.disabled){
        return;
    }

    Vector2 mousePos = GetMousePosition();

    if(CheckCollisionPointRec(mousePos, this->rect)){
        this->isHover = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            this->isClicked = true;
        }else {
            this->isClicked = false;
        }
    }else {
        this->isClicked = false;
        this->isHover = false;
    }
}

void DrawButton(Button *btn) {
    Vector2 textSize = MeasureTextEx(btn->style.font, btn->text, btn->style.fontSize, 2);
    // printf("x: %d y: %d\n", textSize.x, textSize.y );
    int textX = btn->rect.x  + ( btn->rect.width / 2 - textSize.x / 2) ;
    int textY = btn->rect.y  + (btn->rect.height / 2 - textSize.y / 2) ;
    Vector2 textPos = (Vector2){textX, textY}; 
    Color hoverColor = __DarkenColor(btn->style.bgColor, 0.2);

    if(btn->isHover){
        DrawRectangleRec(btn->rect, hoverColor);
    
    }else {
        DrawRectangleRec(btn->rect, btn->style.bgColor);
    }
    DrawTextEx( btn->style.font, btn->text, textPos, btn->style.fontSize,2, btn->style.textColor);
}

InputTextStyle CreateInputTextStyle(Color bgColor, Color textColor, Font font, int fontSize ){
    InputTextStyle style = (InputTextStyle) {
        .bgColor = bgColor,
        .textColor = textColor,
        .fontSize = fontSize,
        .disabled = false,
        .font = font,
    };

    return style;
}

void SetInputTextStylePlaceholder(InputTextStyle *style, char placeholder[20]) {
    strncpy(style->placeholder, placeholder, 20);
}

void CreateInputText(InputText *inputText, Rectangle rec, char inputValueContainer[255], InputTextStyle style){
    inputText->rect = rec;
    inputText->value = &(inputValueContainer[0]);
    inputText->isFocus = false;
    inputText->valueLen = 0;
    inputText->style = style;
}

void UpdateInputText(InputText *inputText){
    Vector2 mousePos = GetMousePosition();
    if(inputText->style.disabled){
        return; 
    }

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
    Vector2 textPos = {textXPos, textYPos};
    Color focusColor = __DarkenColor(this->style.bgColor, 0.5);

    if(this->isFocus){
        DrawRectangleRec(this->rect, focusColor);
        DrawRectangleLinesEx(this->rect, 2, BLACK);
    }else {
        DrawRectangleRec(this->rect, this->style.bgColor);
    }
    if(this->valueLen == 0){
        DrawTextEx(this->style.font ,this->style.placeholder, textPos, this->style.fontSize, 2, this->style.textColor);
    }else {
        DrawTextEx(this->style.font ,this->value, textPos, this->style.fontSize, 2, this->style.textColor);
    }
}


void __InputKeyToValue(InputText *text){
    // TODO: Hentikan saat value_len 255
    for(int c = 'A'; c <= 'Z'; c++) {
        if(IsKeyPressed(c)){
            // printf("ISINYA %s", text->value);
            text->valueLen += 1;
            text->value[text->valueLen - 1] = c;
        }
    }
    if(IsKeyPressed(KEY_BACKSPACE)){
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
