#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

#include <stdio.h>

int x = 8;
int y = 8;

void c8_load(void)
{
/* cel8 */
#include "embed/font.h"
#include "embed/palette.h"
  c8_init(&(c8_desc_t){
      .roms = {
          .chars = (c8_range_t){.ptr = font_h, .size = sizeof(font_h)},
          .palette = (c8_range_t){.ptr = palette_h, .size = sizeof(palette_h)},
      },
  });
}

static void c8_update(void)
{
  if (c8_btn(C8_INPUT_UP))
  {
    y--;
  }
  else if (c8_btn(C8_INPUT_DOWN))
  {
    y++;
  }
  else if (c8_btn(C8_INPUT_LEFT))
  {
    x--;
  }
  else if (c8_btn(C8_INPUT_RIGHT))
  {
    x++;
  }
}

static void c8_draw(void)
{
  /* program */
  c8_cls(0x00, 0x00);

  /* field */
  c8_color(0x05);
  c8_fill(0, 0, 16, 16, 0x53);

  /* crosshair */
  c8_color(0x06);
  c8_fill(0, y, 16, 1, 0x53);
  c8_fill(x, 0, 1, 16, 0x53);

  /* player */
  c8_color(0x08);
  c8_put(x, y, 0x40);
}