#define CEL8_IMPL
#include "cel8.h"

/* sokol */
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"
#if defined(_CEL8_DEBUG)
#include "sokol/sokol_log.h"
#endif

#include <stdio.h>

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
      .width = CEL8_SCREEN_WIDTH,
      .height = CEL8_SCREEN_HEIGHT,
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
      .roms = {
          .chars = (c8_range_t){.ptr = font_h, .size = sizeof(font_h)},
          .palette = (c8_range_t){.ptr = palette_h, .size = sizeof(palette_h)},
      },
  });
}

static void event(const sapp_event *e)
{
  /* body */
}

#include <stdlib.h>

static void frame(void)
{
  // local n = rnd(0, 0x10 * 0x10)
  // cel8.poke(0x0000 + (n * 2)
  //   , rnd(0x00, 0xff)
  //   , rnd(0x00, 0x7f))

  // cel8_poke(addr, i, value);

  /* program */
  i8 n = (rand() % ((0x10 * 0x10) * 2));
  c8_poke(CEL8_VRAM_ADDR + n, 0x00, rand() % 0xff);
  c8_poke(CEL8_VRAM_ADDR + n, 0x01, rand() % 0x7f);
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
    .width = CEL8_WINDOW_WIDTH,
    .height = CEL8_WINDOW_HEIGHT,
    .window_title = "cel8",

#if defined(_CEL8_DEBUG)
    .win32_console_create = true,
    .logger.func = slog_func,
#endif
  };
}