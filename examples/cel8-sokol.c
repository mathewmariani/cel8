#pragma once
/*
    platform-sokol.h -- platform abstraction for cel8 using sokol.

    Do this:
        #define C8_PLATFORM_SOKOL
    before you include this file in *one* C or C++ file to create the
    implementation.
*/

static void c8_load(void);
static void c8_update(void);
static void c8_draw(void);

#ifdef C8_PLATFORM_SOKOL

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
    u8 screen[0x4000];
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
#if defined(OS_EMSCRIPTEN)
#include "embed/vertex.opengles.vs.h"
#include "embed/fragment.opengles.fs.h"
#else
#include "embed/vertex.opengl.vs.h"
#include "embed/fragment.opengl.fs.h"
#endif
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

    c8_load();
}

static void event(const sapp_event *e)
{
}

static void frame(void)
{
    c8_frame();
    c8_update();
    c8_draw();

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

static void cleanup(void)
{
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char *argv[])
{
    /* sokol */
    return (sapp_desc){
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

#endif /* PLATFORM_SOKOL */