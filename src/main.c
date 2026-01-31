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
#define MAX 1024
#define MAX_Path 1024

bool IsExtension(const char *file) {
  bool ext = IsFileExtension(file, ".png") || IsFileExtension(file, ".jpg");
  // printf("%d\n", ext);
  return ext;
}

int main(int argc, char **argv) {

  if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
    printf("Usage:\n");
    return 1;
  }

  char filepath[MAX];

  if (strcmp(argv[1], ".") == 0) {
    char dir_temp[1024];
    getcwd(dir_temp, sizeof(dir_temp));
    strcpy(filepath, dir_temp);
    printf("%s\n", filepath);
  } else {
    strcpy(filepath, argv[1]);
    printf("%s\n", filepath);
  }

  if (DirectoryExists(filepath) == 0 && FileExists(filepath) == 0) {
    printf("Enter a Valid Path\n");
    return 1;
  }

  char images[MAX];
  char imageList[MAX][MAX_Path];
  int imageCount = 0;

  if (DirectoryExists(filepath)) {
    DIR *dir;
    struct dirent *entry;
    printf("this is path %s\n", filepath);

    dir = opendir(filepath);
    if (dir == NULL) {
      perror("unable to open the dir");
      return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_type == DT_REG && IsExtension(entry->d_name)) {
        snprintf(images, sizeof(images), "%s%s", argv[1], entry->d_name);
        // break;

        if (IsExtension(images)) {
          strcpy(imageList[imageCount], images);
          // printf("%c\n", imageList[1]);
          imageCount++;
        }
      }
    }
    closedir(dir);
  } else {
    if (!IsExtension(argv[1])) {
      printf("Not a supported image file\n");
      return 1;
    }
    snprintf(images, MAX, "%s", filepath);
  }

  for (int i = 0; i <= imageCount; i++) {
    printf("%s\n", imageList[i]);
  }

  IsExtension(images);

  InitWindow(WIDTH, HEIGHT, "Image view");
  SetTargetFPS(FPS);

  Image img = LoadImage(images);
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

    // This is nav

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
  UnloadTexture(tex);
  return 0;
}
