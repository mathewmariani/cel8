#define C8_IMPL
#include "cel8.h"

/* sokol */
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"
#if defined(_C8_DEBUG)
#include "sokol/sokol_log.h"
#endif

/* std */
#include <stdio.h>
#include <stdlib.h>

enum
{
  MIN_SNAKE_SIZE = 3,
  MAX_SNAKE_SIZE = 256,
  SNAKE_HEAD_SEG = 0x00,
  SNAKE_BODY_SEG = 0x01,
  GRID_PATTERN = 0x02,
  FRUIT_SCORE = 50,
};

typedef struct
{
  u8 x;
  u8 y;
} pos_t;

struct
{
  pos_t pos;
} apple;

struct
{
  pos_t pos[256];
  u8 size;
} snake;

i8 dx = 0;
i8 dy = 0;

static struct
{
  u16 score;
  u16 score_offset;
  i32 timer;
  bool gameover;
  char str_buffer[5];
} state;

static inline bool check_overlap(const pos_t a, const pos_t b)
{
  return (a.x == b.x) && (a.y == b.y);
}

static inline bool check_overlaps(const pos_t a, const pos_t *b, size_t len)
{
  bool ret = false;
  for (i32 i = 0; i < len; i++)
  {
    ret |= check_overlap(a, *(b + i));
  }
  return ret;
}

static inline void place_snake(void)
{
  snake.size = MIN_SNAKE_SIZE;
  for (i32 i = 0; i < snake.size; i++)
  {
    snake.pos[i].x = 8;
    snake.pos[i].y = 8;
  }
}

static inline void place_apple(void)
{
  do
  {
    apple.pos.x = rand() % 0x0f;
    apple.pos.y = rand() % 0x0f;
  } while (check_overlaps(apple.pos, &snake.pos, snake.size));
}

static struct
{
  sg_pass_action pass_action;
  sg_pipeline pip;
  sg_bindings bind;
} display;

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
  display.pass_action = (sg_pass_action){
      .colors[0] = {
          .load_action = SG_LOADACTION_CLEAR,
          .clear_value = {0.0f, 0.0f, 0.0f, 1.0f},
      },
  };

  /* a pipeline state object */
  display.pip = sg_make_pipeline(&(sg_pipeline_desc){
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
  display.bind = (sg_bindings){
      .vertex_buffers = {[0] = vbuf},
      .index_buffer = ibuf,
      .fs = {
          .images = {[0] = screen},
          .samplers = {[0] = smp},
      },
  };

/* cel8 */
#include "embed/font.h"
#include "embed/palette.h"
  c8_init(&(c8_desc_t){
      .flags = C8_FPS30,
      .roms = {
          .chars = (c8_range_t){.ptr = font_h, .size = sizeof(font_h)},
          .palette = (c8_range_t){.ptr = palette_h, .size = sizeof(palette_h)},
      },
  });

  /* create sprites */
  /* snake head */
  /* 01111110  -- 0x7E */
  /* 11111111  -- 0xFF */
  /* 11011011  -- 0xDB */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 01111110  -- 0x7E */
  c8_poke4(C8_FONT_ADDR + 0x00, 0x00 * 4, 0x7EFFDBFF);
  c8_poke4(C8_FONT_ADDR + 0x00, 0x01 * 4, 0xFFFFFF7E);

  /* snake segment */
  /* 01111110  -- 0x7E */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 01111110  -- 0x7E */
  c8_poke4(C8_FONT_ADDR + 0x08, 0x00 * 4, 0x7EFFFFFF);
  c8_poke4(C8_FONT_ADDR + 0x08, 0x01 * 4, 0xFFFFFF7E);

  /* grid pattern */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  /* 00011000  -- 0x18 */
  /* 00011000  -- 0x18 */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  c8_poke4(C8_FONT_ADDR + 0x10, 0x00 * 4, 0x00000018);
  c8_poke4(C8_FONT_ADDR + 0x10, 0x01 * 4, 0x18000000);

  place_snake();
  place_apple();

  /* game state */
  state.gameover = false;
  state.timer = 0;
}

static void move_snake(void)
{
  pos_t temp;
  pos_t prev = snake.pos[0];
  snake.pos[0].x += dx;
  snake.pos[0].y += dy;

  /* move snake segments */
  for (int i = 1; i < snake.size; i++)
  {
    temp = snake.pos[i];
    snake.pos[i] = prev;
    prev = temp;
  }

  /* check for collisions with body */
  for (int i = 1; i < snake.size; i++)
  {
    if (check_overlap(snake.pos[0], snake.pos[i]))
    {
      state.gameover = true;
    }
  }

  /* check for collision with apple */
  if (check_overlap(snake.pos[0], apple.pos))
  {
    /* increae snake size */
    snake.size++;
    snake.pos[snake.size - 1] = snake.pos[0];
    state.score += FRUIT_SCORE;

    place_apple();
  }
}

static void event(const sapp_event *e)
{
  if (e->key_repeat || e->type != SAPP_EVENTTYPE_KEY_DOWN)
  {
    return;
  }
  switch (e->key_code)
  {
  case SAPP_KEYCODE_UP:
    dx = 0, dy = -1;
    break;
  case SAPP_KEYCODE_DOWN:
    dx = 0, dy = +1;
    break;
  case SAPP_KEYCODE_LEFT:
    dx = -1, dy = 0;
    break;
  case SAPP_KEYCODE_RIGHT:
    dx = +1, dy = 0;
    break;
  default:
    break;
  }
  state.timer = 0;
}

static void frame(void)
{
  if ((--state.timer) <= 0)
  {
    state.timer = 5 - (snake.size / 12);
    if (state.timer < 2)
    {
      state.timer = 2;
    }
    move_snake();
  }

  { /* render */
    c8_cls(0x00, 0x00);

    /* draw board */
    c8_color(0x01);
    c8_fill(0, 0, 16, 16, GRID_PATTERN);

    /* draw the score */
    int score = (state.score_offset < state.score) ? ++state.score_offset : state.score;
    sprintf(state.str_buffer, "%d", score);
    c8_color(0x06);
    c8_print(1, 1, "SCORE:");
    c8_print(8, 1, state.str_buffer);

    /* draw the apple and stem */
    c8_color(0x08);
    c8_print(apple.pos.x, apple.pos.y, "@");
    c8_color(0x0B);
    c8_print(apple.pos.x, apple.pos.y - 1, "c");

    /* draw the snake */
    c8_color(0x0C);
    c8_put(snake.pos[0].x, snake.pos[0].y, SNAKE_HEAD_SEG);
    for (i32 i = 1; i < snake.size; i++)
    {
      c8_put(snake.pos[i].x, snake.pos[i].y, SNAKE_BODY_SEG);
    }
  }

  /* program */
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
  sg_update_image(display.bind.fs.images[0], &(sg_image_data){
                                                 .subimage[0][0] = SG_RANGE(cel8.screen),
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