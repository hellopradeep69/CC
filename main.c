#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 700
#define HEIGHT 700
#define BLOCK_SIZE 150

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Lost");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Mr.Real", 110, 100, 29, RED);

    DrawRectangle(WIDTH / 2, HEIGHT / 2, BLOCK_SIZE, BLOCK_SIZE, GREEN);

    if (IsKeyDown(KEY_Q)) {
      exit(0);
    }

    EndDrawing();
  }
  return 0;
}
