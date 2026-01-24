
#include "navigation.h"
#include "raylib.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 60
#define MAX_PATH_LENGTH 1024
#define MAX_IMAGES 1024

// Helper functions
bool isImageFile(const char *filename);
bool isDirectory(const char *path);

int main(int argc, char **argv) {
  if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
    printf("Usage: %s <file_or_directory_path>\n", argv[0]);
    return 1;
  }

  char filepath[MAX_PATH_LENGTH];
  if (strcmp(argv[1], ".") == 0) {
    if (!getcwd(filepath, sizeof(filepath))) {
      perror("getcwd failed");
      return 1;
    }
  } else {
    strncpy(filepath, argv[1], MAX_PATH_LENGTH - 1);
    filepath[MAX_PATH_LENGTH - 1] = '\0';
  }

  // Validate path
  if (!isDirectory(filepath) && !isImageFile(filepath)) {
    printf("Enter a valid path or image.\n");
    return 1;
  }

  char imagePaths[MAX_IMAGES][MAX_PATH_LENGTH];
  int imageCount = 0;

  if (isDirectory(filepath)) {
    DIR *dir = opendir(filepath);
    if (!dir) {
      perror("Unable to open directory");
      return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

      char fullpath[MAX_PATH_LENGTH];
      snprintf(fullpath, sizeof(fullpath), "%s/%s", filepath, entry->d_name);

      if (isImageFile(fullpath)) {
        strncpy(imagePaths[imageCount], fullpath, MAX_PATH_LENGTH - 1);
        imagePaths[imageCount][MAX_PATH_LENGTH - 1] = '\0';
        imageCount++;
        if (imageCount >= MAX_IMAGES)
          break; // Avoid overflow
      }
    }
    closedir(dir);

    if (imageCount == 0) {
      printf("No images found in directory.\n");
      return 0;
    }
  } else {
    strncpy(imagePaths[0], filepath, MAX_PATH_LENGTH - 1);
    imagePaths[0][MAX_PATH_LENGTH - 1] = '\0';
    imageCount = 1;
  }

  // Initialize Raylib
  InitWindow(WIDTH, HEIGHT, "Image Viewer");
  SetTargetFPS(FPS);

  int currentIndex = 0;
  Image img = LoadImage(imagePaths[currentIndex]);
  if (!img.data) {
    printf("Failed to load image: %s\n", imagePaths[currentIndex]);
    CloseWindow();
    return 1;
  }
  ImageResize(&img, 700, 500);
  Texture2D tex = LoadTextureFromImage(img);
  UnloadImage(img);

  // Main loop
  while (!WindowShouldClose()) {
    // Navigate images
    if (IsKeyPressed(KEY_RIGHT)) {
      UnloadTexture(tex);
      currentIndex = (currentIndex + 1) % imageCount;
      img = LoadImage(imagePaths[currentIndex]);
      ImageResize(&img, 700, 500);
      tex = LoadTextureFromImage(img);
      UnloadImage(img);
    }
    if (IsKeyPressed(KEY_LEFT)) {
      UnloadTexture(tex);
      currentIndex = (currentIndex - 1 + imageCount) % imageCount;
      img = LoadImage(imagePaths[currentIndex]);
      ImageResize(&img, 700, 500);
      tex = LoadTextureFromImage(img);
      UnloadImage(img);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangle(0, 0, WIDTH, 40, DARKGRAY);

    DrawTexture(tex, 50, 50, WHITE);

    DrawFPS(WIDTH - 100, 10);
    DrawText(imagePaths[currentIndex], 50, 10, 20, WHITE);

    // Exit button
    Rectangle exitButton = {WIDTH - 60, 10, 50, 25};
    bool exitHover = Navigation(exitButton, "Exit");
    if ((exitHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ||
        IsKeyDown(KEY_Q))
      break;

    EndDrawing();
  }

  // Cleanup
  UnloadTexture(tex);
  CloseWindow();
  return 0;
}

// Check if file is an image based on extension
bool isImageFile(const char *filename) {
  const char *ext = strrchr(filename, '.');
  if (!ext)
    return false;
  if (strcasecmp(ext, ".png") == 0 || strcasecmp(ext, ".jpg") == 0 ||
      strcasecmp(ext, ".jpeg") == 0 || strcasecmp(ext, ".bmp") == 0 ||
      strcasecmp(ext, ".gif") == 0) {
    return true;
  }
  return false;
}

bool isDirectory(const char *path) {
  DIR *dir = opendir(path);
  if (dir) {
    closedir(dir);
    return true;
  }
  return false;
}
