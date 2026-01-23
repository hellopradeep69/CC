
#include "raylib.h"
#include <dirent.h>
#include <math.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_IMAGES 1024
#define MAX_PATH_LEN 1024

// Simple image extension check
bool IsImageFile(const char *path) {
  const char *ext = GetFileExtension(path);
  if (ext == NULL)
    return false;

  return TextIsEqual(ext, ".png") || TextIsEqual(ext, ".jpg") ||
         TextIsEqual(ext, ".jpeg") || TextIsEqual(ext, ".bmp") ||
         TextIsEqual(ext, ".tga") || TextIsEqual(ext, ".gif") ||
         TextIsEqual(ext, ".qoi");
}

Camera2D camera = {0};
bool flipHorizontal = false;
bool flipVertical = false;
bool info_mode = false;
bool antialiasing = true; // True == Bilinear, False == Nearest Neighbour

void reset_view(Texture2D texture) {
  camera.zoom = fminf((float)GetScreenWidth() / texture.width,
                      (float)GetScreenHeight() / texture.height);
  camera.target = (Vector2){0.0f, 0.0f};
  camera.rotation = 0.0f;
  flipHorizontal = false;
  flipVertical = false;
  antialiasing = true;
  SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
}

// Code from /u/notLinode on reddit
void DrawOutlinedText(const char *text, int posX, int posY, int fontSize,
                      Color color, int outlineSize, Color outlineColor) {
  DrawText(text, posX - outlineSize, posY - outlineSize, fontSize,
           outlineColor);
  DrawText(text, posX + outlineSize, posY - outlineSize, fontSize,
           outlineColor);
  DrawText(text, posX - outlineSize, posY + outlineSize, fontSize,
           outlineColor);
  DrawText(text, posX + outlineSize, posY + outlineSize, fontSize,
           outlineColor);
  DrawText(text, posX, posY, fontSize, color);
}
int main(int argc, char **argv) {
  if (argc < 2) {
    printf("No file or directory specified\n");
    printf("Usage: iv <file-or-directory>\n");
    return 1;
  }

  if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
    printf("r-image-viewer\n"
           "A simple, fast, minimalist image viewer built with C and the "
           "raylib library.\n\n"
           "Usage:\n"
           "  ./iv path/to/your/image.png\n"
           "  ./iv path/to/your/directory/\n\n"
           "Controls:\n"
           "  Right Arrow: Next image in directory\n"
           "  Left Arrow:  Previous image in directory\n"
           "  Mouse Wheel: Zoom in/out at cursor\n"
           "  Ctrl + + / -:Zoom in/out at center\n"
           "  Mouse Left:  Pan image\n"
           "  W, A, S, D:  Pan image\n"
           "  Q, E:        Rotate image\n"
           "  R:           Reset rotation\n"
           "  H:           Flip horizontally\n"
           "  V:           Flip vertically\n"
           "  F:           Reset all view changes\n"
           "  I:           Toggle info display\n"
           "  Ctrl + A:    Toggle anti-aliasing\n"
           "  Esc:         Quit\n");
    return 0;
  }

  // --------------------------------------------------
  // Collect image paths
  // --------------------------------------------------
  char *images[MAX_IMAGES];
  int imageCount = 0;

  struct stat pathStat;
  stat(argv[1], &pathStat);

  if (S_ISDIR(pathStat.st_mode)) {
    DIR *dir = opendir(argv[1]);
    if (!dir) {
      perror("opendir");
      return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) && imageCount < MAX_IMAGES) {
      if (entry->d_type == DT_REG) {
        char fullPath[MAX_PATH_LEN];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", argv[1], entry->d_name);

        if (IsImageFile(fullPath)) {
          images[imageCount] = strdup(fullPath);
          imageCount++;
        }
      }
    }

    closedir(dir);
  } else {
    if (!IsImageFile(argv[1])) {
      printf("Not a supported image file\n");
      return 1;
    }

    images[0] = strdup(argv[1]);
    imageCount = 1;
  }

  if (imageCount == 0) {
    printf("No images found\n");
    return 1;
  }

  // --------------------------------------------------
  // Raylib setup
  // --------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "iv - image viewer");
  SetTargetFPS(60);

  int current = 0;
  Texture2D texture = LoadTexture(images[current]);

  reset_view(texture);
  camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

  antialiasing = true;
  SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

  // --------------------------------------------------
  // Main loop
  // --------------------------------------------------
  while (!WindowShouldClose()) {
    // --------------------------------------------------
    // Update
    // --------------------------------------------------
    if (IsWindowResized()) {
      camera.offset =
          (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
      reset_view(texture);
    }

    // Navigation
    if (IsKeyPressed(KEY_RIGHT)) {
      current = (current + 1) % imageCount;
      UnloadTexture(texture);
      texture = LoadTexture(images[current]);
      reset_view(texture);
    }

    if (IsKeyPressed(KEY_LEFT)) {
      current = (current - 1 + imageCount) % imageCount;
      UnloadTexture(texture);
      texture = LoadTexture(images[current]);
      reset_view(texture);
    }

    // Zoom
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      // Apply zoom exponentially
      float zoomAmount = wheel * 0.05f;
      camera.zoom *= (1.0f + zoomAmount);

      Vector2 mouseWorldPosAfterZoom =
          GetScreenToWorld2D(GetMousePosition(), camera);

      // Adjust camera target to keep mouse position stable
      camera.target =
          Vector2Add(camera.target,
                     Vector2Subtract(mouseWorldPos, mouseWorldPosAfterZoom));
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
      if (IsKeyDown(KEY_EQUAL))
        camera.zoom *= 1.1f;
      if (IsKeyDown(KEY_MINUS))
        camera.zoom *= 1.0f / 1.1f;
    }

    camera.zoom = Clamp(camera.zoom, 0.125f, 64.0f);

    // Panning
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);
    }

    if (IsKeyDown(KEY_W))
      camera.target.y -= 10.0f / camera.zoom;
    if (IsKeyDown(KEY_S))
      camera.target.y += 10.0f / camera.zoom;
    if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_LEFT_CONTROL))
      camera.target.x -= 10.0f / camera.zoom;
    if (IsKeyDown(KEY_D))
      camera.target.x += 10.0f / camera.zoom;

    // Rotation
    if (IsKeyDown(KEY_Q))
      camera.rotation--;
    if (IsKeyDown(KEY_E))
      camera.rotation++;
    if (IsKeyPressed(KEY_R))
      camera.rotation = 0.0f;

    // Flipping
    if (IsKeyPressed(KEY_H))
      flipHorizontal = !flipHorizontal;
    if (IsKeyPressed(KEY_V))
      flipVertical = !flipVertical;

    // Reset
    if (IsKeyPressed(KEY_F)) {
      reset_view(texture);
    }

    // Anti-aliasing
    if (IsKeyPressed(KEY_A) && IsKeyDown(KEY_LEFT_CONTROL)) {
      antialiasing = !antialiasing;
      if (antialiasing) {
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
      } else {
        SetTextureFilter(texture, TEXTURE_FILTER_POINT);
      }
    }

    // Showing File info
    if (IsKeyPressed(KEY_I))
      info_mode = !info_mode;

    // --------------------------------------------------
    // Draw
    // --------------------------------------------------
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera);

    Rectangle sourceRec = {0.0f, 0.0f, (float)texture.width,
                           (float)texture.height};
    if (flipHorizontal)
      sourceRec.width *= -1;
    if (flipVertical)
      sourceRec.height *= -1;

    Rectangle destRec = {-texture.width / 2.0f, -texture.height / 2.0f,
                         (float)texture.width, (float)texture.height};

    DrawTexturePro(texture, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);

    EndMode2D();

    if (info_mode == true) {
      DrawRectangle(0, 0, GetScreenWidth(), 200,
                    (Color){0x44, 0x44, 0x44, 0x44});
      DrawText(TextFormat("%d / %d", current + 1, imageCount), 10, 10, 30,
               WHITE);
      DrawText(GetFileName(images[current]), 10, 35, 30, WHITE);

      struct stat st;
      stat(images[current], &st);
      float size_mb = (float)st.st_size / (1024 * 1024);

      DrawText(TextFormat("Resolution: %dx%d", texture.width, texture.height),
               10, 60, 30, WHITE);
      DrawText(TextFormat("Size: %.2fMB", size_mb), 10, 85, 30, WHITE);
      DrawText(TextFormat("Type: %s", GetFileExtension(images[current])), 10,
               110, 30, WHITE);
      DrawText(TextFormat("Zoom: %.2f%%", camera.zoom * 100), 10, 135, 30,
               WHITE);
      DrawText(TextFormat("Rotation: %.0f", camera.rotation), 10, 160, 30,
               WHITE);
    }
    EndDrawing();
  }

  // --------------------------------------------------
  // Cleanup
  // --------------------------------------------------
  UnloadTexture(texture);

  for (int i = 0; i < imageCount; i++)
    free(images[i]);

  CloseWindow();
  return 0;
}
