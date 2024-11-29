#ifndef CONTROL_H
#define CONTROL_H

#include "raylib.h"

typedef struct{
  Rectangle rect;
  char text[20];
  bool isHover;
} Button;

typedef struct{
  Rectangle rect;
  char Value[255];
  bool isFocus;
  int value_len;
} InputText;


#endif // !CONTROL_H
