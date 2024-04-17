#if defined(CEL8_IMPL) && !defined(CEL8_IMPLEMENTATION)
#define CEL8_IMPLEMENTATION
#endif
#ifndef CEL8_INCLUDED
/*
    cel8.h -- a *tiny* framework for grid-based games in lua.

    Project URL: https://github.com/mathewmariani/cel8


    CALLBACKS:
    ==========

    load( )                          : called after initialization
    update( )                        : called every frame
    draw( )                          : called every frame
    keydown( key )                   : called when a key is pressed
    keyup( key )                     : called when a key is released


    FUNCTIONS:
    ==========
    
    peek( addr [, n] )               : reads n 8-bit values from memory
    peek2( addr [, n] )              : reads n 16-bit values from memory
    peek4( addr [, n] )              : reads n 32-bit values from memory
    poke( addr [, value [, ...]] )   : writes one or more 8-bit values to memory
    poke2( addr [, value [, ...]] )  : writes one or more 16-bit values to memory
    poke4( addr [, value [, ...]] )  : writes one or more 32-bit values to memory
    memcpy( dst, src, len )          : copies a block of memory to another block
    memset( dst, value, len )        : sets a block of memory to a specified value

    cls( clr, chr )                  : clears the screen to a specified color and char
    color( clr )                     : sets the draw color
    fill( x, y, w, h, chr )          : sets the characters in the rectangle to specified char
    put( x, y, str )                 : places text at (x, y)
    get( x, y )                      : current environment
    stat( n )                        : system information
    rnd( )                           : pseudo random number
    time( )                          : time since start

    band( x, y )                     : bitwise and
    bor( x, y )                      : bitwise or
    bxor( x, y )                     : bitwise xor
    bnot( x )                        : bitwise not
    shl( x, n )                      : arithmetic shift left
    shr( x, n )                      : arithmetic shift right
    lshr( x, n )                     : logical shift left
    rotl( x, n )                     : rotate left
    rotr( x, n )                     : rotate right


    MEMORY MAP:
    ===========

    0x0000                           : color mapping
    0x000F                           : color palette
    0x003F                           : draw color
    0x0040                           : random state
    0x0044                           : unused
    0x0050                           : font atlas
    0x0450                           : screen buffer


    MEMORY DUMP:
    ============

    [0x0000] : color mapping
    (16 bytes)

    +-------------------------------- color index
    |
    00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f


    [0x000F] : color palette
    (16*3 bytes)

    +-------------------------------- r-channel
    |  +----------------------------- g-channel
    |  |  +-------------------------- b-channel
    |  |  |
    00 00 00 1D 2B 53 7E 25 53 00 87 51 AB 52 36 5F
    57 4F C2 C3 C7 FF F1 E8 FF 00 4D FF A3 00 FF EC
    27 00 E4 36 29 AD FF 83 76 9C FF 77 A8 FF CC AA


    [0x003F] : draw color
    [0x0040] : random state
    [0x0044] : unused

    (16 bytes)

    +-------------------------------- current background
    |+------------------------------- current foreground
    || +----------------------------- random state 1
    || |  +-------------------------- random state 2
    || |  |  +----------------------- unused
    || |  |  |
    00 01 02 00 00 00 00 00 00 00 00 00 00 00 00 00


    [0x0050] : font atlas
    (128*8 bytes)

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

    [0x0450] : screen bufffer
    (16*16*2 bytes)

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
#define CEL8_INCLUDED

#include <stddef.h> /* size_t */
#include <stdint.h>
#include <stdbool.h>

#if !defined(CEL8_API_DECL)
  #if defined(_WIN32) && defined(CEL8_DLL) && defined(CEL8_IMPLEMENTATION)
    #define CEL8_API_DECL __declspec(dllexport)
  #elif defined(_WIN32) && defined(CEL8_DLL)
    #define CEL8_API_DECL __declspec(dllimport)
  #else
    #define CEL8_STANDALONE (1)
    #define CEL8_API_DECL extern
  #endif
#endif

/* floating point types */
typedef float    f32;
typedef double   f64;

/* integer types */
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;

/* compile time constants */
enum {
  /* virtual resolution */
  CEL8_WINDOW_WIDTH      = 512,
  CEL8_WINDOW_HEIGHT     = 512,

  /* internal resolution */
  CEL8_SCREEN_WIDTH      = 128,
  CEL8_SCREEN_HEIGHT     = 128,

  /* events */
  CEL8_EVENT_BUFFER_SIZE = 256,

  /* memory mapping */
  CEL8_CMAP_ADDR         = 0x0000,
  CEL8_CMAP_SIZE         = 0x000F,
  CEL8_PAL_ADDR          = 0x000F,
  CEL8_PAL_SIZE          = 0x0030,
  CEL8_COLOR_ADDR        = 0x003F,
  CEL8_COLOR_SIZE        = 0x0001,
  CEL8_RND_ADDR          = 0x0040,
  CEL8_RND_SIZE          = 0x0004,
  CEL8_UNUSED_ADDR       = 0x0044,
  CEL8_UNUSED_SIZE       = 0x000C,
  CEL8_FONT_ADDR         = 0x0050,
  CEL8_FONT_SIZE         = 0x0400,
  CEL8_VRAM_ADDR         = 0x0450,
  CEL8_VRAM_SIZE         = 0x0200,
  CEL8_MEM_SIZE          = CEL8_CMAP_SIZE
                         + CEL8_PAL_SIZE
                         + CEL8_COLOR_SIZE
                         + CEL8_RND_SIZE
                         + CEL8_UNUSED_SIZE
                         + CEL8_FONT_SIZE
                         + CEL8_VRAM_SIZE,

  /* stat */
  CEL8_VERSION_STR       = 0x0000,
  CEL8_FRAME_TIME        = 0x0001,
  CEL8_MOUSE_X           = 0x0002,
  CEL8_MOUSE_Y           = 0x0003,
  CEL8_GMT_YEAR          = 0x0004,
  CEL8_GMT_MONTH         = 0x0005,
  CEL8_GMT_DAY           = 0x0006,
  CEL8_GMT_HOUR          = 0x0007,
  CEL8_GMT_MIN           = 0x0008,
  CEL8_GMT_SEC           = 0x0009,
  CEL8_LOCAL_YEAR        = 0x000a,
  CEL8_LOCAL_MONTH       = 0x000b,
  CEL8_LOCAL_DAY         = 0x000c,
  CEL8_LOCAL_HOUR        = 0x000d,
  CEL8_LOCAL_MIN         = 0x000e,
  CEL8_LOCAL_SEC         = 0x000f,
};

typedef enum cel8_event_type {
  CEL8_EVENT_INVALID,

  /* keyboard */
  CEL8_EVENT_KEY_DOWN,
  CEL8_EVENT_KEY_UP,

  /* mouse */
  CEL8_EVENT_MOUSE_DOWN,
  CEL8_EVENT_MOUSE_UP,
  CEL8_EVENT_MOUSE_MOVE,
  CEL8_EVENT_MOUSE_SCROLL,

  /* always last. */
  _CEL8_EVENT_COUNT,
} cel8_event_type;

typedef union {
  i32 type;

  struct {
    i32 type;
    i32 status;
  } quit;

  struct {
    i32 type;
    i32 x, y;
    i32 dx, dy;
    i32 sx, sy;
    i32 button;
  } mouse;

  struct {
    i32 type;
    const char *key;
    bool repeat;
  } keyboard;
} cel8_event_t;

typedef struct cel8_range {
  void *data;
  size_t size;
} cel8_range;

/* cel8 api */
CEL8_API_DECL i32 cel8_init(i32 argc, char** argv);
CEL8_API_DECL void cel8_cleanup(void);
CEL8_API_DECL void cel8_frame(void);
CEL8_API_DECL void cel8_event(const cel8_event_t *e);

CEL8_API_DECL cel8_range cel8_query_memory(void);
CEL8_API_DECL cel8_range cel8_query_vram(void);
CEL8_API_DECL cel8_range cel8_query_font(void);
CEL8_API_DECL cel8_range cel8_query_color(void);
CEL8_API_DECL cel8_range cel8_query_pal(void);
CEL8_API_DECL cel8_range cel8_query_rnd(void);

#endif /* CEL8_INCLUDED */
#ifdef CEL8_IMPLEMENTATION
#define CEL8_IMPL_INCLUDED (1)

/* platform */
#if defined(WIN32) || defined(_WIN32)
  #define OS_WINDOWS 1
#endif
#if defined(__APPLE__)
  #include <TargetConditionals.h>
  #if defined(TARGET_OS_MAC)
    #define OS_MACOS 1
  #elif defined(TARGET_OS_IPHONE)
    #define OS_IOS 1
  #endif
#endif
#if defined(__linux__)
  #define OS_LINUX 1
#endif
#if defined(__FreeBSD__)
  #define OS_FREEBSD 1
#endif
#if defined(__EMSCRIPTEN__)
  #define OS_EMSCRIPTEN 1
#endif

/* check platform */
#if !defined(OS_WINDOWS) && !defined(OS_MACOS) && !defined(OS_IOS) && !defined(OS_LINUX) && !defined(OS_FREEBSD) && !defined(OS_EMSCRIPTEN)
  #error Could not detect target platform
#endif

/* debug */
#if defined(_DEBUG)
  #define _CEL8_DEBUG 1
#endif

/* version */
#define CEL8_VERSION_MAJOR      0
#define CEL8_VERSION_MINOR      0
#define CEL8_VERSION_REVISION   1
#define CEL8_RELEASE_CANDIDATE  0

#define CEL8_VERSION_NUMBER     \
  ((CEL8_VERSION_MAJOR << 16) | \
   (CEL8_VERSION_MINOR <<  8) | \
   (CEL8_VERSION_REVISION))

#if defined(_CEL8_DEBUG)
  #define _CONCAT_VERSION(m, n, r) \
    #m "." #n "." #r "-debug"
#else
  #define _CONCAT_VERSION(m, n, r) \
    #m "." #n "." #r
#endif

#define _MAKE_VERSION(m, n, r) \
  _CONCAT_VERSION(m, n, r)

#define CEL8_VERSION_STRING \
  _MAKE_VERSION(CEL8_VERSION_MAJOR, CEL8_VERSION_MINOR, CEL8_VERSION_REVISION)

#include <string.h>
#include <time.h>

#if defined(OS_WINDOWS)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
#elif defined(OS_MACOS)
  #include <unistd.h>
  #include <mach/mach_time.h>
#endif

#ifndef _CEL8_PRIVATE
 #if defined(__GNUC__) || defined(__clang__)
   #define _CEL8_PRIVATE __attribute__((unused)) static
 #else
   #define _CEL8_PRIVATE static
 #endif
#endif

/* stubbed */
#if defined (_CEL8_DEBUG)
  #include <stdio.h>
  #define STUBBED(x) \
    printf("STUBBED: %s at %s (%s:%d)\n", x, __FUNCTION__, __FILE__, __LINE__);
#else
  #define STUBBED(x)
#endif

/* utils */
#define UNUSED(x)      ((void)(x))
#define MAX(a, b)      ((a) > (b) ? (a) : (b))
#define MIN(a, b)      ((a) < (b) ? (a) : (b))
#define CLAMP(v, a, b) (MIN(MAX(v, a), b))
#define ABS(n)         ((n<0)?(-n):(n))
#define SWAP_INT(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

/* lua */
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

struct {
  /* events */
  cel8_event_t buffer[CEL8_EVENT_BUFFER_SIZE];
  u8 writei;
  u8 readi;

  /* mouse */
  i32 mouse_x;
  i32 mouse_y;

  /* timer */
  f64 curr;
  f64 start;
  f64 dt;

  u8 memory[CEL8_MEM_SIZE];
} cel8;

/* event */

_CEL8_PRIVATE i32 l_poll(lua_State *L) {
  cel8_event_t e;
  if (cel8.readi != cel8.writei) {
    e = cel8.buffer[cel8.readi++];

    switch (e.type) {
      /* keyboard */
      case CEL8_EVENT_KEY_DOWN:
        lua_pushstring(L, "keypressed");
        lua_pushstring(L, e.keyboard.key);
        return 2;
      case CEL8_EVENT_KEY_UP:
        lua_pushstring(L, "keyreleased");
        lua_pushstring(L, e.keyboard.key);
        return 2;

      /* mouse */
      case CEL8_EVENT_MOUSE_DOWN:
        lua_pushstring(L, "mousepressed");
        lua_pushnumber(L, e.mouse.x);
        lua_pushnumber(L, e.mouse.y);
        lua_pushnumber(L, e.mouse.button);
        return 4;
      case CEL8_EVENT_MOUSE_UP:
        lua_pushstring(L, "mousereleased");
        lua_pushnumber(L, e.mouse.x);
        lua_pushnumber(L, e.mouse.y);
        lua_pushnumber(L, e.mouse.button);
        return 4;
      case CEL8_EVENT_MOUSE_MOVE:
        /* store mouse position */
        cel8.mouse_x = e.mouse.x;
        cel8.mouse_y = e.mouse.y;

        lua_pushstring(L, "mousemoved");
        lua_pushnumber(L, e.mouse.x);
        lua_pushnumber(L, e.mouse.y);
        lua_pushnumber(L, e.mouse.dx);
        lua_pushnumber(L, e.mouse.dy);
        return 5;
      case CEL8_EVENT_MOUSE_SCROLL:
        lua_pushstring(L, "mousescroll");
        lua_pushnumber(L, e.mouse.sx);
        lua_pushnumber(L, e.mouse.sy);
        return 3;

      case CEL8_EVENT_INVALID:
      default:
        break;
    }
  }

  return 0;
}

/* memory */

_CEL8_PRIVATE inline const u8 cel8_peek(const u32 addr, const u32 index) {
  return *(cel8.memory + addr + index);
}

_CEL8_PRIVATE inline const u16 cel8_peek2(const u32 addr, const u32 index) {
  const u8 b0 = cel8_peek(addr, index+0);
  const u8 b1 = cel8_peek(addr, index+1);

  /* combine 16-bit value */
  return ((b0 << 8) | (b1 << 0));
}

_CEL8_PRIVATE inline const u32 cel8_peek4(const u32 addr, const u32 index) {
  const u8 b0 = cel8_peek(addr, index+0);
  const u8 b1 = cel8_peek(addr, index+1);
  const u8 b2 = cel8_peek(addr, index+2);
  const u8 b3 = cel8_peek(addr, index+3);

  /* combine 32-bit value */
  return ((b0 << 24) | (b1 << 16) | (b2 <<  8) | (b3 <<  0));
}

_CEL8_PRIVATE inline void cel8_poke(const u32 addr, const u32 index, const u8 value) {
  *(cel8.memory + addr + index) = value;
}

_CEL8_PRIVATE inline void cel8_poke2(const u32 addr, const u32 index, const u16 value) {
  cel8_poke(addr, index+0, (value >> 8) & 0xff);
  cel8_poke(addr, index+1, (value >> 0) & 0xff);
}

_CEL8_PRIVATE inline void cel8_poke4(const u32 addr, const u32 index, const u32 value) {
  cel8_poke(addr, index+0, (value >> 24) & 0xff);
  cel8_poke(addr, index+1, (value >> 16) & 0xff);
  cel8_poke(addr, index+2, (value >>  8) & 0xff);
  cel8_poke(addr, index+3, (value >>  0) & 0xff);
}

/* memory */

_CEL8_PRIVATE i32 l_peek(lua_State *L) {
  const i32 addr = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);

  if (n == 0) {
    n = 1;
  }

  i32 i = 0;
  for (; i < n; ++i) {
    /* TODO: implement bound-checking */
    lua_pushnumber(L, cel8_peek(addr, i));
  }

  return n;
}

_CEL8_PRIVATE i32 l_peek2(lua_State *L) {
  const i32 addr = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);

  if (n == 0) {
    n = 1;
  }

  i32 i = 0;
  for (; i < n; ++i) {
    /* TODO: implement bound-checking */
    lua_pushnumber(L, cel8_peek2(addr, i*2));
  }

  return n;
}

_CEL8_PRIVATE i32 l_peek4(lua_State *L) {
  const i32 addr = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);

  if (n == 0) {
    n = 1;
  }

  i32 i = 0;
  for (; i < n; ++i) {
    /* TODO: implement bound-checking */
    lua_pushnumber(L, cel8_peek4(addr, i*4));
  }

  return n;
}

_CEL8_PRIVATE i32 l_poke(lua_State *L) {
  const i32 addr = (i32) lua_tonumber(L, 1);
  const i32 n = lua_gettop(L) - 1;

  i32 i = 0;
  for (; i < n; i++) {
    /* TODO: implement bound-checking */
    const u8 value = (u8) lua_tonumber(L, 2 + i);
    cel8_poke(addr, i, value);
  }

  return 0;
}

_CEL8_PRIVATE i32 l_poke2(lua_State *L) {
  const i32 addr = (i32) lua_tonumber(L, 1);
  const i32 n = lua_gettop(L) - 1;

  i32 i = 0;
  for (; i < n; i++) {
    /* TODO: implement bound-checking */
    const u16 value = (u16) lua_tonumber(L, 2 + i);
    cel8_poke2(addr, (i*2), value);
  }

  return 0;
}

_CEL8_PRIVATE i32 l_poke4(lua_State *L) {
  const i32 addr = (i32) lua_tonumber(L, 1);
  const i32 n = lua_gettop(L) - 1;

  i32 i = 0;
  for (; i < n; i++) {
    /* TODO: implement bound-checking */
    const u32 value = (u32) lua_tonumber(L, 2 + i);
    cel8_poke4(addr, (i*4), value);
  }

  return 0;
}

_CEL8_PRIVATE i32 l_memcpy(lua_State *L) {
  i32 dstaddr = (i32) lua_tonumber(L, 1);
  i32 srcaddr = (i32) lua_tonumber(L, 2);
  i32 num = (i32) lua_tonumber(L, 3);

  /* TODO: implement bound-checking */

  void *dst = &*(cel8.memory + dstaddr);
  void *src = &*(cel8.memory + srcaddr);
  memcpy(dst, src, num);

  return 0;
}

_CEL8_PRIVATE i32 l_memset(lua_State *L) {
  i32 dstaddr = (i32) lua_tonumber(L, 1);
  i32 val = (i32) lua_tonumber(L, 2);
  i32 len = (i32) lua_tonumber(L, 3);

  /* TODO: implement bound-checking */

  void *dst = &*(cel8.memory + dstaddr);
  memset(dst, val, len * sizeof(u8));

  return 0;
}

/* drawing */

#define _CEL8_SET_CELL(offset, color, glyph) \
  cel8_poke(CEL8_VRAM_ADDR + offset, 0, color); \
  cel8_poke(CEL8_VRAM_ADDR + offset, 1, glyph);

_CEL8_PRIVATE inline bool should_clip(u8 x, u8 y) {
  return (x < 0 || x >= 0x10 || y < 0 || y >= 0x10);
}

_CEL8_PRIVATE i32 l_cls(lua_State *L) {
  const u8 color = (u8) lua_tonumber(L, 1);
  const u8 glyph = (u8) lua_tonumber(L, 2);

  i32 i = 0;
  for (; i < CEL8_VRAM_SIZE; i+=2) {
    _CEL8_SET_CELL(i, color, glyph);
  }

  return 0;
}

_CEL8_PRIVATE i32 l_color(lua_State *L) {
  const u8 c = (u8) lua_tonumber(L, 1);
  cel8_poke(CEL8_COLOR_ADDR, 0, c);
  return 0;
}

_CEL8_PRIVATE inline void put_char(u32 x, u32 y, const char c) {
  if (should_clip(x, y)) {
    return;
  }

  /* FIXME: magic number */
  const u8 color = cel8_peek(CEL8_COLOR_ADDR, 0);
  const u32 offset = (x * 2) + 0x20 * y;

  _CEL8_SET_CELL(offset, color, c);
}

_CEL8_PRIVATE i32 l_fill(lua_State *L) {
  const i32 x = (i32) lua_tonumber(L, 1);
  const i32 y = (i32) lua_tonumber(L, 2);
  const i32 w = (i32) lua_tonumber(L, 3);
  const i32 h = (i32) lua_tonumber(L, 4);

  i32 c = 0;
  if (lua_type(L, 5) == LUA_TSTRING) {
    const char *str = lua_tostring(L, 5);
    c = (i32) *(str + 0);
  } else {
    c = (i32) lua_tonumber(L, 5);
  }

  for (i32 i = y; i < (y + h); i++) {
    for (i32 j = x; j < (x + w); j++) {
      put_char(j, i, (char) c);
    }
  }

  return 0;
}

_CEL8_PRIVATE i32 l_pal(lua_State *L) {
  return 0;
}

_CEL8_PRIVATE i32 l_put(lua_State *L) {
  const i32 x = (i32) lua_tonumber(L, 1);
  const i32 y = (i32) lua_tonumber(L, 2);

  if (lua_type(L, 3) == LUA_TSTRING) {
    const char *str = lua_tostring(L, 3);
    
    i32 i = 0;
    while (*str) {
      put_char(x+(i++), y, *(str++));
    }

  } else {
    const i32 c = lua_tonumber(L, 3);
    put_char(x, y, c);
  }

  return 0;
}

_CEL8_PRIVATE i32 l_get(lua_State *L) {
  const i32 x = (i32) lua_tonumber(L, 1);
  const i32 y = (i32) lua_tonumber(L, 2);

  /* boundary check */
  if (should_clip(x, y)) {
    return 0;
  }

  /* FIXME: magic numbers (0x20) */
  const u32 offset = (x * 2) + 0x20 * y;

  lua_pushnumber(L, cel8_peek(offset, 0));

  return 1;
}

/* system */

_CEL8_PRIVATE i32 l_stat(lua_State *L) {
  const i32 id = (i32) lua_tonumber(L, 1);

  f64 result = 0;
  switch (id) {
    case CEL8_VERSION_STR: {
      lua_pushstring(L, CEL8_VERSION_STRING);
      return 1;
    }
    case CEL8_FRAME_TIME: {
      result = cel8.dt;
      break;
    }
    case CEL8_MOUSE_X: {
      result = cel8.mouse_x;
      break;
    }
    case CEL8_MOUSE_Y: {
      result = cel8.mouse_y;
      break;
    }
    case CEL8_GMT_YEAR: {
      time_t t = time(NULL);
      result = gmtime(&t)->tm_year + 1900;
      break;
    }
    case CEL8_GMT_MONTH: {
      time_t t = time(NULL);
      result = gmtime(&t)->tm_mon + 1;
      break;
    }
    case CEL8_GMT_DAY: {
      time_t t = time(NULL);
      result = gmtime(&t)->tm_mday;
      break;
    }
    case CEL8_GMT_HOUR: {
      time_t t = time(NULL);
      result = gmtime(&t)->tm_hour;
      break;
    }
    case CEL8_GMT_MIN: {
      time_t t = time(NULL);
      result = gmtime(&t)->tm_min;
      break;
    }
    case CEL8_GMT_SEC: {
      time_t t = time(NULL);
      result = gmtime(&t)->tm_sec;
      break;
    }
    case CEL8_LOCAL_YEAR: {
      time_t t = time(NULL);
      result = localtime(&t)->tm_year + 1900;
      break;
    }
    case CEL8_LOCAL_MONTH: {
      time_t t = time(NULL);
      result = localtime(&t)->tm_mon + 1;
      break;
    }
    case CEL8_LOCAL_DAY: {
      time_t t = time(NULL);
      result = localtime(&t)->tm_mday;
      break;
    }
    case CEL8_LOCAL_HOUR: {
      time_t t = time(NULL);
      result = localtime(&t)->tm_hour;
      break;
    }
    case CEL8_LOCAL_MIN: {
      time_t t = time(NULL);
      result = localtime(&t)->tm_min;
      break;
    }
    case CEL8_LOCAL_SEC: {
      time_t t = time(NULL);
      result = localtime(&t)->tm_sec;
      break;
    }
    default:
      return 0;
  }

  lua_pushnumber(L, result);
  return 1;
}

_CEL8_PRIVATE i32 l_rnd(lua_State *L) {
  i32 i = 1;
  for (; i >= 0; --i) {
    u8 reg_0 = cel8_peek(CEL8_RND_ADDR, 0);
    u8 reg_1 = cel8_peek(CEL8_RND_ADDR, 1);

    cel8_poke(CEL8_RND_ADDR, 0, 5 * reg_0 + 1);
    cel8_poke(CEL8_RND_ADDR, 1, ((reg_1 & 0x80) == (reg_1 & 0x10)) ? 2 * reg_1 + 1 : 2 * reg_1);
    cel8_poke(CEL8_RND_ADDR, 2+i, (reg_0 ^ reg_1));
  }

  u8 reg_2 = cel8_peek(CEL8_RND_ADDR, 2);
  u8 reg_3 = cel8_peek(CEL8_RND_ADDR, 3);
  u16 n = ((u16) reg_2 << 0x8) | reg_3;
  
  lua_pushnumber(L, n);
  return 1;
}

/* timer */

#if defined(OS_WINDOWS)
_CEL8_PRIVATE f64 get_frequency() {
  LARGE_INTEGER li;
  QueryPerformanceFrequency(&li);
  return (f64) li.QuadPart;
}
_CEL8_PRIVATE f64 get_time_absolute() {
  LARGE_INTEGER li;
  QueryPerformanceCounter(&li);
  return (f64) li.QuadPart;
}
#elif defined(OS_MACOS)
_CEL8_PRIVATE mach_timebase_info_data_t get_timebase_info() {
  mach_timebase_info_data_t info;
  mach_timebase_info(&info);
  return info;
}
#endif

_CEL8_PRIVATE f64 get_time(f64 start) {
#if defined(OS_WINDOWS)
  const f64 now = get_time_absolute();
  const f64 frequency = get_frequency();
  return (now - start) / frequency;
#elif defined(OS_MACOS)
  const mach_timebase_info_data_t timebase = get_timebase_info();
  const u64 mach_now = mach_absolute_time() - start;
  return ((f64) mach_now * 1.0e-9) * (f64) timebase.numer / (f64) timebase.denom;
#endif
}

_CEL8_PRIVATE i32 l_step(lua_State *L) {
  f64 prev = cel8.curr;

  cel8.curr = get_time(cel8.start);
  cel8.dt = cel8.curr - prev;

  return 0;
}

_CEL8_PRIVATE i32 l_time(lua_State *L) {
  lua_pushnumber(L, cel8.curr);
  return 1;
}

/* bitwise */

_CEL8_PRIVATE i32 l_band(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  i32 y = (i32) lua_tonumber(L, 2);
  lua_pushnumber(L, x & y);
  return 1;
}

_CEL8_PRIVATE i32 l_bor(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  i32 y = (i32) lua_tonumber(L, 2);
  lua_pushnumber(L, x | y);
  return 1;
}

_CEL8_PRIVATE i32 l_bxor(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  i32 y = (i32) lua_tonumber(L, 2);
  lua_pushnumber(L, x ^ y);
  return 1;
}

_CEL8_PRIVATE i32 l_bnot(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  lua_pushnumber(L, ~x);
  return 1;
}

_CEL8_PRIVATE i32 l_shl(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);
  lua_pushnumber(L, x << n);
  return 1;
}

_CEL8_PRIVATE i32 l_shr(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);
  lua_pushnumber(L, x >> n);
  return 1;
}

_CEL8_PRIVATE i32 l_lshr(lua_State *L) {
  i32 x = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);
  const i32 mask = ~(-1 << n) << (32 - n);
  lua_pushnumber(L, ~mask & (x >> n));
  return 1;
}

_CEL8_PRIVATE i32 l_rotl(lua_State *L) {
  const size_t b = sizeof(i32) * 8 - 1;

  i32 x = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);
  
  i32 d;
  n %= b;
  while (n--) {
    d = (x >> b) & 1; 
    x = (x << 1) | d;
  }
  lua_pushnumber(L, x);
  return 1;
}

_CEL8_PRIVATE i32 l_rotr(lua_State *L) {
  const i32 b = sizeof(i32) * 8 - 1;

  i32 x = (i32) lua_tonumber(L, 1);
  i32 n = (i32) lua_tonumber(L, 2);

  i32 d;
  n %= b;
  while (n--) {
    d = x & 1;
    x = (x >> 1) & (~(1 << b));
    x = x | (d << b);
  }

  lua_pushnumber(L, x);
  return 1;
}

#define luax_preload(k, f)        \
  lua_getglobal(L, "package");    \
  lua_getfield(L, -1, "preload"); \
  lua_pushcfunction(L, f);        \
  lua_setfield(L, -2, k);         \
  lua_pop(L, 2);

#define luax_require(k)        \
  lua_getglobal(L, "require"); \
  lua_pushstring(L, k);        \
  lua_call(L, 1, 1);

int luaopen_cel8_boot(lua_State *L) {
  #include "embed/boot.lua.h"
  if (luaL_loadbuffer(L, (const char *) boot_lua, sizeof(boot_lua), "=[cel8 \"boot.lua\"]") == 0) {
    lua_call(L, 0, 1);
  }
  
  return 1;
}

int luaopen_cel8(lua_State *L) {
  /* preload modules */
  luax_preload("cel8.boot", luaopen_cel8_boot);

  /* create cel8 table */
  lua_getglobal(L, "cel8");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "cel8");
  }

  /* cel8 functions */
  luaL_Reg reg[] = {
    /* event */
    { "poll",          l_poll              },

    /* memory */
    { "peek",          l_peek              },
    { "peek2",         l_peek2             },
    { "peek4",         l_peek4             },
    { "poke",          l_poke              },
    { "poke2",         l_poke2             },
    { "poke4",         l_poke4             },
    { "memcpy",        l_memcpy            },
    { "memset",        l_memset            },

    /* drawing */
    { "cls",           l_cls               },
    { "color",         l_color             },
    { "fill",          l_fill              },
    { "pal",           l_pal               },
    { "put",           l_put               },
    { "get",           l_get               },

    /* system */
    { "stat",          l_stat              },
    { "rnd",           l_rnd               },

    /* time */
    { "step",          l_step              },
    { "time",          l_time              },

    /* bitwise */
    { "band",          l_band              },
    { "bor",           l_bor               },
    { "bxor",          l_bxor              },
    { "bnot",          l_bnot              },
    { "shl",           l_shl               },
    { "shr",           l_shr               },
    { "lshr",          l_lshr              },
    { "rotl",          l_rotl              },
    { "rotr",          l_rotr              },

    /* always last. */
    { 0, 0 },
  };

  const luaL_Reg *l = &reg[0];
  for (; l->name != NULL; l++) {
    lua_pushcfunction(L, l->func);
    lua_setfield(L, -2, l->name);
  }

  return 1;
}

_CEL8_PRIVATE lua_State *L = NULL;

i32 cel8_init(i32 argc, char** argv) {
  /* initialize font */
  #include "embed/font.h"
  memcpy(cel8.memory + CEL8_FONT_ADDR, font_h, sizeof(font_h));

  /* initialize palette */
  #include "embed/palette.h"
  memcpy(cel8.memory + CEL8_PAL_ADDR, palette_h, sizeof(palette_h));
  
  /* initialize lua state */
  L = luaL_newstate();
  luaL_openlibs(L);

  luax_preload("cel8", luaopen_cel8);
  luax_require("cel8");

  /* create `cel8.argv` */
  lua_newtable(L);
  i32 i = 0;
  for (; i < argc; i++) {
    lua_pushstring(L, argv[i]);
    lua_rawseti(L, -2, i + 1);
  }
  lua_setfield(L, -2, "argv");

  /* pop cel8 */
  lua_pop(L, 1);

  luax_require("cel8.boot");
  lua_pop(L, 1);

  /* initialize timer */
#if defined(OS_WINDOWS)
  cel8.start = get_time_absolute();
#elif defined(OS_MACOS) 
  cel8.start = mach_absolute_time();
#endif

  return 0;
}

void cel8_cleanup(void) {
  /* lua virtual machine */
  lua_close(L);
  L = NULL;
}

void cel8_frame(void) {
  lua_getglobal(L, "cel8");
  if (!lua_isnil(L, -1)) {
    lua_getfield(L, -1, "frame");
    if (lua_pcall(L, 0, 0, 0) != 0) {
      const char *str = lua_tostring(L, -1);
      printf("Error: %s\n", str);
      return;
    }
  }
  lua_pop(L, 1);
}

void cel8_event(const cel8_event_t *e) {
  cel8.buffer[cel8.writei++] = *e;
}

#define _CEL8_RANGE(addr, sz) \
  (cel8_range) { .data = cel8.memory + addr, .size = sz };

cel8_range cel8_query_memory(void) {
  return _CEL8_RANGE(0x0000, CEL8_MEM_SIZE);
}

cel8_range cel8_query_pal(void) {
 return _CEL8_RANGE(CEL8_PAL_ADDR, CEL8_PAL_SIZE);
}

cel8_range cel8_query_color(void) {
  return _CEL8_RANGE(CEL8_COLOR_ADDR, CEL8_COLOR_SIZE);
}

cel8_range cel8_query_rnd(void) {
 return _CEL8_RANGE(CEL8_RND_ADDR, CEL8_RND_SIZE);
}

cel8_range cel8_query_font(void) {
  return _CEL8_RANGE(CEL8_FONT_ADDR, CEL8_FONT_SIZE);
}

cel8_range cel8_query_vram(void) {
  return _CEL8_RANGE(CEL8_VRAM_ADDR, CEL8_VRAM_SIZE);
}

#undef _CEL8_RANGE

#endif
