#include <raylib.h>
#include <raymath.h>
#include <iostream>

typedef unsigned char uchar;

uchar GetRandomUChar(uchar min, uchar max) {
  return GetRandomValue(min, max);
}

float GetRandomFloat(float min, float max) {
  return (float)GetRandomValue((int)min, (int)max);
}

struct RectangleOutline {
  Vector2 TopRight, BottomRight, BottomLeft, TopLeft;
};

void DrawRectangleOutline(RectangleOutline rect_outline, Color color) {
  DrawLineStrip((Vector2*)&rect_outline, 4, color);
  DrawLineV(rect_outline.TopLeft, rect_outline.TopRight, color); // draw last one
}

struct Frustum {
  float fov;
  float minz;
  float maxz;
  float x;
  float y;
};

void DrawFrustum(Frustum frustum, Color color) {
  float half_fov = frustum.fov / 2.0f;

  Vector2 TopLeft, BottomLeft;
  TopLeft.x = BottomLeft.x = frustum.x - frustum.maxz;
  BottomLeft.y = sinf(half_fov) * frustum.maxz;
  TopLeft.y = -BottomLeft.y + frustum.y;
  BottomLeft.y += frustum.y;

  Vector2 TopRight, BottomRight;
  TopRight.x = BottomRight.x = frustum.x - frustum.minz;
  BottomRight.y = sinf(half_fov) * frustum.minz;
  TopRight.y = -BottomRight.y + frustum.y;
  BottomRight.y += frustum.y;

  DrawLineV(TopLeft, TopRight, color);
  DrawLineV(TopRight, BottomRight, color);
  DrawLineV(BottomRight, BottomLeft, color);
  DrawLineV(BottomLeft, TopLeft, color);
}

float FrustumXToScale(Frustum frustum, float x) {
//  float minx = frustum.x - frustum.minz;
//  float maxx = frustum.x - frustum.maxz;
  float half_fov = frustum.fov / 2.0f;
  float h = sinf(half_fov) * x;// - (frustum.minz / 2.0f));
  float maxh = sinf(half_fov) * (frustum.maxz);// - (frustum.minz / 2.0f));

  return abs(maxh / h);
}

//float YFromXAndFrustum(Frustum frustum) {
//  float max_w = frustum.BottomLeft.y - frustum.TopLeft.y;
//  float min_w = frustum.BottomRight.y - frustum.TopRight.y;
//}

int main() {
  const int screen_width = 800, screen_height = 800;

  InitWindow(screen_width, screen_height, "2DShadowTransform");
  SetTargetFPS(60);

  //Rectangle p = {400, 280, 40, 40};
  Rectangle rects[20];
  Color colors[20];
  for(int i = 0; i < 20; i += 1) {
    rects[i] = Rectangle{ GetRandomFloat(-85,85), GetRandomFloat(-85,85), GetRandomFloat(5, 30), GetRandomFloat(5,30) };
    colors[i] = Color{ GetRandomUChar(140, 200), GetRandomUChar(140, 200), GetRandomUChar(140, 200), 255 };
  }

  Frustum frustum = {
    .fov = 3.14f / 3.0f,
    .minz = 10,
    .maxz = 200,
    .x = 100,
    .y = 0,
  };

  Rectangle shadow = {
    .x = -100,
    .y = -100,
    .width = 200,
    .height = 200,
  };

  Camera2D camera = {0};
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;

  //printf("%f\n", YFromXAndFrustum(frustum));

  while(!WindowShouldClose()) {
    if(IsKeyDown(KEY_W)) {
      frustum.y -= 0.2f;
    }
    
    if(IsKeyDown(KEY_S)) {
      frustum.y += 0.2f;
    }

    if(IsKeyDown(KEY_A)) {
      frustum.x -= 0.2f;
    }
    
    if(IsKeyDown(KEY_D)) {
      frustum.x += 0.2f;
    }

    shadow.x = frustum.x - 200;
    shadow.y = frustum.y - 100;

    RectangleOutline scaled_rects[20];
    for(int i = 0; i < 20; i += 1) {
      Vector2 rect_dim = {rects[i].width, rects[i].height};
      Vector2 rect_pos = {rects[i].x, rects[i].y};

      scaled_rects[i].BottomRight = Vector2Add(rect_pos, rect_dim);

      scaled_rects[i].BottomLeft.x = scaled_rects[i].BottomRight.x - rect_dim.x;
      scaled_rects[i].BottomLeft.y = scaled_rects[i].BottomRight.y;


      scaled_rects[i].TopRight.x = scaled_rects[i].BottomRight.x;
      scaled_rects[i].TopRight.y = scaled_rects[i].BottomRight.y - rect_dim.y;

      scaled_rects[i].TopLeft = Vector2Subtract(scaled_rects[i].BottomRight, rect_dim);

      scaled_rects[i].BottomRight = Vector2Subtract(scaled_rects[i].BottomRight, {frustum.x, frustum.y});
      scaled_rects[i].BottomLeft = Vector2Subtract(scaled_rects[i].BottomLeft, {frustum.x, frustum.y});
      scaled_rects[i].TopRight = Vector2Subtract(scaled_rects[i].TopRight, {frustum.x, frustum.y});
      scaled_rects[i].TopLeft = Vector2Subtract(scaled_rects[i].TopLeft, {frustum.x, frustum.y});

      scaled_rects[i].BottomRight.y *= FrustumXToScale(frustum, scaled_rects[i].BottomRight.x);
      scaled_rects[i].BottomLeft.y *= FrustumXToScale(frustum, scaled_rects[i].BottomLeft.x);
      scaled_rects[i].TopRight.y *= FrustumXToScale(frustum, scaled_rects[i].TopRight.x);
      scaled_rects[i].TopLeft.y *= FrustumXToScale(frustum, scaled_rects[i].TopLeft.x);

      scaled_rects[i].BottomRight = Vector2Add(scaled_rects[i].BottomRight, {frustum.x, frustum.y});
      scaled_rects[i].BottomLeft = Vector2Add(scaled_rects[i].BottomLeft, {frustum.x, frustum.y});
      scaled_rects[i].TopRight = Vector2Add(scaled_rects[i].TopRight, {frustum.x, frustum.y});
      scaled_rects[i].TopLeft = Vector2Add(scaled_rects[i].TopLeft, {frustum.x, frustum.y});
    }

    RectangleOutline unscaled_rects[20];
    for(int i = 0; i < 20; i += 1) {
      Vector2 rect_dim = {rects[i].width, rects[i].height};
      Vector2 rect_pos = {rects[i].x, rects[i].y};

      unscaled_rects[i] = scaled_rects[i];

      unscaled_rects[i].BottomRight = Vector2Subtract(unscaled_rects[i].BottomRight, {frustum.x, frustum.y});
      unscaled_rects[i].BottomLeft = Vector2Subtract(unscaled_rects[i].BottomLeft, {frustum.x, frustum.y});
      unscaled_rects[i].TopRight = Vector2Subtract(unscaled_rects[i].TopRight, {frustum.x, frustum.y});
      unscaled_rects[i].TopLeft = Vector2Subtract(unscaled_rects[i].TopLeft, {frustum.x, frustum.y});

      unscaled_rects[i].BottomRight.y /= FrustumXToScale(frustum, unscaled_rects[i].BottomRight.x);
      unscaled_rects[i].BottomLeft.y /= FrustumXToScale(frustum, unscaled_rects[i].BottomLeft.x);
      unscaled_rects[i].TopRight.y /= FrustumXToScale(frustum, unscaled_rects[i].TopRight.x);
      unscaled_rects[i].TopLeft.y /= FrustumXToScale(frustum, unscaled_rects[i].TopLeft.x);

      unscaled_rects[i].BottomRight = Vector2Add(unscaled_rects[i].BottomRight, {frustum.x, frustum.y});
      unscaled_rects[i].BottomLeft = Vector2Add(unscaled_rects[i].BottomLeft, {frustum.x, frustum.y});
      unscaled_rects[i].TopRight = Vector2Add(unscaled_rects[i].TopRight, {frustum.x, frustum.y});
      unscaled_rects[i].TopLeft = Vector2Add(unscaled_rects[i].TopLeft, {frustum.x, frustum.y});
    }

    BeginDrawing();
      ClearBackground(RAYWHITE);
      BeginMode2D(camera);
        for(int i = 0; i < 20; i += 1) {
          DrawRectangleRec(rects[i], colors[i]);
        }

        for(int i = 0; i < 20; i += 1) {
          DrawRectangleOutline(scaled_rects[i], ORANGE);
        }

        for(int i = 0; i < 20; i += 1) {
          DrawRectangleOutline(unscaled_rects[i], SKYBLUE);
        }

        DrawCircleLines(frustum.x, frustum.y, 5.0f, BLUE);
        DrawFrustum(frustum, GREEN);
        DrawRectangleLinesEx(shadow, 1.0f, RED);
      EndMode2D();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
