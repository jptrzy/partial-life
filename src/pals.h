
#include "raylib.h"

#define GetColor(hexValue) ((Color){        \
    (unsigned char)(hexValue >> 24) & 0xff, \
    (unsigned char)(hexValue >> 16) & 0xff, \
    (unsigned char)(hexValue >> 8) & 0xff,  \
    (unsigned char)hexValue & 0xff})

/* Palette: Arthur */
/* Source: https://github.com/tinted-theming/schemes/blob/spec-0.11/base24/arthur.yaml */
const Color pal_arthur[24] = {
  GetColor(0x1c1c1dff),
  GetColor(0x3d352aff),
  GetColor(0x554444ff),
  GetColor(0x6e5d59ff),
  GetColor(0x88776eff),
  GetColor(0xa19083ff),
  GetColor(0xbbaa99ff),
  GetColor(0xddccbbff),
  GetColor(0xcd5c5cff),
  GetColor(0xe8ae5bff),
  GetColor(0x87ceebff),
  GetColor(0x86af80ff),
  GetColor(0xb0c4deff),
  GetColor(0x6495edff),
  GetColor(0xdeb887ff),
  GetColor(0x662e2eff),
  GetColor(0x382d2dff),
  GetColor(0x1c1616ff),
  GetColor(0xcc5533ff),
  GetColor(0xffa75dff),
  GetColor(0x88aa22ff),
  GetColor(0xb0c4deff),
  GetColor(0x87ceebff),
  GetColor(0x996600ff),
};


const Color pal_alucard[24] = {
  GetColor(0xfffbebff),
  GetColor(0xceccc0ff),
  GetColor(0xcfcfdeff),
  GetColor(0x6c664bff),
  GetColor(0x4c4a3dff),
  GetColor(0x1f1f1fff),
  GetColor(0x1f1f1fff),
  GetColor(0x2c2b31ff),
  GetColor(0xcb3a2aff), 
  GetColor(0xA34D14ff),
  GetColor(0x846e15ff),
  GetColor(0x14710aff),
  GetColor(0x036a96ff),
  GetColor(0x644ac9ff),
  GetColor(0xa3144dff),
  GetColor(0x792219ff),
  GetColor(0xfdf8e2ff),
  GetColor(0xf5f0daff),
  GetColor(0xd74c3dff),
  GetColor(0x9e841aff),
  GetColor(0x198d0cff),
  GetColor(0x047fb4ff),
  GetColor(0x7862d0ff),
  GetColor(0xbf185aff),
};

const Color pal_laser[] = {
  GetColor(0x030d18ff),
  GetColor(0x616161ff),
  GetColor(0x8e8e8eff),
  GetColor(0xa6a6a6ff),
  GetColor(0xbfbfbfff),
  GetColor(0xd8d8d8ff),
  GetColor(0xf1f1f1ff),
  GetColor(0xfeffffff),
  GetColor(0xff8272ff),
  GetColor(0x09b4bdff),
  GetColor(0xf92883ff),
  GetColor(0xb4fa72ff),
  GetColor(0xd0d1feff),
  GetColor(0xfed300ff),
  GetColor(0xff8ffdff),
  GetColor(0x7f4139ff),
  GetColor(0x5e5e5eff),
  GetColor(0x2f2f2fff),
  GetColor(0xffc4bdff),
  GetColor(0xfefdd5ff),
  GetColor(0xd6fcb9ff),
  GetColor(0xe5e6feff),
  GetColor(0xf92883ff),
  GetColor(0xffb1feff),
};

const Color pal_miami[] = {
  GetColor(0x000000ff),
  GetColor(0x111112ff),
  GetColor(0x1e1d1fff),
  GetColor(0x69676cff),
  GetColor(0x88858cff),
  GetColor(0xf7f1ffff),
  GetColor(0xf9f4ffff),
  GetColor(0xf7f1ffff),
  GetColor(0xff4c8bff),
  GetColor(0xff926cff),
  GetColor(0xffd84cff),
  GetColor(0x7fffd4ff),
  GetColor(0x47cfffff),
  GetColor(0x00ffa8ff),
  GetColor(0xd36cffff),
  GetColor(0xa65f46ff),
  GetColor(0x000000ff),
  GetColor(0x000000ff),
  GetColor(0xff4c8bff),
  GetColor(0xffd84cff),
  GetColor(0x7fffd4ff),
  GetColor(0x47cfffff),
  GetColor(0x00ffa8ff),
  GetColor(0xd36cffff),
};

const Color *pals[] = {
  pal_laser,
  pal_arthur,
  //pal_alucard,
  pal_miami,
};
