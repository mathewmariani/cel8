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

  /* query memory */
  const c8_range_t vram = c8_query_vram();
  const c8_range_t font = c8_query_font();

  /* decode vram */
  for (i32 i = 0, j = 0; i < sizeof(display.screen); i += 8)
  {
    /* convert from screen to cell */
    j = ((i % 128) / 8) + 16 * (i / 1024);

    /* screen buffer */
    u8 color = *((u8 *)vram.ptr + (j * 2) + 0);
    u8 glyph = *((u8 *)vram.ptr + (j * 2) + 1);

    /* convert color */
    u8 high = ((color >> 4) & 0x0F);
    u8 low = ((color) & 0x0F);

    /* decode glyph */
    i32 y = (i / 128) % 8;
    for (i32 x = 0; x < 8; x++)
    {
      u8 b = *((u8 *)font.ptr + y + glyph * 8) >> x;
      *((u8 *)display.screen + i + x) = (b & 1) ? low : high;
    }
  }

  /* query palette data. */
  const c8_range_t pal = c8_query_pal();
  float palette[48] = {0};
  for (i32 i = 0; i < 48; i += 3)
  {
    *(palette + i + 0) = *((u8 *)pal.ptr + i + 0) / 255.0f;
    *(palette + i + 1) = *((u8 *)pal.ptr + i + 1) / 255.0f;
    *(palette + i + 2) = *((u8 *)pal.ptr + i + 2) / 255.0f;
  }

  /* update gpu resources */
  sg_update_image(display.bind.fs.images[0], &(sg_image_data){
                                                 .subimage[0][0] = SG_RANGE(display.screen),
                                             });

  /* graphics pipeline */
  sg_begin_pass(&(sg_pass){.action = display.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(display.pip);
  sg_apply_bindings(&display.bind);
  sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, SG_RANGE_REF(palette));
  sg_draw(0, 6, 1);
  sg_end_pass();
  sg_commit();
}