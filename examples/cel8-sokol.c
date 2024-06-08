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
} display;

static void init(void)
{
    /* setup sokol-gfx */
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });

    /* a vertex buffer */
    const float vertices[] = {
        /* positions */           /* texture coord */
        -1.0f, 1.0f, 0.0f, 0.0f,  /* top-left */
        1.0f, 1.0f, 1.0f, 0.0f,   /* top-right */
        1.0f, -1.0f, 1.0f, 1.0f,  /* bottom-right */
        -1.0f, -1.0f, 0.0f, 1.0f, /* bottom-left */
    };

    /* an index buffer with 2 triangles */
    const uint16_t indices[] = {0, 1, 2, 0, 2, 3};

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
                    .size = sizeof(float) * 48,
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

    /* images and samplers */
    sg_sampler smp = sg_make_sampler(&(sg_sampler_desc){
        .min_filter = SG_FILTER_NEAREST,
        .mag_filter = SG_FILTER_NEAREST,
        .wrap_u = SG_WRAP_REPEAT,
        .wrap_v = SG_WRAP_REPEAT,
        .label = "screen-sampler",
    });

    sg_image img = sg_make_image(&(sg_image_desc){
        .width = C8_SCREEN_WIDTH,
        .height = C8_SCREEN_HEIGHT,
        .pixel_format = SG_PIXELFORMAT_R8,
        .usage = SG_USAGE_STREAM,
        .label = "screen-texture",
    });

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

    /* bindings */
    display.bind = (sg_bindings){
        .vertex_buffers = {[0] = vbuf},
        .index_buffer = ibuf,
        .fs = {
            .images = {[0] = img},
            .samplers = {[0] = smp},
        },
    };

    c8_load();
}

static void event(const sapp_event *e)
{
    switch (e->type)
    {
    case SAPP_EVENTTYPE_KEY_DOWN:
        switch (e->key_code)
        {
        case SAPP_KEYCODE_RIGHT:
            c8_input_set(C8_INPUT_RIGHT);
            break;
        case SAPP_KEYCODE_LEFT:
            c8_input_set(C8_INPUT_LEFT);
            break;
        case SAPP_KEYCODE_UP:
            c8_input_set(C8_INPUT_UP);
            break;
        case SAPP_KEYCODE_DOWN:
            c8_input_set(C8_INPUT_DOWN);
            break;
        case SAPP_KEYCODE_Z:
            c8_input_set(C8_INPUT_A);
            break;
        case SAPP_KEYCODE_X:
            c8_input_set(C8_INPUT_B);
            break;
        case SAPP_KEYCODE_E:
            c8_input_set(C8_INPUT_START);
            break;
        case SAPP_KEYCODE_R:
            c8_input_set(C8_INPUT_SELECT);
            break;
        default:
            break;
        }
        break;
    case SAPP_EVENTTYPE_KEY_UP:
        switch (e->key_code)
        {
        case SAPP_KEYCODE_RIGHT:
            c8_input_clear(C8_INPUT_RIGHT);
            break;
        case SAPP_KEYCODE_LEFT:
            c8_input_clear(C8_INPUT_LEFT);
            break;
        case SAPP_KEYCODE_UP:
            c8_input_clear(C8_INPUT_UP);
            break;
        case SAPP_KEYCODE_DOWN:
            c8_input_clear(C8_INPUT_DOWN);
            break;
        case SAPP_KEYCODE_Z:
            c8_input_clear(C8_INPUT_A);
            break;
        case SAPP_KEYCODE_X:
            c8_input_clear(C8_INPUT_B);
            break;
        case SAPP_KEYCODE_E:
            c8_input_clear(C8_INPUT_START);
            break;
        case SAPP_KEYCODE_R:
            c8_input_clear(C8_INPUT_SELECT);
            break;
        default:
            break;
        }
    default:
        break;
    }
}

static void frame(void)
{
    c8_exec();
    c8_update();
    c8_draw();

    /* FIXME: proof of concept for `c8_btnp()` */
    c8_input_clear(C8_INPUT_RIGHT | C8_INPUT_LEFT | C8_INPUT_UP | C8_INPUT_DOWN | C8_INPUT_A | C8_INPUT_B | C8_INPUT_START | C8_INPUT_SELECT);

    /* query palette data. */
    const c8_range_t pal = c8_query_pal();
    float palette[48] = {0};
    for (int32_t i = 0; i < 48; i += 3)
    {
        *(palette + i + 0) = *((uint8_t *)pal.ptr + i + 0) / 255.0f;
        *(palette + i + 1) = *((uint8_t *)pal.ptr + i + 1) / 255.0f;
        *(palette + i + 2) = *((uint8_t *)pal.ptr + i + 2) / 255.0f;
    }

    /* query screen data. */
    const c8_range_t screen = c8_query_screen();

    /* update gpu resources */
    sg_update_image(display.bind.fs.images[0], &(sg_image_data){
                                                   .subimage[0][0] = {.ptr = screen.ptr, .size = screen.size},
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