#include "raylib.h"
#include <stdlib.h>

int main(void) {
  InitWindow(700, 700, "Lost");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RED);
    DrawText("hello", 110, 100, 29, YELLOW);

    if (IsKeyDown(KEY_Q)) {
      exit(0);
    }

    EndDrawing();
  }
  return 0;
}
