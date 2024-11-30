#ifndef CONTROL_H
#define CONTROL_H

#include "raylib.h"

typedef struct{
  Rectangle rect;
  char text[20];
  int fontSize;
  bool isHover;
  bool isClicked;
  Color color;
  Color textColor;
} Button;

typedef struct{
  Rectangle rect;
  char *value;
  bool isFocus;
  int valueLen;
} InputText;

void CreateButton(Button *btn, Rectangle rec, char text[20], Color color, Color textColor, int fontSize);
void UpdateButton(Button *this);
void DrawButton(Button *this);

void CreateInputText(InputText *this, Rectangle rec, char inputValueContainer[255]);
void UpdateInputText(InputText *this);
void DrawInputText(InputText *this);

// Private Function: Cek input dari keyboard, lalu masukan ke value
void __InputKeyToValue(InputText *text);
// Membuat color yang diberikan lebih gelap
Color __DarkenColor(Color color, float factor);

#endif // !CONTROL_H
