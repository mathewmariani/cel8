#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

#include <stdio.h>

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

int x = 8;
int y = 8;
// static void event(const sapp_event *e)
// {
//   if (e->key_code == SAPP_KEYCODE_UP)
//   {
//     y++;
//   }
//   else if (e->key_code == SAPP_KEYCODE_DOWN)
//   {
//     y--;
//   }
//   else if (e->key_code == SAPP_KEYCODE_LEFT)
//   {
//     x--;
//   }
//   else if (e->key_code == SAPP_KEYCODE_RIGHT)
//   {
//     x++;
//   }
// }

static void c8_update(void)
{
  /* body */
}

static void c8_draw(void)
{
  /* program */
  c8_cls(0x00, 0x00);

  /* field */
  c8_color(0x05);
  c8_fill(0, 0, 16, 16, ".");

  /* crosshair */
  c8_color(0x06);
  c8_fill(0, y, 16, 1, ".");
  c8_fill(x, 0, 1, 16, ".");

  /* player */
  c8_color(0x08);
  c8_put(x, y, "@");
}