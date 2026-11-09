#include "stdio.h"
#include "stdlib.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#define RINI_IMPLEMENTATION
#define RINI_MAX_TEXT_FILE_SIZE 4096*2
#include "rini.h"

#ifdef __EMSCRIPTEN__
  #include <emscripten/emscripten.h>
#endif

#include "pals.h"
#include "vec2f.h"

#define PAL_LEN 24
#define PAL_DEFAULT (pal_miami)

#define VERSION 1

#define KINDS_MAX 8
#define BALLS_MAX 1024

#define MIN_DIS 4.0f
#define MIN_DIS_FORCE 100.0f
#define MAX_DIS 100.0f
#define MAX_DIS_FORCE 100.0f

#define REL_FORCE_MAX 100.0f
#define REL_RANGE_MAX 20.0f

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)


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

  Color rand_pal[PAL_LEN];
  Color *pal;
} State;

float frand() {
  return ((float) rand() / (float) RAND_MAX);
}

void state_update(State *state, float dt) {
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
            force = rel.force;
          }

          if (force == 0) continue;

          Vec2f dir = vec2f_sub(_ball->pos, ball->pos);
          if (dis == 0) {
            dis = 1.0f;
            float angle = 6.28f * frand();
            dir.x = cosf(angle);
            dir.y = sinf(angle);
          }

          _ball->vel = vec2f_add(
              _ball->vel,
              vec2f_scale(
                dir,
                1.0f*dt*force/dis // trick to normalise a vector
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

      float force = 0.1f;
      ball->vel = vec2f_add(ball->vel, vec2f_scale(ball->pos, -force/dis));

      ball->pos = vec2f_add(ball->pos, vec2f_scale(ball->vel, dt));

      //ball->vel = vec2f_scale(ball->vel, dt*0.9f);
      ball->vel = vec2f_scale(ball->vel, powf(0.1f, dt));
    }
  }
}

void state_draw(State *state) {
  DrawCircleLines(0, 0, MAX_DIS, state->pal[5]);

  Color *colors = state->pal + 8;

  if (KINDS_MAX > 16) {
    // Could it be check by a preprocessor
    fprintf(stderr, "Not enough colors for all kinds\n");
    return;
  }

  for (int k=0;k<state->rels_len;k++) {
    for (int b=0;b<state->balls_len;b++) {
      Ball *ball = &state->balls[k][b];

      DrawCircleV(*(Vector2*)&(ball->pos), 2.0f, colors[k]);
    }
  }
}

void state_rel_rand(State *state, float scale) {
  for (int k=0;k<state->rels_len;k++) {
    for (int _k=0;_k<state->rels_len;_k++) {
      state->rels[k][_k].force = scale * (frand()-0.5) * 2 * REL_FORCE_MAX;
      state->rels[k][_k].range = scale * frand() * REL_RANGE_MAX;
    }
  }
}

//Color* pal_gen() {
//  Color *pal = malloc(sizeof(Color) * 24);
//
//  if (pal == NULL) {
//    printf("FUCK YOU MALLOC DOESN'T WORK");
//    return pal;
//  }
//
//}

void state_pal_set(State *state, Color *new_pal) {
  if (new_pal == NULL) {
    new_pal = (Color *) PAL_DEFAULT;
  }

  //if (state->pal != PAL_DEFAULT)  {
  //  free(state->pal);
  //}

  state->pal = new_pal;

  #ifdef __EMSCRIPTEN__
    for (int i=0; i<24; i++) {
      EM_ASM({
        i = $0;
        r = $1;
        g = $2;
        b = $3;
        a = $4;

        hex = i.toString(16).padStart(2, '0').toUpperCase();

        document.documentElement.style.setProperty(
            `--base${hex}`, `rgb(${r} ${g} ${b})`);
      }, i, state->pal[i].r, state->pal[i].g, state->pal[i].b, state->pal[i].a);
    }
  #endif

}

void state_pal_rand(State *state) {
  int r = rand() % (sizeof(pals) / sizeof(*pals));

  state_pal_set(state, (Color *) pals[r]);
}

void state_balls_reset(State *state) {

  for (int k=0;k<state->rels_len;k++) {
    for (int b=0;b<state->balls_len;b++) {
      Ball *ball = &state->balls[k][b];


      float angle = 6.28f * frand();
      ball->pos.x = cosf(angle);
      ball->pos.y = sinf(angle);
      ball->pos = vec2f_scale(ball->pos, frand() * MAX_DIS);

      ball->vel = VEC2f_ZERO;
    }
  }
}

#define SNAP_FLOAT_RES (1000.0f)

int state_load_snap(State *state) {
  const char* str = GetClipboardText();

  rini_data snap = rini_load_from_memory(str);

  int ver = rini_get_value(snap, "VERSION");

  if (ver != VERSION) {
    fprintf(stderr, "Version doesn't match\n");
    return 1;
  }

  int len = rini_get_value(snap, "RELATIONS");

  if (len > KINDS_MAX) {
    fprintf(stderr, "Too many kinds\n");
    return 2;
  }

  for (int k=0;k<len;k++) {
    for (int _k=0;_k<len;_k++) {
      state->rels[k][_k].range = rini_get_value(snap,
          TextFormat("RELATION_OF_%d_TO_%d_RANGE", k, _k));
      state->rels[k][_k].range /= SNAP_FLOAT_RES;

      state->rels[k][_k].force = rini_get_value(snap,
          TextFormat("RELATION_OF_%d_TO_%d_FORCE", k, _k));
      state->rels[k][_k].force /= SNAP_FLOAT_RES;
    }
  }

  state->rels_len = len;
  state->balls_len = rini_get_value(snap, "BALLS");

  return 0;
}

int state_save_snap(State *state) {
  rini_data snap = rini_load(NULL);

  rini_set_comment_line(&snap, "");
  rini_set_comment_line(&snap, "Partial Life Snapshot");
  rini_set_comment_line(&snap, "");

  rini_set_value(&snap, "VERSION", VERSION, "Version of the snapshot");
  rini_set_value(&snap, "BALLS", state->balls_len, "Amount of balls per kind");
  rini_set_value(&snap, "RELATIONS", state->rels_len,
                 "Amount of relations/kinds");

  if (state->rels_len > 9) {
    fprintf(stderr, "More kinds then expected for the buffer space\n");
    return 1;
  }

  rini_set_comment_line(&snap, "");
  rini_set_comment_line(&snap, "RELATION_A_B_RANGE -> Minimal range between A and B to apply relation force");
  rini_set_comment_line(&snap, "RELATION_A_B_FORCE -> Force applied on B, if it is in range of A");
  rini_set_comment_line(&snap, "");

  for (int k=0;k<state->rels_len;k++) {
    for (int _k=0;_k<state->rels_len;_k++) {
      rini_set_value(&snap, TextFormat("RELATION_OF_%d_TO_%d_RANGE", k, _k),
                     state->rels[k][_k].range * SNAP_FLOAT_RES, "");
      rini_set_value(&snap, TextFormat("RELATION_OF_%d_TO_%d_FORCE", k, _k),
                     state->rels[k][_k].force * SNAP_FLOAT_RES, "");
    }
  }

  char* str = rini_save_to_memory(snap);
  SetClipboardText(str);
  return 0;
}

char hsl_n(float n, float a, float light, int hue) {
  float k = fmodf(n + ((float)hue / 30.0f), 12.0f);

  float i = (light - (a * MAX(-1.0f, MIN(MIN(k - 3.0f, 9.0f - k), 1.0f))));

  return (i*255.0);
}

Color hsl_to_rgb(int hue, float sat, float light) {
  float a = sat * MIN(light, 1.0f - light);

  return (Color) {
    hsl_n(0.0f, a, light, hue),
    hsl_n(8.0f, a, light, hue),
    hsl_n(4.0f, a, light, hue),
    255,
  };
}

void pal_rand(Color *pal) {
  // 0-7 Greyscale (Dark -> Light)
  // 8-15 Colors
  // 16-23 Darker Colors
  int hue = rand() % 360;
  float light = frand() * 0.2;
  for (int i=0; i<8; i++) {
    pal[i] = hsl_to_rgb(hue, light, 1.0f/9.0f*(i+1));
  }

  int hues[] = {0, 30, 120, 60, 240, 300, 260, 180};

  float sat = frand() * 0.3;
  int hue_shift = rand() % 30;
  int idx_shift = rand() % 8;
  for (int j=0; j<8; j++) {
    int i = (j + idx_shift) % 8;
    pal[j+8] = hsl_to_rgb((hues[i] + hue_shift) % 360, 0.7f + sat, 0.35+light);
    pal[j+16] = hsl_to_rgb((hues[i] + hue_shift) % 360, 0.7f + sat, 0.2+light);
  }
}

void state_rand_pal_set(State *state) {
  pal_rand(state->rand_pal);
  state_pal_set(state, state->rand_pal);
}

int main(void) {
  State state = {0};
  state.balls_len = 258;
  state.rels_len = 4;

  state_balls_reset(&state);
  state_pal_set(&state, NULL);

  InitWindow(320, 240, "Partial Life");
  SetTargetFPS(60);
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  Image icon = LoadImage("icon-32.png");
  SetWindowIcon(icon);

  Camera2D camera = {0};
  camera.target = (Vector2){ 0.0f, 0.0f };
  camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  int debug_ui = 3;

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      camera.offset = (Vector2){GetRenderWidth()/2.0f, GetRenderHeight()/2.0f};

      float val = MIN(camera.offset.x, camera.offset.y);
      camera.zoom = val / (MAX_DIS * 1.1f);
    }

    state_update(&state, GetFrameTime());

    if (IsKeyPressed(KEY_R)) state_rel_rand(&state, 1.0f);
    if (IsKeyPressed(KEY_ZERO)) state_rel_rand(&state, 0.0f);
    if (IsKeyPressed(KEY_Q)) state_balls_reset(&state);
    if (IsKeyPressed(KEY_C)) state_save_snap(&state);
    if (IsKeyPressed(KEY_V)) state_load_snap(&state);
    if (IsKeyPressed(KEY_L)) state_pal_rand(&state);
    if (IsKeyPressed(KEY_P)) state_rand_pal_set(&state);

    if (IsKeyPressed(KEY_ONE)) debug_ui = 0;
    if (IsKeyPressed(KEY_TWO)) debug_ui = 1;
    if (IsKeyPressed(KEY_THREE)) debug_ui = 2;
    if (IsKeyPressed(KEY_H)) debug_ui = 3;
    if (IsKeyPressed(KEY_C)) debug_ui = 4;

    BeginDrawing();
      ClearBackground(state.pal[0]);

      BeginMode2D(camera);
        state_draw(&state);
      EndMode2D();

      if (debug_ui == 1) {
        DrawText(TextFormat("%i FPS %.2f DT", GetFPS(), GetFrameTime()), 0, 0, 10, state.pal[7]);
      } else if (debug_ui == 2) {
        for (int k=0;k<state.rels_len;k++) {
          for (int _k=0;_k<state.rels_len;_k++) {
            Rel rel = state.rels[k][_k];
            DrawText(TextFormat("%f %f", rel.range, rel.force), 0, 10 * (state.rels_len * k + _k), 10, state.pal[7]);
          }
        }
      } else if (debug_ui == 3) {
        const char buffer[][64] = {
          "Shortcuts:",
          "R - randomise relations",
          "0 - zero relations",
          "E - randomise position",
          "",
          "C - save snapshot to clipboard",
          "V - load snapshot from clipboard",
          "",
          "L - load random color palette",
          "P - generate random color palette",
          "",
          "1 - UI diable",
          "2 - UI fps",
          "3 - UI relations",
          "C - UI color palette",
          "H - UI shortcuts / help",
        };
        for (unsigned int i=0; i < (sizeof(buffer) / sizeof(*buffer)); i++) {
          DrawText(buffer[i], 2, 10 * i, 10, state.pal[7]);
        }
      } else if (debug_ui == 4) {
        for(int i=0; i<PAL_LEN; i++) {
          DrawRectangle(10*(i%8), 10*(i/8), 10, 10, state.pal[i]);
        }
      }

    EndDrawing();
  }

  UnloadImage(icon);
  CloseWindow();
}
