#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

/* std */
#include <stdio.h>
#include <stdlib.h>

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

  /* program */
  int i = 0;
  for (int32_t y = 0; y <= 3; y++)
  {
    for (int32_t x = 0; x <= 3; x++)
    {
      c8_color(i << 4);
      c8_put(x, y, 0x00);
      i = i + 1;
    }
  }

  i = 0;
  for (int32_t y = 0; y <= 3; y++)
  {
    for (int32_t x = 0; x <= 3; x++)
    {
      c8_color(i << 4);
      c8_put(x, y, 0x00);
      i = i + 1;
    }
  }
}

void c8_update(void)
{
  /* body */
}

void c8_draw(void)
{
  /* body */
}