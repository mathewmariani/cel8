#pragma once
/*
    cel8.h -- a *tiny* framework for making grid-based games.

    Project URL: https://github.com/mathewmariani/cel8

    Do this:
        #define C8_IMPL
    before you include this file in *one* C or C++ file to create the
    implementation.

    FUNCTIONS:
    ==========

    c8_peek( addr [, n] )               : reads n 8-bit values from memory
    c8_peek2( addr [, n] )              : reads n 16-bit values from memory
    c8_peek4( addr [, n] )              : reads n 32-bit values from memory
    c8_poke( addr [, value [, ...]] )   : writes one or more 8-bit values to memory
    c8_poke2( addr [, value [, ...]] )  : writes one or more 16-bit values to memory
    c8_poke4( addr [, value [, ...]] )  : writes one or more 32-bit values to memory
    c8_memcpy( dst, src, len )          : copies a block of memory to another block
    c8_memset( dst, value, len )        : sets a block of memory to a specified value

    c8_cls( clr, chr )                  : clears the screen to a specified color and char
    c8_color( clr )                     : sets the draw color
    c8_fill( x, y, w, h, chr )          : sets the characters in the rectangle to specified char
    c8_print( x, y, str )               : places text at (x, y)
    c8_put( x, y, c )                   : puts a char at (x, y)
    c8_get( x, y )                      : current environment
    c8_stat( n )                        : system information
    c8_rnd( )                           : pseudo random number


    MEMORY MAP:
    ===========

    0x0000                           : font
    0x0400                           : drawstate
    0x0440                           : hardware state
    0x0450                           : vram
    0x0650                           : screen


    MEMORY DUMP:
    ============

    [0x0000] : font atlas (128*8 bytes)

      binary representation of the glyph `!`

      0b00111000 ~> 0x38
      0b00111000 ~> 0x38
      0b00110000 ~> 0x30
      0b01110000 ~> 0x70
      0b01100000 ~> 0x60
      0b00000000 ~> 0x00
      0b11000000 ~> 0xc0
      0b11000000 ~> 0xc0

    +-------------------------------- glyph 1
    |                       +-------- glyph 2
    |                       |
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 18 3C 3C 18 18 00 18 00
    36 36 00 00 00 00 00 00 36 36 7F 36 7F 36 36 00
    0C 3E 03 1E 30 1F 0C 00 00 63 33 18 0C 66 63 00
    1C 36 1C 6E 3B 33 6E 00 06 06 03 00 00 00 00 00
    18 0C 06 06 06 0C 18 00 06 0C 18 18 18 0C 06 00
    00 66 3C FF 3C 66 00 00 00 0C 0C 3F 0C 0C 00 00
    00 00 00 00 00 0C 0C 06 00 00 00 3F 00 00 00 00
    00 00 00 00 00 0C 0C 00 60 30 18 0C 06 03 01 00
    3E 63 73 7B 6F 67 3E 00 0C 0E 0C 0C 0C 0C 3F 00
    1E 33 30 1C 06 33 3F 00 1E 33 30 1C 30 33 1E 00
    38 3C 36 33 7F 30 78 00 3F 03 1F 30 30 33 1E 00
    1C 06 03 1F 33 33 1E 00 3F 33 30 18 0C 0C 0C 00
    1E 33 33 1E 33 33 1E 00 1E 33 33 3E 30 18 0E 00
    00 0C 0C 00 00 0C 0C 00 00 0C 0C 00 00 0C 0C 06
    18 0C 06 03 06 0C 18 00 00 00 3F 00 00 3F 00 00
    06 0C 18 30 18 0C 06 00 1E 33 30 18 0C 00 0C 00
    3E 63 7B 7B 7B 03 1E 00 0C 1E 33 33 3F 33 33 00
    3F 66 66 3E 66 66 3F 00 3C 66 03 03 03 66 3C 00
    1F 36 66 66 66 36 1F 00 7F 46 16 1E 16 46 7F 00
    7F 46 16 1E 16 06 0F 00 3C 66 03 03 73 66 7C 00
    33 33 33 3F 33 33 33 00 1E 0C 0C 0C 0C 0C 1E 00
    78 30 30 30 33 33 1E 00 67 66 36 1E 36 66 67 00
    0F 06 06 06 46 66 7F 00 63 77 7F 7F 6B 63 63 00
    63 67 6F 7B 73 63 63 00 1C 36 63 63 63 36 1C 00
    3F 66 66 3E 06 06 0F 00 1E 33 33 33 3B 1E 38 00
    3F 66 66 3E 36 66 67 00 1E 33 07 0E 38 33 1E 00
    3F 2D 0C 0C 0C 0C 1E 00 33 33 33 33 33 33 3F 00
    33 33 33 33 33 1E 0C 00 63 63 63 6B 7F 77 63 00
    63 63 36 1C 1C 36 63 00 33 33 33 1E 0C 0C 1E 00
    7F 63 31 18 4C 66 7F 00 1E 06 06 06 06 06 1E 00
    03 06 0C 18 30 60 40 00 1E 18 18 18 18 18 1E 00
    08 1C 36 63 00 00 00 00 00 00 00 00 00 00 00 FF
    0C 0C 18 00 00 00 00 00 00 00 1E 30 3E 33 6E 00
    07 06 06 3E 66 66 3B 00 00 00 1E 33 03 33 1E 00
    38 30 30 3e 33 33 6E 00 00 00 1E 33 3f 03 1E 00
    1C 36 06 0f 06 06 0F 00 00 00 6E 33 33 3E 30 1F
    07 06 36 6E 66 66 67 00 0C 00 0E 0C 0C 0C 1E 00
    30 00 30 30 30 33 33 1E 07 06 66 36 1E 36 67 00
    0E 0C 0C 0C 0C 0C 1E 00 00 00 33 7F 7F 6B 63 00
    00 00 1F 33 33 33 33 00 00 00 1E 33 33 33 1E 00
    00 00 3B 66 66 3E 06 0F 00 00 6E 33 33 3E 30 78
    00 00 3B 6E 66 06 0F 00 00 00 3E 03 1E 30 1F 00
    08 0C 3E 0C 0C 2C 18 00 00 00 33 33 33 33 6E 00
    00 00 33 33 33 1E 0C 00 00 00 63 6B 7F 7F 36 00
    00 00 63 36 1C 36 63 00 00 00 33 33 33 3E 30 1F
    00 00 3F 19 0C 26 3F 00 38 0C 0C 07 0C 0C 38 00
    18 18 18 00 18 18 18 00 07 0C 0C 38 0C 0C 07 00
    6E 3B 00 00 00 00 00 00 00 00 00 00 00 00 00 00


    [0x0400] : draw state (80 bytes)

    +-------------------------------- color index
    |
    00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f

    +-------------------------------- r-channel
    |  +----------------------------- g-channel
    |  |  +-------------------------- b-channel
    |  |  |
    00 00 00 1D 2B 53 7E 25 53 00 87 51 AB 52 36 5F
    57 4F C2 C3 C7 FF F1 E8 FF 00 4D FF A3 00 FF EC
    27 00 E4 36 29 AD FF 83 76 9C FF 77 A8 FF CC AA

    +-------------------------------- current background
    |+------------------------------- current foreground
    || +----------------------------- unused
    || |
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00


    [0x003F] : hardware state (16 bytes)

    +-------------------------------- input
    |     +-------------------------- random state 1
    |     |  +----------------------- random state 2
    |     |  |  +-------------------- unused
    |     |  |  |
    |     |  |  |
    |     |  |  |
    |     |  |  |
    00 01 02 00 00 00 00 00 00 00 00 00 00 00 00 00


    [0x0450] : vram (16*16*2 bytes)

    +-------------------------------- background
    |+------------------------------- foreground
    || +----------------------------- char glyph
    || |
    27 4b 92 70 2f 18 6a 21 d3 5b 63 2a 09 34 3e 07
    af 27 d0 41 1a 1d 7f 63 62 4e 58 60 ce 51 a8 58
    e1 59 80 26 62 5e f7 0b 69 1b 23 33 04 1f 86 35
    cf 5d 61 75 47 14 99 43 ab 52 4f 5c 7b 12 1e 57
    fc 49 54 60 40 28 dc 06 7e 2f 49 49 07 0a 65 4b
    6e 67 c7 7c a2 67 55 42 7d 04 ff 03 9d 62 ed 71
    fe 33 d4 14 12 22 03 21 c0 41 59 0e fc 62 e1 60
    89 4c 4c 5b ec 20 4d 0c 83 08 4b 66 95 5d 8c 78
    48 00 8e 76 32 27 02 35 94 04 35 2b 2b 5c da 79
    e6 3d 41 4f 99 14 ca 69 1d 47 aa 65 7a 29 80 44
    8d 00 94 22 a8 73 40 76 28 3a f5 7e 14 19 c8 46
    d4 27 65 0d c8 7e ca 3d f7 7e fb 0a 9c 6c 28 66
    82 80 8b 24 00 01 53 3b bc 07 d5 14 c7 5a 98 4e
    08 0b b6 53 66 08 f0 60 2c 37 f1 0a 9a 35 32 4b
    cc 7d c9 2f a5 49 f0 40 24 44 e8 60 17 34 cd 7f
    94 35 16 6a 45 78 7d 43 02 0a 13 75 a0 00 a4 75
    e1 75 13 38 13 7a a3 7b f0 69 86 10 35 38 25 51
    4a 19 c5 1b e2 7d 5c 31 52 3d ce 43 45 35 7c 02
    cc 5c a6 40 24 45 da 3f 23 19 2f 00 66 6a b0 3a
    80 0d f6 2d 52 58 17 71 73 02 cc 80 cc 08 53 2a
    24 20 3f 19 72 07 a0 2f b4 5c 6b 33 a9 6a 0e 1c
    12 3a 6d 4e 7f 6a a0 29 d1 66 3e 5f b5 23 0e 80
    f7 41 81 4b 1f 0a ff 54 c4 07 98 1c 89 70 7e 5e
    4c 50 26 6b 0d 2a f0 37 61 51 54 10 a0 24 bf 66
    16 73 eb 60 d5 52 78 54 2a 15 a0 5b d8 6b b7 18
    6d 32 b7 6f 4f 17 ff 70 02 19 34 4a 8b 31 13 1e
    db 79 7a 1a d8 0e 89 74 a7 2b cd 0e b5 1b f3 2e
    db 08 50 57 3d 46 ba 68 f2 6f dc 3a 44 66 72 55
    6e 36 7b 66 d4 25 85 70 29 2b 7d 1d 4b 43 27 1e
    26 2b de 63 a6 56 dc 77 1d 5f c4 07 37 0c 75 0a
    26 4f 8d 28 0e 19 6e 0a ce 52 16 54 86 01 30 3f
    ed 46 73 27 34 29 b7 73 ff 10 d2 58 23 0f 2a 32
*/

#include <stddef.h> /* size_t */
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(C8_API_DECL)
#if defined(_WIN32) && defined(C8_DLL) && defined(C8_IMPLEMENTATION)
#define C8_API_DECL __declspec(dllexport)
#elif defined(_WIN32) && defined(C8_DLL)
#define C8_API_DECL __declspec(dllimport)
#else
#define C8_STANDALONE (1)
#define C8_API_DECL extern
#endif
#endif

  /* compile time constants */
  enum
  {
    /* virtual resolution */
    C8_WINDOW_WIDTH = 512,
    C8_WINDOW_HEIGHT = 512,

    /* internal resolution */
    C8_SCREEN_WIDTH = 128,
    C8_SCREEN_HEIGHT = 128,

    /* input */
    C8_INPUT_INVALID = (0 << 0),
    C8_INPUT_UP = (1 << 0),
    C8_INPUT_LEFT = (1 << 1),
    C8_INPUT_RIGHT = (1 << 2),
    C8_INPUT_DOWN = (1 << 3),
    C8_INPUT_A = (1 << 4),
    C8_INPUT_B = (1 << 5),
    C8_INPUT_START = (1 << 6),
    C8_INPUT_SELECT = (1 << 7),

    /* memory addresses */
    C8_MEM_FONT_ADDR = 0x0000,
    C8_MEM_FONT_SIZE = 0x0400,
    C8_MEM_DRAWSTATE_ADDR = 0x0400,
    C8_MEM_DRAWSTATE_SIZE = 0x0040,
    C8_MEM_HARDWARE_ADDR = 0x0440,
    C8_MEM_HARDWARE_SIZE = 0x0010,
    C8_MEM_VRAM_ADDR = 0x0450,
    C8_MEM_VRAM_SIZE = 0x0200,
    C8_MEM_SCREEN_ADDR = 0x0650,
    C8_MEM_SCREEN_SIZE = 0x4000,
    C8_MEM_SIZE = C8_MEM_FONT_SIZE + C8_MEM_DRAWSTATE_SIZE + C8_MEM_VRAM_SIZE + C8_MEM_SCREEN_SIZE,

    /* drawstate specific */
    C8_MEM_CMAP_ADDR = C8_MEM_DRAWSTATE_ADDR + 0x0000,
    C8_MEM_CMAP_SIZE = 0x000F,
    C8_MEM_PAL_ADDR = C8_MEM_DRAWSTATE_ADDR + 0x000F,
    C8_MEM_PAL_SIZE = 0x0030,
    C8_MEM_COLOR_ADDR = C8_MEM_DRAWSTATE_ADDR + 0x003F,
    C8_MEM_COLOR_SIZE = 0x0001,

    /* hardware specific */
    C8_MEM_IO_ADDR = C8_MEM_HARDWARE_ADDR + 0x0000,
    C8_MEM_IO_SIZE = 0x0002,
    C8_MEM_RND_ADDR = C8_MEM_HARDWARE_ADDR + 0x0002,
    C8_MEM_RND_SIZE = 0x0004,

    /* stat */
    C8_STAT_VERSION_STR = 0x0000,
    C8_STAT_FRAME_TIME = 0x0001,
    C8_STAT_CURSOR_X = 0x0002,
    C8_STAT_CURSOR_Y = 0x0003,
    C8_STAT_GMT_YEAR = 0x0004,
    C8_STAT_GMT_MONTH = 0x0005,
    C8_STAT_GMT_DAY = 0x0006,
    C8_STAT_GMT_HOUR = 0x0007,
    C8_STAT_GMT_MIN = 0x0008,
    C8_STAT_GMT_SEC = 0x0009,
    C8_STAT_LOCAL_YEAR = 0x000a,
    C8_STAT_LOCAL_MONTH = 0x000b,
    C8_STAT_LOCAL_DAY = 0x000c,
    C8_STAT_LOCAL_HOUR = 0x000d,
    C8_STAT_LOCAL_MIN = 0x000e,
    C8_STAT_LOCAL_SEC = 0x000f,
    C8_STAT_FLAGS = 0x0010,
  };

  typedef struct c8_range_t
  {
    void *ptr;
    size_t size;
  } c8_range_t;

  typedef struct
  {
    struct
    {
      c8_range_t chars;
      c8_range_t palette;
    } roms;
  } c8_desc_t;

  /* main chip functionality */
  C8_API_DECL void c8_init(const c8_desc_t *desc);
  C8_API_DECL void c8_shutdown();
  C8_API_DECL void c8_reset(void);
  C8_API_DECL void c8_exec(void);
  C8_API_DECL void c8_input_set(uint32_t mask);
  C8_API_DECL void c8_input_clear(uint32_t mask);

  /* cel8 api */
  C8_API_DECL const uint8_t c8_peek(const uint32_t addr, const uint32_t index);
  C8_API_DECL const uint16_t c8_peek2(const uint32_t addr, const uint32_t index);
  C8_API_DECL const uint32_t c8_peek4(const uint32_t addr, const uint32_t index);
  C8_API_DECL void c8_poke(const uint32_t addr, const uint8_t value);
  C8_API_DECL void c8_poke2(const uint32_t addr, const uint16_t value);
  C8_API_DECL void c8_poke4(const uint32_t addr, const uint32_t value);
  C8_API_DECL void c8_memcpy(void *dst, const void *src, size_t len);
  C8_API_DECL void c8_memset(void *dst, int value, size_t len);

  C8_API_DECL bool c8_btn(uint32_t mask);
  C8_API_DECL void c8_cls(uint8_t clr, uint8_t chr);
  C8_API_DECL void c8_color(uint8_t color);
  C8_API_DECL void c8_fill(int32_t x, int32_t y, int32_t w, int32_t h, int32_t chr);
  C8_API_DECL void c8_print(int32_t x, int32_t y, const char *str);
  C8_API_DECL void c8_put(int32_t x, int32_t y, uint8_t c);
  C8_API_DECL uint8_t c8_get(int32_t x, int32_t y);
  C8_API_DECL uint16_t c8_stat(int32_t n);
  C8_API_DECL uint16_t c8_rnd(void);

  C8_API_DECL c8_range_t c8_query_memory(void);
  C8_API_DECL c8_range_t c8_query_vram(void);
  C8_API_DECL c8_range_t c8_query_screen(void);
  C8_API_DECL c8_range_t c8_query_font(void);
  C8_API_DECL c8_range_t c8_query_color(void);
  C8_API_DECL c8_range_t c8_query_pal(void);
  C8_API_DECL c8_range_t c8_query_rnd(void);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef C8_IMPL

/* debug */
#if defined(_DEBUG)
#define _C8_DEBUG 1
#endif

/* version */
#define C8_VERSION_MAJOR 0
#define C8_VERSION_MINOR 0
#define C8_VERSION_REVISION 1
#define C8_RELEASE_CANDIDATE 0

#define C8_VERSION_NUMBER     \
  ((C8_VERSION_MAJOR << 16) | \
   (C8_VERSION_MINOR << 8) |  \
   (C8_VERSION_REVISION))

#if defined(_C8_DEBUG)
#define _CONCAT_VERSION(m, n, r) \
  #m "." #n "." #r "-debug"
#else
#define _CONCAT_VERSION(m, n, r) \
  #m "." #n "." #r
#endif

#define _MAKE_VERSION(m, n, r) \
  _CONCAT_VERSION(m, n, r)

#define C8_VERSION_STRING \
  _MAKE_VERSION(C8_VERSION_MAJOR, C8_VERSION_MINOR, C8_VERSION_REVISION)

#include <string.h> /* memcpy, memset */
#include <time.h>

#ifndef C8_ASSERT
#include <assert.h>
#define C8_ASSERT(c) assert(c)
#endif

#ifndef _C8_PRIVATE
#if defined(__GNUC__) || defined(__clang__)
#define _C8_PRIVATE __attribute__((unused)) static
#else
#define _C8_PRIVATE static
#endif
#endif

#if defined(_C8_SIMD)
#include <emmintrin.h>
#endif

/* stubbed */
#if defined(_C8_DEBUG)
#include <stdio.h>
#define STUBBED(x) \
  printf("STUBBED: %s at %s (%s:%d)\n", x, __FUNCTION__, __FILE__, __LINE__);
#else
#define STUBBED(x)
#endif

static struct
{
  bool valid;
  struct
  {
    uint8_t font[C8_MEM_FONT_SIZE];
    uint8_t drawstate[C8_MEM_DRAWSTATE_SIZE];
    uint8_t hardware[C8_MEM_DRAWSTATE_SIZE];
    uint8_t vram[C8_MEM_VRAM_SIZE];
    uint8_t screen[C8_MEM_SCREEN_SIZE];
  } memory;
} _c8;

_C8_PRIVATE inline bool _c8__should_clip(uint8_t x, uint8_t y)
{
  return (x < 0 || x >= 0x10 || y < 0 || y >= 0x10);
}

_C8_PRIVATE inline void _c8__put_char(uint32_t x, uint32_t y, uint8_t glyph)
{
  if (_c8__should_clip(x, y))
  {
    return;
  }

  /* FIXME: magic number */
  const uint32_t offset = (x * 2) + 0x20 * y;
  const uint8_t color = c8_peek(C8_MEM_COLOR_ADDR, 0);
  c8_poke(C8_MEM_VRAM_ADDR + offset + 0, color);
  c8_poke(C8_MEM_VRAM_ADDR + offset + 1, glyph);
}

/* memory */

void c8_init(const c8_desc_t *desc)
{
  _c8.valid = true;

  /* initialize default font and palette */
  memcpy((uint8_t *)&_c8.memory + C8_MEM_FONT_ADDR, desc->roms.chars.ptr, desc->roms.chars.size);
  memcpy((uint8_t *)&_c8.memory + C8_MEM_PAL_ADDR, desc->roms.palette.ptr, desc->roms.palette.size);
}

void c8_shutdown(void)
{
  C8_ASSERT(_c8.valid);
}

void c8_reset(void)
{
  C8_ASSERT(_c8.valid);
}

void c8_input_set(uint32_t mask)
{
  C8_ASSERT(_c8.valid);
  _c8.memory.hardware[0] |= mask;
}

void c8_input_clear(uint32_t mask)
{
  C8_ASSERT(_c8.valid);
  _c8.memory.hardware[0] &= ~mask;
}

_C8_PRIVATE void _c8__decode_video(void)
{
  /* query memory */
  const c8_range_t vram = c8_query_vram();
  const c8_range_t font = c8_query_font();
  const c8_range_t screen = c8_query_screen();

  uint8_t *vram_ptr = (uint8_t *)vram.ptr;
  uint8_t *font_ptr = (uint8_t *)font.ptr;
  uint8_t *screen_ptr = (uint8_t *)screen.ptr;

  for (int32_t i = 0; i < C8_MEM_SCREEN_SIZE; i += 8)
  {
    /* convert from screen to cell */
    int32_t j = ((i % 128) / 8) + 16 * (i / 1024);

    /* screen buffer */
    uint8_t color = *(vram_ptr + (j * 2));
    uint8_t glyph = *(vram_ptr + (j * 2 + 1));

    /* convert color */
    uint8_t high = (color >> 4) & 0x0F;
    uint8_t low = color & 0x0F;

    /* decode glyph */
    int32_t y = (i / 128) % 8;
    uint8_t *glyph_row = font_ptr + (glyph * 8) + y;

#if defined(_C8_SIMD)
    __m128i glyph_bits = _mm_setr_epi8(
        (glyph_row & 0x80) >> 7, (glyph_row & 0x40) >> 6,
        (glyph_row & 0x20) >> 5, (glyph_row & 0x10) >> 4,
        (glyph_row & 0x08) >> 3, (glyph_row & 0x04) >> 2,
        (glyph_row & 0x02) >> 1, (glyph_row & 0x01) >> 0,
        0, 0, 0, 0, 0, 0, 0, 0);

    __m128i result = _mm_or_si128(
        _mm_and_si128(glyph_bits, _mm_set1_epi8(low)),
        _mm_andnot_si128(glyph_bits, _mm_set1_epi8(high)));

    _mm_storel_epi64((__m128i *)(screen_ptr + i), result);
#else
    for (int32_t x = 0; x < 8; x++)
    {
      *(screen_ptr + (i + x)) = ((*glyph_row >> x) & 1) ? low : high;
    }
#endif
  }
}

void c8_exec(void)
{
  C8_ASSERT(_c8.valid);
  _c8__decode_video();
}

const uint8_t c8_peek(const uint32_t addr, const uint32_t index)
{
  return *((uint8_t *)&_c8.memory + addr + index);
}

const uint16_t c8_peek2(const uint32_t addr, const uint32_t index)
{
  const uint8_t b0 = c8_peek(addr, index + 0);
  const uint8_t b1 = c8_peek(addr, index + 1);

  /* combine 16-bit value */
  return ((b0 << 8) | (b1 << 0));
}

const uint32_t c8_peek4(const uint32_t addr, const uint32_t index)
{
  const uint8_t b0 = c8_peek(addr, index + 0);
  const uint8_t b1 = c8_peek(addr, index + 1);
  const uint8_t b2 = c8_peek(addr, index + 2);
  const uint8_t b3 = c8_peek(addr, index + 3);

  /* combine 32-bit value */
  return ((b0 << 24) | (b1 << 16) | (b2 << 8) | (b3 << 0));
}

void c8_poke(const uint32_t addr, const uint8_t value)
{
  *((uint8_t *)&_c8.memory + addr) = value;
}

void c8_poke2(const uint32_t addr, const uint16_t value)
{
  c8_poke(addr + 0, (value >> 8) & 0xff);
  c8_poke(addr + 1, (value >> 0) & 0xff);
}

void c8_poke4(const uint32_t addr, const uint32_t value)
{
  c8_poke(addr + 0, (value >> 24) & 0xff);
  c8_poke(addr + 1, (value >> 16) & 0xff);
  c8_poke(addr + 2, (value >> 8) & 0xff);
  c8_poke(addr + 3, (value >> 0) & 0xff);
}

void c8_memcpy(void *dst, const void *src, size_t len)
{
  memcpy(dst, src, len);
}

void c8_memset(void *dst, int value, size_t len)
{
  memset(dst, value, len);
}

bool c8_btn(uint32_t mask)
{
  return (_c8.memory.hardware[0] & mask) == mask;
}

void c8_cls(uint8_t clr, uint8_t chr)
{
  c8_memset((uint8_t *)&_c8.memory + C8_MEM_VRAM_ADDR, ((clr << 4) | chr), C8_MEM_VRAM_SIZE);
}

void c8_color(uint8_t color)
{
  c8_poke(C8_MEM_COLOR_ADDR, color);
}

void c8_fill(int32_t x, int32_t y, int32_t w, int32_t h, int32_t chr)
{
  for (int32_t i = y; i < (y + h); i++)
  {
    for (int32_t j = x; j < (x + w); j++)
    {
      _c8__put_char(j, i, chr);
    }
  }
}

void c8_print(int32_t x, int32_t y, const char *str)
{
  int32_t i = 0;
  while (*str)
  {
    _c8__put_char(x + (i++), y, *(str++));
  }
}

void c8_put(int32_t x, int32_t y, uint8_t c)
{
  _c8__put_char(x, y, c);
}

uint8_t c8_get(int32_t x, int32_t y)
{
  /* boundary check */
  if (_c8__should_clip(x, y))
  {
    return 0;
  }

  /* FIXME: magic numbers (0x20) */
  const uint32_t offset = (x * 2) + 0x20 * y;
  return c8_peek(offset, 0);
}

uint16_t c8_stat(int32_t n)
{
  switch (n)
  {
  case C8_STAT_VERSION_STR:
  {
    /* TODO: implementation. */
    return 0;
  }
  case C8_STAT_FRAME_TIME:
  {
    /* TODO: implementation. */
    return 0;
  }
  case C8_STAT_CURSOR_X:
  {
    /* TODO: implementation. */
    return 0;
  }
  case C8_STAT_CURSOR_Y:
  {
    /* TODO: implementation. */
    return 0;
  }
  case C8_STAT_GMT_YEAR:
  {
    time_t t = time(NULL);
    return gmtime(&t)->tm_year + 1900;
  }
  case C8_STAT_GMT_MONTH:
  {
    time_t t = time(NULL);
    return gmtime(&t)->tm_mon + 1;
  }
  case C8_STAT_GMT_DAY:
  {
    time_t t = time(NULL);
    return gmtime(&t)->tm_mday;
  }
  case C8_STAT_GMT_HOUR:
  {
    time_t t = time(NULL);
    return gmtime(&t)->tm_hour;
  }
  case C8_STAT_GMT_MIN:
  {
    time_t t = time(NULL);
    return gmtime(&t)->tm_min;
  }
  case C8_STAT_GMT_SEC:
  {
    time_t t = time(NULL);
    return gmtime(&t)->tm_sec;
  }
  case C8_STAT_LOCAL_YEAR:
  {
    time_t t = time(NULL);
    return localtime(&t)->tm_year + 1900;
  }
  case C8_STAT_LOCAL_MONTH:
  {
    time_t t = time(NULL);
    return localtime(&t)->tm_mon + 1;
  }
  case C8_STAT_LOCAL_DAY:
  {
    time_t t = time(NULL);
    return localtime(&t)->tm_mday;
  }
  case C8_STAT_LOCAL_HOUR:
  {
    time_t t = time(NULL);
    return localtime(&t)->tm_hour;
  }
  case C8_STAT_LOCAL_MIN:
  {
    time_t t = time(NULL);
    return localtime(&t)->tm_min;
  }
  case C8_STAT_LOCAL_SEC:
  {
    time_t t = time(NULL);
    return localtime(&t)->tm_sec;
  }
  default:
    return 0;
  }
}

uint16_t c8_rnd(void)
{
  for (int32_t i = 1; i >= 0; --i)
  {
    uint8_t reg_0 = c8_peek(C8_MEM_RND_ADDR, 0);
    uint8_t reg_1 = c8_peek(C8_MEM_RND_ADDR, 1);

    c8_poke(C8_MEM_RND_ADDR + 0, 5 * reg_0 + 1);
    c8_poke(C8_MEM_RND_ADDR + 1, ((reg_1 & 0x80) == (reg_1 & 0x10)) ? 2 * reg_1 + 1 : 2 * reg_1);
    c8_poke(C8_MEM_RND_ADDR + 2 + i, (reg_0 ^ reg_1));
  }

  uint8_t reg_2 = c8_peek(C8_MEM_RND_ADDR, 2);
  uint8_t reg_3 = c8_peek(C8_MEM_RND_ADDR, 3);
  return ((uint16_t)reg_2 << 0x8) | reg_3;
}

#define _C8_RANGE(addr, sz) \
  (c8_range_t){.ptr = (uint8_t *)&_c8.memory + addr, .size = sz};

c8_range_t c8_query_memory(void)
{
  return _C8_RANGE(0x0000, C8_MEM_SIZE);
}

c8_range_t c8_query_pal(void)
{
  return _C8_RANGE(C8_MEM_PAL_ADDR, C8_MEM_PAL_SIZE);
}

c8_range_t c8_query_color(void)
{
  return _C8_RANGE(C8_MEM_COLOR_ADDR, C8_MEM_COLOR_SIZE);
}

c8_range_t c8_query_rnd(void)
{
  return _C8_RANGE(C8_MEM_RND_ADDR, C8_MEM_RND_SIZE);
}

c8_range_t c8_query_font(void)
{
  return _C8_RANGE(C8_MEM_FONT_ADDR, C8_MEM_FONT_SIZE);
}

c8_range_t c8_query_vram(void)
{
  return _C8_RANGE(C8_MEM_VRAM_ADDR, C8_MEM_VRAM_SIZE);
}

c8_range_t c8_query_screen(void)
{
  return _C8_RANGE(C8_MEM_SCREEN_ADDR, C8_MEM_SCREEN_SIZE);
}

#undef _C8_RANGE

#endif /* C8_IMPL */