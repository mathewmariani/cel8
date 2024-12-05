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
    sg_pass pass;
    sg_pipeline pip;
    sg_bindings bind;
} display;

static void init(void)
{
    /* setup sokol-gfx */
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });

    /* default pass action */
    display.pass = (sg_pass){
        .action = (sg_pass_action){
            .colors[0] = {
                .load_action = SG_LOADACTION_CLEAR,
                .clear_value = {0.0f, 0.0f, 0.0f, 1.0f},
            },
        },
        .swapchain = sglue_swapchain(),
    };

    const char *display_vs_src = 0;
    const char *display_fs_src = 0;
    switch (sg_query_backend())
    {
    case SG_BACKEND_GLCORE:
        display_vs_src =
            "#version 410\n"
            "layout(location=0) in vec2 pos;\n"
            "out vec2 uv;\n"
            "void main() {\n"
            "  gl_Position = vec4((pos.xy - 0.5) * 2.0, 0.0, 1.0);\n"
            "  uv = vec2(pos.x, 1.0 - pos.y);\n"
            "}\n";
        display_fs_src =
            "#version 410\n"
            "uniform vec3[16] palette;\n"
            "uniform sampler2D tex;\n"
            "in vec2 uv;\n"
            "out vec4 frag_color;\n"
            "vec4 getColor(int idx) { return vec4(palette[idx], 1.0); }\n"
            "void main() {\n"
            "  frag_color = getColor(int(texture(tex, uv).r * 255.0 + 0.5));\n"
            "}\n";
        break;
    case SG_BACKEND_GLES3:
        display_vs_src =
            "#version 300 es\n"
            "attribute vec2 pos;\n"
            "varying vec2 uv;\n"
            "void main() {\n"
            "  gl_Position = vec4((pos.xy - 0.5) * 2.0, 0.0, 1.0);\n"
            "  uv = vec2(pos.x, 1.0 - pos.y);\n"
            "}\n";
        display_fs_src =
            "#version 300 es\n"
            "precision mediump float;\n"
            "uniform vec3[16] palette;\n"
            "uniform sampler2D tex;\n"
            "varying vec2 uv;\n"
            "out vec4 frag_color;\n"
            "vec4 getColor(int idx) { return vec4(palette[idx], 1.0); }\n"
            "void main() {\n"
            "  frag_color = getColor(int(texture(tex, uv).r * 255.0 + 0.5));\n"
            "}\n";
        break;
    default:
        break;
    }

    /* a pipeline state object */
    display.pip = sg_make_pipeline(&(sg_pipeline_desc){
        .layout = {
            .attrs[0].format = SG_VERTEXFORMAT_FLOAT2,
        },
        .shader = sg_make_shader(&(sg_shader_desc){
            .vertex_func.source = display_vs_src,
            .fragment_func.source = display_fs_src,
            .images[0] = {.stage = SG_SHADERSTAGE_FRAGMENT, .image_type = SG_IMAGETYPE_2D},
            .samplers[0] = {.stage = SG_SHADERSTAGE_FRAGMENT, .sampler_type = SG_SAMPLERTYPE_FILTERING},
            .image_sampler_pairs[0] = {
                .stage = SG_SHADERSTAGE_FRAGMENT,
                .glsl_name = "tex",
                .image_slot = 0,
                .sampler_slot = 0,
            },
            .uniform_blocks[0] = {
                .stage = SG_SHADERSTAGE_FRAGMENT,
                .layout = SG_UNIFORMLAYOUT_NATIVE,
                .size = sizeof(float) * 48,
                .glsl_uniforms[0] = {
                    .glsl_name = "palette",
                    .type = SG_UNIFORMTYPE_FLOAT3,
                    .array_count = 16,
                },
            },
        }),
        .index_type = SG_INDEXTYPE_UINT16,
    });

    /* a vertex buffer */
    const float vertices[] = {
        -1.0f, 1.0f,  /* top-left */
        1.0f, 1.0f,   /* top-right */
        1.0f, -1.0f,  /* bottom-right */
        -1.0f, -1.0f, /* bottom-left */
    };

    /* an index buffer with 2 triangles */
    const uint16_t indices[] = {0, 1, 2, 0, 2, 3};

    /* bindings */
    display.bind = (sg_bindings){
        .vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
            .data = SG_RANGE(vertices),
        }),
        .index_buffer = sg_make_buffer(&(sg_buffer_desc){
            .type = SG_BUFFERTYPE_INDEXBUFFER,
            .data = SG_RANGE(indices),
        }),
        .images[0] = sg_make_image(&(sg_image_desc){
            .width = C8_SCREEN_WIDTH,
            .height = C8_SCREEN_HEIGHT,
            .pixel_format = SG_PIXELFORMAT_R8,
            .usage = SG_USAGE_STREAM,
        }),
        .samplers[0] = sg_make_sampler(&(sg_sampler_desc){
            .min_filter = SG_FILTER_NEAREST,
            .mag_filter = SG_FILTER_NEAREST,
            .wrap_u = SG_WRAP_REPEAT,
            .wrap_v = SG_WRAP_REPEAT,
        }),
    };

    c8_load();
}

static uint32_t _c8__translate_key(const sapp_keycode code)
{
    // clang-format off
    switch (code)
    {
    case SAPP_KEYCODE_RIGHT: return C8_INPUT_RIGHT;
    case SAPP_KEYCODE_LEFT: return C8_INPUT_LEFT;
    case SAPP_KEYCODE_UP: return C8_INPUT_UP;
    case SAPP_KEYCODE_DOWN: return C8_INPUT_DOWN;
    case SAPP_KEYCODE_Z: return C8_INPUT_A;
    case SAPP_KEYCODE_X: return C8_INPUT_B;
    case SAPP_KEYCODE_E: return C8_INPUT_START;
    case SAPP_KEYCODE_R: return C8_INPUT_SELECT;
    default: break;
    }
    return C8_INPUT_INVALID;
    // clang-format on
}

static void event(const sapp_event *e)
{
    // clang-format off
    switch (e->type)
    {
    case SAPP_EVENTTYPE_KEY_DOWN: c8_input_set(_c8__translate_key(e->key_code)); break;
    case SAPP_EVENTTYPE_KEY_UP: c8_input_clear(_c8__translate_key(e->key_code)); break;
    default: break;
    }
    // clang-format on
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
    float palette[48];
    for (int32_t i = 0; i < 48; ++i)
    {
        palette[i] = ((uint8_t *)pal.ptr)[i] / 255.0f;
    }

    /* query screen data. */
    const c8_range_t screen = c8_query_screen();

    /* update gpu resources */
    sg_update_image(display.bind.images[0],
                    &(sg_image_data){
                        .subimage[0][0] = {.ptr = screen.ptr, .size = screen.size},
                    });

    /* graphics pipeline */
    sg_begin_pass(&display.pass);
    sg_apply_pipeline(display.pip);
    sg_apply_bindings(&display.bind);
    sg_apply_uniforms(0, SG_RANGE_REF(palette));
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