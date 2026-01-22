#include "navigation.h"
#include "raylib.h"
#include <stdbool.h>

bool Navigation(Rectangle rec2, char *nav) {
  Vector2 v2 = GetMousePosition();

  DrawText(nav, (int)rec2.x, (int)rec2.y, 20, WHITE);

  bool hover = CheckCollisionPointRec(v2, rec2);

  if (hover) {
    DrawText(nav, (int)rec2.x, (int)rec2.y, 20, GRAY);
  }

  return hover;
}
