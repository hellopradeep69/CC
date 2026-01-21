#include "shader.h"
#include <raylib.h>
#include <stdio.h>
void name(void) { printf("hello"); }

void Triangle(void) {
  DrawTriangle((Vector2){500, 500}, (Vector2){700, 700}, (Vector2){900, 500},
               PINK);
}
