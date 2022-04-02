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
  float angle;
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

void DrawFrustumRotated(Frustum frustum, Color color) {
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

  Vector2 Copy = {TopLeft.x - frustum.x, TopLeft.y - frustum.y};
  TopLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
  TopLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);
  TopLeft = {TopLeft.x + frustum.x, TopLeft.y + frustum.y};

  //Copy = TopRight;
  Copy = {TopRight.x - frustum.x, TopRight.y - frustum.y};
  TopRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
  TopRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);
  TopRight = {TopRight.x + frustum.x, TopRight.y + frustum.y};

  //Copy = BottomRight;
  Copy = {BottomRight.x - frustum.x, BottomRight.y - frustum.y};
  BottomRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
  BottomRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);
  BottomRight = {BottomRight.x + frustum.x, BottomRight.y + frustum.y};

  //Copy = BottomLeft;
  Copy = {BottomLeft.x - frustum.x, BottomLeft.y - frustum.y};
  BottomLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
  BottomLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);
  BottomLeft = {BottomLeft.x + frustum.x, BottomLeft.y + frustum.y};

  DrawLineV(TopLeft, TopRight, color);
  DrawLineV(TopRight, BottomRight, color);
  DrawLineV(BottomRight, BottomLeft, color);
  DrawLineV(BottomLeft, TopLeft, color);
}

float FrustumXToScale(Frustum frustum, float x) {
  return abs(frustum.maxz / x);
}

//Vector2 FrustumXYToScale(Frustum frustum, Vector2 pos) {
//  return Vector2{
//    abs((frustum.maxz * cosf(frustum.angle)) / pos.x),
//    abs((frustum.maxz * sinf(frustum.angle)) / pos.y)
//  };
//}

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
    .angle = 0,
  };

  //Frustum shadow = {
  //  .fov  = 0,
  //  .minz = 0,
  //  .maxz = 200,
  //  .x = -100,
  //  .y = -100,
  //};

  RectangleOutline shadow = {
    { 200, 200 },
    { 200, 200 },
    { 200, 200 },
    { 200, 200 },
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

    if(IsKeyDown(KEY_E)) {
      frustum.angle += 0.04f;
    }

    if(IsKeyDown(KEY_Q)) {
      frustum.angle -= 0.04f;
    }

    {
      Vector2 Copy = {100, 100};
      shadow.BottomRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      shadow.BottomRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = {-100, 100};
      shadow.BottomLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      shadow.BottomLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = {100, -100};
      shadow.TopRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      shadow.TopRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = {-100, -100};
      shadow.TopLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      shadow.TopLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      shadow.BottomRight.x += frustum.x - cosf(frustum.angle) * 100.0f;
      shadow.BottomRight.y += frustum.y - sinf(frustum.angle) * 100.0f;
      shadow.BottomLeft.x += frustum.x - cosf(frustum.angle) * 100.0f;
      shadow.BottomLeft.y += frustum.y - sinf(frustum.angle) * 100.0f;
      shadow.TopRight.x += frustum.x - cosf(frustum.angle) * 100.0f;
      shadow.TopRight.y += frustum.y - sinf(frustum.angle) * 100.0f;
      shadow.TopLeft.x += frustum.x - cosf(frustum.angle) * 100.0f;
      shadow.TopLeft.y += frustum.y - sinf(frustum.angle) * 100.0f;
    }

    //shadow.x = frustum.x - 200;
    //shadow.y = frustum.y - 100;

    camera.target.x = frustum.x;
    camera.target.y = frustum.y;

    RectangleOutline scaled_rects[20];
    for(int i = 0; i < 20; i += 1) {
      Vector2 rect_dim = {rects[i].width, rects[i].height};
      Vector2 rect_pos = {rects[i].x, rects[i].y};

      // Find the four corners of the rectangle
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

      // unrotate
      Vector2 Copy = scaled_rects[i].BottomRight;
      scaled_rects[i].BottomRight.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      scaled_rects[i].BottomRight.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      Copy = scaled_rects[i].BottomLeft;
      scaled_rects[i].BottomLeft.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      scaled_rects[i].BottomLeft.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      Copy = scaled_rects[i].TopRight;
      scaled_rects[i].TopRight.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      scaled_rects[i].TopRight.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      Copy = scaled_rects[i].TopLeft;
      scaled_rects[i].TopLeft.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      scaled_rects[i].TopLeft.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      // scale
      scaled_rects[i].BottomRight.y *= FrustumXToScale(frustum, scaled_rects[i].BottomRight.x);
      scaled_rects[i].BottomLeft.y *= FrustumXToScale(frustum, scaled_rects[i].BottomLeft.x);
      scaled_rects[i].TopRight.y *= FrustumXToScale(frustum, scaled_rects[i].TopRight.x);
      scaled_rects[i].TopLeft.y *= FrustumXToScale(frustum, scaled_rects[i].TopLeft.x);

      // rerotate
      Copy = scaled_rects[i].BottomRight;
      scaled_rects[i].BottomRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      scaled_rects[i].BottomRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = scaled_rects[i].BottomLeft;
      scaled_rects[i].BottomLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      scaled_rects[i].BottomLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = scaled_rects[i].TopRight;
      scaled_rects[i].TopRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      scaled_rects[i].TopRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = scaled_rects[i].TopLeft;
      scaled_rects[i].TopLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      scaled_rects[i].TopLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      // retranslate
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

      // unrotate
      Vector2 Copy = unscaled_rects[i].BottomRight;
      unscaled_rects[i].BottomRight.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      unscaled_rects[i].BottomRight.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      Copy = unscaled_rects[i].BottomLeft;
      unscaled_rects[i].BottomLeft.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      unscaled_rects[i].BottomLeft.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      Copy = unscaled_rects[i].TopRight;
      unscaled_rects[i].TopRight.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      unscaled_rects[i].TopRight.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      Copy = unscaled_rects[i].TopLeft;
      unscaled_rects[i].TopLeft.x = Copy.x*cosf(-frustum.angle) - Copy.y*sinf(-frustum.angle);
      unscaled_rects[i].TopLeft.y = Copy.y*cosf(-frustum.angle) + Copy.x*sinf(-frustum.angle);

      // scale
      unscaled_rects[i].BottomRight.y /= FrustumXToScale(frustum, unscaled_rects[i].BottomRight.x);
      unscaled_rects[i].BottomLeft.y /= FrustumXToScale(frustum, unscaled_rects[i].BottomLeft.x);
      unscaled_rects[i].TopRight.y /= FrustumXToScale(frustum, unscaled_rects[i].TopRight.x);
      unscaled_rects[i].TopLeft.y /= FrustumXToScale(frustum, unscaled_rects[i].TopLeft.x);

      // rerotate
      Copy = unscaled_rects[i].BottomRight;
      unscaled_rects[i].BottomRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      unscaled_rects[i].BottomRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = unscaled_rects[i].BottomLeft;
      unscaled_rects[i].BottomLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      unscaled_rects[i].BottomLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = unscaled_rects[i].TopRight;
      unscaled_rects[i].TopRight.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      unscaled_rects[i].TopRight.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

      Copy = unscaled_rects[i].TopLeft;
      unscaled_rects[i].TopLeft.x = Copy.x*cosf(frustum.angle) - Copy.y*sinf(frustum.angle);
      unscaled_rects[i].TopLeft.y = Copy.y*cosf(frustum.angle) + Copy.x*sinf(frustum.angle);

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
        DrawFrustumRotated(frustum, GREEN);
        DrawFrustumRotated(frustum, GREEN);
        DrawRectangleOutline(shadow, RED);
        //DrawRectangleLinesEx(shadow, 1.0f, RED);
      EndMode2D();

      //char text[256];
      //Vector2 p = {40, 50};
      //Vector2 s = FrustumXYToScale(frustum, p);
      //sprintf(text, "angle: %f, scale: (%f, %f) for (%f, %f)", frustum.angle, s.x, s.y, p.x, p.y);

      //Vector2 e = {p.x - frustum.x, p.y - frustum.y};
      //e = {e.x * s.x, e.y * s.y};
      //e = {e.x + frustum.x, e.y + frustum.y};
      //e = {e.x - frustum.x, e.y - frustum.y};
      //e = {e.x / s.x, e.y / s.y};
      //e = {e.x + frustum.x, e.y + frustum.y};
      //printf("(%f, %f)\n", e.x, e.y);

      //DrawText(text, 20, 20, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
