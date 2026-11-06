/* Palette: Arthur */
/* Source: https://github.com/tinted-theming/schemes/blob/spec-0.11/base24/arthur.yaml */

#include "raylib.h"

#define GetColor(hexValue) ((Color){        \
    (unsigned char)(hexValue >> 24) & 0xff, \
    (unsigned char)(hexValue >> 16) & 0xff, \
    (unsigned char)(hexValue >> 8) & 0xff,  \
    (unsigned char)hexValue & 0xff})

const Color palette[24] = {
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
