#include "navigation.h"
#include "raylib.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define WIDTH 700
#define HEIGHT 700
#define FPS 60

int main(int argc, char **argv) {

  if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
    printf("Usage:\n");
    return 1;
  }

  if (strcmp(argv[1], ".") == 0) {
    char dir[1024];
    getcwd(dir, sizeof(dir));
    printf("%s\n", dir);
    return *dir;
  }

  char filepath;
  strcpy(&filepath, argv[1]);
  printf("%s\n", &filepath);

  if (DirectoryExists(&filepath) == 1) {
    printf("Directory Not Exists\n");
  } else if (FileExists(&filepath) == 1) {
    printf("it is a file");
  }

  InitWindow(WIDTH, HEIGHT, "Image view");
  SetTargetFPS(FPS);

  Image img = LoadImage(&filepath);
  ImageResize(&img, 500, 500);

  ImageDrawPixel(&img, WIDTH / 2, HEIGHT / 2, WHITE);

  Texture2D tex = LoadTextureFromImage(img);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangle(0, 0, 700, 40, DARKGRAY);
    DrawTexture(tex, 100, 100, WHITE);

    // cursor : debugging
    // printf("%d\n", GetMouseX());
    // printf("%d\n", GetMouseY());

    DrawFPS(600, 50);

    Rectangle Openbutton = {10, 10, 50, 25};
    bool Open_hover = Navigation(Openbutton, "ImgView");

    Rectangle Exitbutton = {650, 10, 50, 25};
    bool Exit_hover = Navigation(Exitbutton, "Exit");

    if (Open_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      printf("WOw\n");
    } else if (Exit_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      exit(0);
    }

    if (IsKeyDown(KEY_Q)) {
      exit(0);
    }

    EndDrawing();
  }
  return 0;
}
