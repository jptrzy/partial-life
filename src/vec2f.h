#pragma once

#include <math.h>

// Vector 2D float
typedef struct {
  float x;
  float y;
} Vec2f;

Vec2f vec2f_add(Vec2f a, Vec2f b) {
  return (Vec2f) {.x = a.x + b.x, .y = a.y + b.y};
}

Vec2f vec2f_sub(Vec2f a, Vec2f b) {
  return (Vec2f) {.x = a.x - b.x, .y = a.y - b.y};
}

Vec2f vec2f_scale(Vec2f a, float val) {
  return (Vec2f) {.x = a.x * val, .y = a.y * val};
}

float vec2f_len(Vec2f a) {
  // TODO power of 2 hack
  return sqrtf(powf(a.x, 2) + powf(a.y, 2));
}

// Normalise a vector
Vec2f vec2f_norm(Vec2f a){
  return vec2f_scale(a, 1/vec2f_len(a));
}

static float lerp(float a, float b, float t) {
  return a * (1 - t) + b * t;
}

Vec2f vec2f_lerp(Vec2f a, Vec2f b, float t) {
  return (Vec2f) {.x = lerp(a.x, b.x, t), .y = lerp(a.y, b.y, t)};
}
