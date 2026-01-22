#include "navigation.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define WIDTH 700
#define HEIGHT 700
#define BLOCK_SIZE 200
#define BLOCK_SIZE2 100
#define FPS 60

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Image view");
  SetTargetFPS(FPS);

  Rectangle rect = {WIDTH / 2.0, HEIGHT / 2.0, BLOCK_SIZE2, BLOCK_SIZE2};
  Vector2 origin = {BLOCK_SIZE2 / 2.0, BLOCK_SIZE2 / 2.0};

  Rectangle rect2 = {WIDTH / 2.0, HEIGHT / 2.0, BLOCK_SIZE, BLOCK_SIZE};
  Vector2 origin2 = {BLOCK_SIZE / 2.0, BLOCK_SIZE / 2.0};

  float angle = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawText("Mr.Real", 110, 100, 29, RED);

    DrawRectangle(WIDTH / 2 - BLOCK_SIZE / 2, HEIGHT / 2 - BLOCK_SIZE / 2,
                  BLOCK_SIZE, BLOCK_SIZE, DARKBROWN);

    DrawRectangle(0, 0, 700, 40, BLACK);

    // rotating rect
    DrawRectanglePro(rect2, origin2, angle, GREEN);
    DrawRectanglePro(rect, origin, angle + 10, YELLOW);

    // cursor : debugging
    printf("%d\n", GetMouseX());
    printf("%d\n", GetMouseY());

    angle += 1.50;

    DrawFPS(600, 50);

    Rectangle Openbutton = {10, 10, 50, 25};
    bool Open_hover = Navigation(Openbutton, "Open ");

    Rectangle Exitbutton = {650, 10, 50, 25};
    bool Exit_hover = Navigation(Exitbutton, "Exit");

    if (Open_hover && IsMouseButtonPressed(10)) {
      printf("WOw\n");
    } else if (Exit_hover && IsMouseButtonPressed(10)) {
      exit(0);
    }

    if (IsKeyDown(KEY_Q)) {
      exit(0);
    }

    EndDrawing();
  }
  return 0;
}
