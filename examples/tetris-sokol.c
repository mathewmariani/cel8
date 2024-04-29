#define C8_IMPL
#include "cel8.h"

/* std */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
  TETRIS_FIELD_WIDTH = 8,
  TETRIS_FIELD_HEIGHT = 16,
  TETRIS_PIECE_COUNT = 7,
  TETRIS_CLEAR_LINE_TICKS = 25,
};

u8 color_table[] = {
    0x90,
    0xC0,
    0xA0,
    0xF0,
    0xE0,
    0x80,
    0xD0,
    0x70,
};

f32 gravity_table[] = {
    0.01667f,
    0.021017f,
    0.026977f,
    0.035256f,
    0.04693f,
    0.06361f,
    0.0879f,
    0.1236f,
    0.1775f,
    0.2598f,
    0.388f,
    0.59f,
    0.92f,
    1.46f,
    2.36f,
};

u8 tetromino[7][16] = {
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

typedef struct
{
  i8 x, y, r;
} tetromino_t;

struct
{
  u64 ticks;
  u16 score;
  u16 score_offset;
  u8 level;
  u8 lines;
  i8 fx_ticks;

  bool should_clear;
  bool gameover;

  char str_buffer[5];

  int cx, cy, cr;
  int cp, np;
  i8 grid[16][8];

} state;

static int rotate(int x, int y, int r)
{
  switch (r % 4)
  {
  case 0:
    return y * 4 + x;
  case 1:
    return 12 + y - (x * 4);
  case 2:
    return 15 - (y * 4) - x;
  case 3:
    return 3 - y + (x * 4);
  default:
    return 0;
  }
}

static bool does_piece_fit(int i, int r, int px, int py)
{
  u8 *t = tetromino[i];
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
      if (t[rotate(x, y, r)] == 0)
      {
        continue;
      }
      if ((px + x) >= 0 && (px + x) < 8 && (py + y) >= 0 && (py + y) < 16)
      {
        if (state.grid[y + py][x + px] != 0)
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

static void drop_piece(void)
{
  while (does_piece_fit(state.cp, state.cr, state.cx, state.cy + 1))
  {
    state.cy++;
  }
}

static void draw_tetromino(int p, int x, int y, int r)
{
  c8_color(color_table[p]);
  for (int j = 0; j < 4; j++)
  {
    for (int i = 0; i < 4; i++)
    {
      if (tetromino[p][rotate(i, j, r)] == 0)
      {
        continue;
      }
      c8_put(i + x, j + y, 0x00);
    }
  }
}

static void lock_piece(void)
{
  u8 *t = tetromino[state.cp];
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
      if (t[rotate(x, y, state.cr)] != 0)
      {
        state.grid[y + state.cy][x + state.cx] = color_table[state.cp];
      }
    }
  }
}

static int count_line(int y)
{
  int count = 0;
  for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
  {
    count += (state.grid[y][x] != 0) ? 1 : 0;
  }
  return count;
}

static void flag_for_clear(int y)
{
  state.should_clear = true;
  for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
  {
    state.grid[y][x] = 0x70;
  }
}

static void clear_line(int line)
{
  for (int y = line; y > 0; y--)
  {
    for (int x = 0; x < 8; x++)
    {
      state.grid[y][x] = state.grid[y - 1][x];
    }
  }
  state.lines++;
}

static void clear_lines(void)
{
  int count;
  for (int y = 0; y < TETRIS_FIELD_HEIGHT; y++)
  {
    count = 0;
    for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
    {
      count += (state.grid[y][x] != 0) ? 1 : 0;
    }

    /* clear the row */
    if (count == TETRIS_FIELD_WIDTH)
    {
      flag_for_clear(y);
    }
  }
}

static void move_piece(void)
{
  u8 *t = tetromino[state.cp];
  if (does_piece_fit(state.cp, state.cr, state.cx, state.cy + 1))
  {
    state.cy++;
  }
  else
  {
    lock_piece();
    clear_lines();

    /* update score, lines and level */
    state.score += 25;
    state.level = (int)floor(state.lines / 10);

    /* new piece */
    state.cx = 2;
    state.cy = 0;
    state.cr = 0;
    state.cp = state.np;
    state.np = rand() % 7;

    /* game over */
    state.gameover = !does_piece_fit(state.cp, state.cr, state.cx, state.cy);
  }
}

static void reset()
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

  /* FIXME: api needs to adapt. */
  c8_poke4(0x0050, 0x00 * 4, 0x00808080);
  c8_poke4(0x0050, 0x01 * 4, 0x8080807E);
  c8_poke4(0x0058, 0x00 * 4, 0x00000018);
  c8_poke4(0x0058, 0x01 * 4, 0x18000000);

  state.ticks = 0;
  state.fx_ticks = 0;
  state.score = 0;
  state.level = 0;
  state.lines = 0;
  state.gameover = false;
  state.should_clear = false;
  state.cx = 2;
  state.cy = 0;
  state.cr = 0;
  state.cp = rand() % 7;
  state.np = rand() % 7;

  memset(&state.grid, 0x00, sizeof(state.grid));
}

/* sokol */
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"
#if defined(_C8_DEBUG)
#include "sokol/sokol_log.h"
#endif

static struct
{
  sg_pass_action pass_action;
  sg_pipeline pip;
  sg_bindings bind;
} str_buffer;

static void init(void)
{
  /* setup sokol-gfx */
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
  });

  /* a vertex buffer */
  const f32 vertices[] = {
      /* positions */           /* texture coord */
      -1.0f, 1.0f, 0.0f, 0.0f,  /* top-left */
      1.0f, 1.0f, 1.0f, 0.0f,   /* top-right */
      1.0f, -1.0f, 1.0f, 1.0f,  /* bottom-right */
      -1.0f, -1.0f, 0.0f, 1.0f, /* bottom-left */
  };

  /* an index buffer with 2 triangles */
  const u16 indices[] = {0, 1, 2, 0, 2, 3};

  sg_buffer vbuf = sg_make_buffer(&(sg_buffer_desc){
      .data = SG_RANGE(vertices),
      .label = "quad-vertices",
  });

  sg_buffer ibuf = sg_make_buffer(&(sg_buffer_desc){
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .data = SG_RANGE(indices),
      .label = "quad-indices",
  });

/* load embedded shader stages */
#include "embed/vertex.vs.h"
#include "embed/fragment.fs.h"
  sg_shader_desc shd_desc = (sg_shader_desc){
      .vs = {
          .source = (const char *)&vertex_vs[0],
      },
      .fs = {
          .source = (const char *)&fragment_fs[0],
          .images = {
              [0] = {.used = true, .image_type = SG_IMAGETYPE_2D},
          },
          .samplers = {
              [0] = {.used = true, .sampler_type = SG_SAMPLERTYPE_FILTERING},
          },
          .image_sampler_pairs = {
              [0] = {
                  .used = true,
                  .glsl_name = "screen",
                  .image_slot = 0,
                  .sampler_slot = 0,
              },
          },
          .uniform_blocks = {
              [0] = {
                  .size = sizeof(f32) * 48,
                  .uniforms = {
                      [0] = {
                          .name = "palette",
                          .type = SG_UNIFORMTYPE_FLOAT3,
                          .array_count = 16,
                      },
                  },
              },
          },
      },
  };

  /* default pass action */
  str_buffer.pass_action = (sg_pass_action){
      .colors[0] = {
          .load_action = SG_LOADACTION_CLEAR,
          .clear_value = {0.0f, 0.0f, 0.0f, 1.0f},
      },
  };

  /* a pipeline state object */
  str_buffer.pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = sg_make_shader(&shd_desc),
      .index_type = SG_INDEXTYPE_UINT16,
      .layout = {
          .attrs = {
              [0].format = SG_VERTEXFORMAT_FLOAT2,
              [1].format = SG_VERTEXFORMAT_FLOAT2,
          },
      },
      .label = "quad-pipeline",
  });

  /* images and samplers */
  sg_sampler smp = sg_make_sampler(&(sg_sampler_desc){
      .min_filter = SG_FILTER_NEAREST,
      .mag_filter = SG_FILTER_NEAREST,
      .wrap_u = SG_WRAP_REPEAT,
      .wrap_v = SG_WRAP_REPEAT,
      .label = "screen-sampler",
  });

  sg_image screen = sg_make_image(&(sg_image_desc){
      .width = C8_SCREEN_WIDTH,
      .height = C8_SCREEN_HEIGHT,
      .pixel_format = SG_PIXELFORMAT_R8,
      .usage = SG_USAGE_STREAM,
      .label = "screen-texture",
  });

  /* bindings */
  str_buffer.bind = (sg_bindings){
      .vertex_buffers = {[0] = vbuf},
      .index_buffer = ibuf,
      .fs = {
          .images = {[0] = screen},
          .samplers = {[0] = smp},
      },
  };

  reset();
}

static void event(const sapp_event *e)
{

  if (e->type == SAPP_EVENTTYPE_KEY_UP && e->key_code == SAPP_KEYCODE_R)
  {
    reset();
  }

  if (e->key_repeat || e->type != SAPP_EVENTTYPE_KEY_DOWN)
  {
    return;
  }
  if (e->key_code == SAPP_KEYCODE_UP)
  {
    if (does_piece_fit(state.cp, state.cr + 1, state.cx, state.cy))
    {
      state.cr++;
    }
  }
  else if (e->key_code == SAPP_KEYCODE_DOWN)
  {
    drop_piece();
  }
  else if (e->key_code == SAPP_KEYCODE_LEFT)
  {
    if (does_piece_fit(state.cp, state.cr, state.cx - 1, state.cy))
    {
      state.cx--;
    }
  }
  else if (e->key_code == SAPP_KEYCODE_RIGHT)
  {
    if (does_piece_fit(state.cp, state.cr, state.cx + 1, state.cy))
    {
      state.cx++;
    }
  }
}

static inline int drop_speed()
{
  return (int)(1 / gravity_table[state.level]);
}

static void frame(void)
{
  if (!state.gameover)
  {
    if (state.ticks % drop_speed() == 0)
    {
      move_piece();
    }
    state.ticks++;
  }

  /* apply fx */
  if (state.should_clear)
  {
    if (++state.fx_ticks >= TETRIS_CLEAR_LINE_TICKS)
    {
      for (int y = 0; y < TETRIS_FIELD_HEIGHT; y++)
      {
        if (state.grid[y][0] == color_table[7])
        {
          clear_line(y);
        }
      }
      state.fx_ticks = 0;
      state.should_clear = false;
    }
  }

  { /* render */
    c8_cls(0x00, 0x00);

    /* draw field */
    for (int y = 0; y < TETRIS_FIELD_HEIGHT; y++)
    {
      for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
      {
        c8_color(state.grid[y][x] == 0x00 ? 0x05 : state.grid[y][x]);
        c8_put(x, y, state.grid[y][x] == 0x00 ? 0x01 : 0x00);
      }
    }

    /* draw current and next tetromino */
    draw_tetromino(state.cp, state.cx, state.cy, state.cr);
    draw_tetromino(state.np, 9, 3, 0);

    /* draw user interface */
    c8_color(0x05);
    c8_print(9, 1, "NEXT:");

    /* draw score */
    int score = (state.score_offset < state.score) ? ++state.score_offset : state.score;
    sprintf(state.str_buffer, "%d", score);
    c8_color(0x05);
    c8_print(9, 8, "SCORE:");
    c8_color(0x06);
    c8_print(9, 9, state.str_buffer);

    /* draw level */
    sprintf(state.str_buffer, "%d", state.level);
    c8_color(0x05);
    c8_print(9, 11, "LEVEL:");
    c8_color(0x06);
    c8_print(9, 12, state.str_buffer);

    /* draw lines */
    sprintf(state.str_buffer, "%d", state.lines);
    c8_color(0x05);
    c8_print(9, 14, "LINES:");
    c8_color(0x06);
    c8_print(9, 15, state.str_buffer);

    if (state.gameover)
    {
      c8_color(0x07);
      c8_print(2, 7, "GAME");
      c8_print(2, 8, "OVER");
      c8_color(0x05);
      c8_print(1, 11, "PRESS ");
      c8_print(1, 12, "'R' TO");
      c8_print(1, 13, " RESET");
    }
  }

  c8_frame();

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
  sg_update_image(str_buffer.bind.fs.images[0], &(sg_image_data){
                                                    .subimage[0][0] = SG_RANGE(cel8.screen),
                                                });

  /* graphics pipeline */
  sg_begin_pass(&(sg_pass){.action = str_buffer.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(str_buffer.pip);
  sg_apply_bindings(&str_buffer.bind);
  sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, SG_RANGE_REF(palette));
  sg_draw(0, 6, 1);
  sg_end_pass();
  sg_commit();
}

static void cleanup(void)
{
  sg_shutdown();
}

sapp_desc sokol_main(i32 argc, char *argv[])
{
  /* sokol */
  return (sapp_desc)
  {
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .event_cb = event,
    .width = C8_WINDOW_WIDTH,
    .height = C8_WINDOW_HEIGHT,
    .window_title = "cel8",

#if defined(_C8_DEBUG)
    .win32_console_create = true,
    .logger.func = slog_func,
#endif
  };
}