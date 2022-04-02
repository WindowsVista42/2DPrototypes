#include <raylib.h>

int main() {
  const int screen_width = 800, screen_height = 450;

  InitWindow(screen_width, screen_height, "2DShadowTransform");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("2DShadowTransform", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
