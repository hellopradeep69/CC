#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define WIDTH 700
#define HEIGHT 700
#define BLOCK_SIZE 200
#define FPS 60

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Lost");
  SetTargetFPS(FPS);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Mr.Real", 110, 100, 29, RED);

    DrawRectangle(WIDTH / 2 - BLOCK_SIZE / 2, HEIGHT / 2 - BLOCK_SIZE / 2,
                  BLOCK_SIZE, BLOCK_SIZE, GREEN);

    DrawFPS(50, 50);

    if (IsKeyDown(KEY_Q)) {
      exit(0);
    }

    EndDrawing();
  }
  return 0;
}
