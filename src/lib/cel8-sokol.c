#define CEL8_IMPLEMENTATION
#include "../cel8.h"

/* sokol */
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"

/* NOTE: 121 keycodes */
static struct { char *name; sapp_keycode keycode; } key_entries[] = {
  { "space", SAPP_KEYCODE_SPACE },
  { "'", SAPP_KEYCODE_APOSTROPHE },
  { ",", SAPP_KEYCODE_COMMA },
  { "-", SAPP_KEYCODE_MINUS },
  { ".", SAPP_KEYCODE_PERIOD },
  { "/", SAPP_KEYCODE_SLASH },
  { "0", SAPP_KEYCODE_0 },
  { "1", SAPP_KEYCODE_1 },
  { "2", SAPP_KEYCODE_2 },
  { "3", SAPP_KEYCODE_3 },
  { "4", SAPP_KEYCODE_4 },
  { "5", SAPP_KEYCODE_5 },
  { "6", SAPP_KEYCODE_6 },
  { "7", SAPP_KEYCODE_7 },
  { "8", SAPP_KEYCODE_8 },
  { "9", SAPP_KEYCODE_9 },
  { ";", SAPP_KEYCODE_SEMICOLON },
  { "=", SAPP_KEYCODE_EQUAL },
  { "a", SAPP_KEYCODE_A },
  { "b", SAPP_KEYCODE_B },
  { "c", SAPP_KEYCODE_C },
  { "d", SAPP_KEYCODE_D },
  { "e", SAPP_KEYCODE_E },
  { "f", SAPP_KEYCODE_F },
  { "g", SAPP_KEYCODE_G },
  { "h", SAPP_KEYCODE_H },
  { "i", SAPP_KEYCODE_I },
  { "j", SAPP_KEYCODE_J },
  { "k", SAPP_KEYCODE_K },
  { "l", SAPP_KEYCODE_L },
  { "m", SAPP_KEYCODE_M },
  { "n", SAPP_KEYCODE_N },
  { "o", SAPP_KEYCODE_O },
  { "p", SAPP_KEYCODE_P },
  { "q", SAPP_KEYCODE_Q },
  { "r", SAPP_KEYCODE_R },
  { "s", SAPP_KEYCODE_S },
  { "t", SAPP_KEYCODE_T },
  { "u", SAPP_KEYCODE_U },
  { "v", SAPP_KEYCODE_V },
  { "w", SAPP_KEYCODE_W },
  { "x", SAPP_KEYCODE_X },
  { "y", SAPP_KEYCODE_Y },
  { "z", SAPP_KEYCODE_Z },
  { "[", SAPP_KEYCODE_LEFT_BRACKET },
  { "\\", SAPP_KEYCODE_BACKSLASH },
  { "]", SAPP_KEYCODE_RIGHT_BRACKET },
  { "grave", SAPP_KEYCODE_GRAVE_ACCENT },
  { "world1", SAPP_KEYCODE_WORLD_1 },
  { "world2", SAPP_KEYCODE_WORLD_2 },
  { "esc", SAPP_KEYCODE_ESCAPE },
  { "enter", SAPP_KEYCODE_ENTER },
  { "tab", SAPP_KEYCODE_TAB },
  { "backspace", SAPP_KEYCODE_BACKSPACE },
  { "insert", SAPP_KEYCODE_INSERT },
  { "delete", SAPP_KEYCODE_DELETE },
  { "right", SAPP_KEYCODE_RIGHT },
  { "left", SAPP_KEYCODE_LEFT },
  { "down", SAPP_KEYCODE_DOWN },
  { "up", SAPP_KEYCODE_UP },
  { "pageup", SAPP_KEYCODE_PAGE_UP },
  { "pagedown", SAPP_KEYCODE_PAGE_DOWN },
  { "home", SAPP_KEYCODE_HOME },
  { "end", SAPP_KEYCODE_END },
  { "capslock", SAPP_KEYCODE_CAPS_LOCK },
  { "scrolllock", SAPP_KEYCODE_SCROLL_LOCK },
  { "numlock", SAPP_KEYCODE_NUM_LOCK },
  { "printscreen", SAPP_KEYCODE_PRINT_SCREEN },
  { "pause", SAPP_KEYCODE_PAUSE },
  { "f1", SAPP_KEYCODE_F1 },
  { "f2", SAPP_KEYCODE_F2 },
  { "f3", SAPP_KEYCODE_F3 },
  { "f4", SAPP_KEYCODE_F4 },
  { "f5", SAPP_KEYCODE_F5 },
  { "f6", SAPP_KEYCODE_F6 },
  { "f7", SAPP_KEYCODE_F7 },
  { "f8", SAPP_KEYCODE_F8 },
  { "f9", SAPP_KEYCODE_F9 },
  { "f10", SAPP_KEYCODE_F10 },
  { "f11", SAPP_KEYCODE_F11 },
  { "f12", SAPP_KEYCODE_F12 },
  { "f13", SAPP_KEYCODE_F13 },
  { "f14", SAPP_KEYCODE_F14 },
  { "f15", SAPP_KEYCODE_F15 },
  { "f16", SAPP_KEYCODE_F16 },
  { "f17", SAPP_KEYCODE_F17 },
  { "f18", SAPP_KEYCODE_F18 },
  { "f19", SAPP_KEYCODE_F19 },
  { "f20", SAPP_KEYCODE_F20 },
  { "f21", SAPP_KEYCODE_F21 },
  { "f22", SAPP_KEYCODE_F22 },
  { "f23", SAPP_KEYCODE_F23 },
  { "f24", SAPP_KEYCODE_F24 },
  { "f25", SAPP_KEYCODE_F25 },
  { "kp0", SAPP_KEYCODE_KP_0 },
  { "kp1", SAPP_KEYCODE_KP_1 },
  { "kp2", SAPP_KEYCODE_KP_2 },
  { "kp3", SAPP_KEYCODE_KP_3 },
  { "kp4", SAPP_KEYCODE_KP_4 },
  { "kp5", SAPP_KEYCODE_KP_5 },
  { "kp6", SAPP_KEYCODE_KP_6 },
  { "kp7", SAPP_KEYCODE_KP_7 },
  { "kp8", SAPP_KEYCODE_KP_8 },
  { "kp9", SAPP_KEYCODE_KP_9 },
  { "kp.", SAPP_KEYCODE_KP_DECIMAL },
  { "kp/", SAPP_KEYCODE_KP_DIVIDE },
  { "kp*", SAPP_KEYCODE_KP_MULTIPLY },
  { "kp-", SAPP_KEYCODE_KP_SUBTRACT },
  { "kp+", SAPP_KEYCODE_KP_ADD },
  { "kpenter", SAPP_KEYCODE_KP_ENTER },
  { "kp=", SAPP_KEYCODE_KP_EQUAL },
  { "lshift", SAPP_KEYCODE_LEFT_SHIFT },
  { "lctrl", SAPP_KEYCODE_LEFT_CONTROL },
  { "lalt", SAPP_KEYCODE_LEFT_ALT },
  { "lsuper", SAPP_KEYCODE_LEFT_SUPER },
  { "rshift", SAPP_KEYCODE_RIGHT_SHIFT },
  { "rctrl", SAPP_KEYCODE_RIGHT_CONTROL },
  { "ralt", SAPP_KEYCODE_RIGHT_ALT },
  { "rsuper", SAPP_KEYCODE_RIGHT_SUPER },
  { "menu", SAPP_KEYCODE_MENU },

  /* always last. */
  { "unknown", SAPP_KEYCODE_INVALID },
};

static const char *search_for_key(sapp_keycode keycode) {
  i32 i = 0;
  sapp_keycode entry = key_entries[i].keycode;
  while (entry != 0) {
    if (entry == keycode) {
      return key_entries[i].name;
    }
    entry = key_entries[++i].keycode;
  }
  return NULL;
}

static struct {
  sg_pass_action pass_action;
  sg_pipeline pip;
  sg_bindings bind;
} state;

static u8 screen[0x4000];

static void init(void) {
  /* setup sokol-gfx */
  sg_setup(&(sg_desc) {
    .context = sapp_sgcontext(),
  });

  /* a vertex buffer */
  const f32 vertices[] = {
    /* positions */ /* texture coord */
    -1.0f,  1.0f,   0.0f, 0.0f, /* top-left */
     1.0f,  1.0f,   1.0f, 0.0f, /* top-right */
     1.0f, -1.0f,   1.0f, 1.0f, /* bottom-right */
    -1.0f, -1.0f,   0.0f, 1.0f, /* bottom-left */
  };
  state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
    .data = SG_RANGE(vertices),
    .label = "quad-vertices",
  });

  /* an index buffer with 2 triangles */
  const u16 indices[] = { 0, 1, 2,  0, 2, 3 };
  state.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc) {
    .type = SG_BUFFERTYPE_INDEXBUFFER,
    .data = SG_RANGE(indices),
    .label = "quad-indices",
  });

  state.bind.fs_images[0] = sg_make_image(&(sg_image_desc) {
    .width = CEL8_SCREEN_WIDTH,
    .height = CEL8_SCREEN_HEIGHT,
    .pixel_format = SG_PIXELFORMAT_R8,
    .usage = SG_USAGE_STREAM,
    .label = "screen-texture"
  });

  /* load embedded shader stages */
  #include "../embed/vertex.vs.h"
  #include "../embed/fragment.fs.h"
  sg_shader shd = sg_make_shader(&(sg_shader_desc) {
    .vs.source = (const char *) &vertex_vs[0],
    .fs.source = (const char *) &fragment_fs[0],
    .fs.images = {
      [0] = { .name = "screen", .image_type = SG_IMAGETYPE_2D },
    },
    .fs.uniform_blocks = {
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
  });

  /* a pipeline state object */
  state.pip = sg_make_pipeline(&(sg_pipeline_desc) {
    .shader = shd,
    .index_type = SG_INDEXTYPE_UINT16,
    .layout = {
      .attrs = {
        [0].format = SG_VERTEXFORMAT_FLOAT2,
        [1].format = SG_VERTEXFORMAT_FLOAT2,
      },
    },
    .label = "quad-pipeline",
  });

  /* default pass action */
  state.pass_action = (sg_pass_action) {
    .colors[0] = {
      .action = SG_ACTION_CLEAR,
      .value= { 0.0f, 0.0f, 0.0f, 1.0f },
    },
  };
}

static void event(const sapp_event *e) {
  switch (e->type) {

    /* mouse */
    case SAPP_EVENTTYPE_MOUSE_DOWN:
      /* e->mouse_button */
      cel8_event(&(cel8_event_t) {
        .mouse = {
          .type = CEL8_EVENT_MOUSE_DOWN,
          .button = (i32) e->mouse_button,
          .x = (i32) e->mouse_x,
          .y = (i32) e->mouse_y,
        }
      });
      break;
    case SAPP_EVENTTYPE_MOUSE_UP:
      cel8_event(&(cel8_event_t) {
        .mouse = {
          .type = CEL8_EVENT_MOUSE_UP,
          .button = (i32) e->mouse_button,
          .x = (i32) e->mouse_x,
          .y = (i32) e->mouse_y,
        }
      });
      break;
    case SAPP_EVENTTYPE_MOUSE_MOVE:
      cel8_event(&(cel8_event_t) {
        .mouse = {
          .type = CEL8_EVENT_MOUSE_MOVE,
          .x = (i32) e->mouse_x,
          .y = (i32) e->mouse_y,
          .dx = (i32) e->mouse_dx,
          .dy = (i32) e->mouse_dy,
        }
      });
      break;
    case SAPP_EVENTTYPE_MOUSE_SCROLL:
      cel8_event(&(cel8_event_t) {
        .mouse = {
          .type = CEL8_EVENT_MOUSE_SCROLL,
          .sx = (i32) e->scroll_x,
          .sy = (i32) e->scroll_y,
        }
      });
      break;

    /* keyboard */
    case SAPP_EVENTTYPE_KEY_DOWN:
      cel8_event(&(cel8_event_t) {
        .keyboard = {
          .type = CEL8_EVENT_KEY_DOWN,
          .key = search_for_key(e->key_code),
          .repeat = e->key_repeat,
        }
      });
      break;
    case SAPP_EVENTTYPE_KEY_UP:
      cel8_event(&(cel8_event_t) {
        .keyboard = {
          .type = CEL8_EVENT_KEY_UP,
          .key = search_for_key(e->key_code),
          .repeat = e->key_repeat,
        }
      });
      break;

    /* default */
    default:
      break;
  }
}

static void frame(void) {
  cel8_frame();

  /* update screen data */
  const cel8_range vram = cel8_query_vram();
  const cel8_range font = cel8_query_font();

  i32 i = 0, j = 0;
  for(; i < sizeof(screen); i+=8) {
    /* convert from screen to cell */
    j = ((i % 128) / 8) + 16 * (i / 1024);

    /* screen buffer */
    u8 color = *((u8 *) vram.data + (j*2)+0);
    u8 glyph = *((u8 *) vram.data + (j*2)+1);

    /* convert color */
    u8 h = ((color >> 4) & 0x0F);
    u8 l = ((color)      & 0x0F);

    i32 x = 0; i32 y = (i / 128) % 8;
    for(; x < 8; x++) {
      u8 b = *((u8 *) font.data + y + glyph * 8) >> x;
      *((u8 *) screen + i + x) = (b & 1) ? l : h;
    }
  }

  /* update screen in gpu */
  sg_update_image(state.bind.fs_images[0], &(sg_image_data) {
    .subimage[0][0] = (sg_range) {
      .ptr = screen,
      .size = sizeof(screen),
    }
  });

  /* graphics pipeline */
  sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);

  float palette[48] = {};
  const cel8_range pal = cel8_query_pal();

  i = 0;
  for(; i < 48; i+=3) {
    *(palette + i+0) = *((u8 *) pal.data + i+0) / 255.0f;
    *(palette + i+1) = *((u8 *) pal.data + i+1) / 255.0f;
    *(palette + i+2) = *((u8 *) pal.data + i+2) / 255.0f;
  }

  sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, SG_RANGE_REF(palette));

  sg_draw(0, 6, 1);
  sg_end_pass();
  sg_commit();
}

static void cleanup(void) {
  sg_shutdown();
  cel8_cleanup();
}

sapp_desc sokol_main(i32 argc, char* argv[]) {
  /* cel8 */
  cel8_init(argc, argv);

  /* sokol */
  return (sapp_desc){ 
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .event_cb = event,
    .width = CEL8_WINDOW_WIDTH,
    .height = CEL8_WINDOW_HEIGHT,
    .window_title = "cel8",

#if defined(_CEL8_DEBUG)
    .win32_console_create = true,
#endif
  };
}
