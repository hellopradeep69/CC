#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#define WIDTH 700
#define HEIGHT 700
#define BLOCK_SIZE 200
#define BLOCK_SIZE2 100
#define FPS 60

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Lost");
  SetTargetFPS(FPS);

  Rectangle rect = {WIDTH / 2, HEIGHT / 2, BLOCK_SIZE2, BLOCK_SIZE2};
  Vector2 origin = {BLOCK_SIZE2 / 2, BLOCK_SIZE2 / 2};

  Rectangle rect2 = {WIDTH / 2, HEIGHT / 2, BLOCK_SIZE, BLOCK_SIZE};
  Vector2 origin2 = {BLOCK_SIZE / 2, BLOCK_SIZE / 2};

  float angle = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKBROWN);

    DrawText("Mr.Real", 110, 100, 29, RED);

    DrawRectangle(WIDTH / 2 - BLOCK_SIZE / 2, HEIGHT / 2 - BLOCK_SIZE / 2,
                  BLOCK_SIZE, BLOCK_SIZE, DARKBROWN);

    // rotating rect
    DrawRectanglePro(rect2, origin2, angle, GREEN);
    DrawRectanglePro(rect, origin, angle + 10, YELLOW);

    angle += 1.50;

    DrawFPS(50, 50);

    if (IsKeyDown(KEY_Q)) {
      exit(0);
    }

    EndDrawing();
  }
  return 0;
}
