#include "stdio.h"
#include "stdlib.h"
#include "raylib.h"

#include "vec2f.h"

#define KINDS_MAX 4
#define BALLS_MAX 1024

#define MIN_DIS 2.0f
#define MIN_DIS_FORCE 1.0f
#define MAX_DIS 100.0f
#define MAX_DIS_FORCE 10.0f

#define REL_FORCE_MAX 10.0f
#define REL_RANGE_MAX 10.0f

// Relations
typedef struct {
  float range;
  float force;
} Rel;

typedef struct {
  Vec2f pos;
  Vec2f vel;
} Ball;

typedef struct {
  Rel rels[KINDS_MAX][KINDS_MAX];
  Ball balls[KINDS_MAX][BALLS_MAX];
  int rels_len;
  int balls_len;
} State;

float frand() {
  return ((float) rand() / (float) RAND_MAX);
}

void state_update(State *state) {
  for (int k=0;k<state->rels_len;k++) {
    for (int b=0;b<state->balls_len;b++) {
      Ball *ball = &state->balls[k][b];

      for (int _k=0;_k<state->rels_len;_k++) {
        for (int _b=0;_b<state->balls_len;_b++) {
          if (_b == b && _k == k) continue;

          Ball *_ball = &state->balls[_k][_b];
          float dis = vec2f_len(vec2f_sub(ball->pos, _ball->pos));

          Rel rel = state->rels[k][_k];
          float force = 0;
          if (MIN_DIS > dis) {
            force = (MIN_DIS - dis) / MIN_DIS * MIN_DIS_FORCE;
          } else if (rel.range > dis) {
              
          }

          if (force == 0) continue;

          Vec2f dir = vec2f_sub(_ball->pos, ball->pos);
          if (dis == 0) {
            dis = 1e-6f;
            float angle = 6.28f * frand();
            dir.x = cosf(angle);
            dir.y = sinf(angle);
          }

          _ball->vel = vec2f_add(
              _ball->vel,
              vec2f_scale(
                dir,
                // TODO scale by delta
                1.0f*force/dis // trick to normalise a vector
              )
          );
        }
      }
    }
  }


  for (int k=0;k<state->rels_len;k++) {
    for (int b=0;b<state->balls_len;b++) {
      Ball *ball = &state->balls[k][b];


      float dis = vec2f_len(ball->pos);
      if (dis > MAX_DIS) {
        float force = (dis - MAX_DIS) / MAX_DIS * MAX_DIS_FORCE;

        ball->vel = vec2f_add(ball->vel, vec2f_scale(ball->pos, -force/dis));
      }

      // TODO scale by delta
      ball->pos = vec2f_add(ball->pos, vec2f_scale(ball->vel, 1.0f));

      ball->vel = vec2f_scale(ball->vel, 0.9f);
    }
  }
}

void state_draw(State *state) {
  DrawCircleLines(0, 0, MAX_DIS, ORANGE);
  
  for (int k=0;k<state->rels_len;k++) {
    for (int b=0;b<state->balls_len;b++) {
      Ball *ball = &state->balls[k][b];

      DrawCircleV(*(Vector2*)&(ball->pos), 2.0f, RED);
    }
  }
}

void state_rel_rand(State *state, float scale) {
  for (int k=0;k<state->rels_len;k++) {
    for (int _k=0;_k<state->rels_len;_k++) {
      state->rels[k][_k].force = frand() * REL_FORCE_MAX;
      state->rels[k][_k].range = frand() * REL_RANGE_MAX;
    }
  }
}

int main(void) {
  State state = {0};
  state.balls_len = 16;
  state.rels_len = 4;

  InitWindow(320, 320, "Partial Life");
  SetTargetFPS(60);

  //Camera2D camera = { 0 };
  //camera.target = (Vector2){ player.x + 20.0f, player.y + 20.0f };
  //camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
  //camera.rotation = 0.0f;
  //camera.zoom = 1.0f;

  while (!WindowShouldClose()) {
    state_update(&state);

    printf("%f\n", state.balls[0][0].pos.x);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      state_draw(&state);
    EndDrawing();
  }

  CloseWindow();
}
