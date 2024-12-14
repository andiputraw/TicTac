/**
 * control.h
 * Author : Andi Putra Wijaya & Maulana Ishak
 * Created: 30 November 2024
 * 
 * Header file untuk control.c. Yang memiliki definisi untuk pembuatan Button dan InputText sebagai ui control. 
 */
#ifndef CONTROL_H
#define CONTROL_H

#include "raylib.h"

#define NO_RECT (Rectangle){0,0,0,0}

typedef struct {
  Color bgColor;
  Color textColor;
  int fontSize;
  bool disabled;
  Font font;
} ButtonStyle;

typedef struct{
  Rectangle rect;
  char text[20];
  bool isHover;
  bool isClicked;
  ButtonStyle style;
} Button;

typedef struct {
  Color bgColor;
  Color textColor;
  int fontSize;
  bool disabled;
  char placeholder[20];
  Font font;
} InputTextStyle;

typedef struct{
  Rectangle rect;
  char *value;
  bool isFocus;
  int valueLen;
  InputTextStyle style;
} InputText;

ButtonStyle CreateButtonStyle(Color bgColor, Color textColor,Font font, int fontSize);
void SetInputTextStylePlaceholder(InputTextStyle *style, char placeholder[20]);
void CreateButton(Button *btn, Rectangle rec, char text[20], ButtonStyle style);
void UpdateButton(Button *this);
void DrawButton(Button *this);

InputTextStyle CreateInputTextStyle(Color bgColor, Color textColor, Font font, int fontSize );
void CreateInputText(InputText *this, Rectangle rec, char inputValueContainer[255], InputTextStyle style);
void UpdateInputText(InputText *this);
void DrawInputText(InputText *this);

// Private Function:
// Cek input dari keyboard, lalu masukan ke value
void __InputKeyToValue(InputText *text);
// Membuat color yang diberikan lebih gelap
Color __DarkenColor(Color color, float factor);

#endif // CONTROL_H
