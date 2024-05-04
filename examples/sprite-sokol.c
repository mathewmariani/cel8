#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

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

    // custom ascii sprite:
    // 01111110  -- 0x7E
    // 11011011  -- 0xDB
    // 11011011  -- 0xDB
    // 11011011  -- 0xDB
    // 11111111  -- 0xFF
    // 10111101  -- 0xBD
    // 11000011  -- 0xC3
    // 01111110  -- 0x7E

    const uint32_t font_addr = 0x0050;
    const uint32_t font_size = 0x0008;

    /* poke */
    c8_poke(font_addr + font_size * 0, 0x00, 0x7E);
    c8_poke(font_addr + font_size * 0, 0x01, 0xDB);
    c8_poke(font_addr + font_size * 0, 0x02, 0xDB);
    c8_poke(font_addr + font_size * 0, 0x03, 0xDB);
    c8_poke(font_addr + font_size * 0, 0x04, 0xFF);
    c8_poke(font_addr + font_size * 0, 0x05, 0xBD);
    c8_poke(font_addr + font_size * 0, 0x06, 0xC3);
    c8_poke(font_addr + font_size * 0, 0x07, 0x7E);

    /* poke */
    c8_poke2(font_addr + font_size * 1, 0x00 * 2, 0x7EDB);
    c8_poke2(font_addr + font_size * 1, 0x01 * 2, 0xDBDB);
    c8_poke2(font_addr + font_size * 1, 0x02 * 2, 0xFFBD);
    c8_poke2(font_addr + font_size * 1, 0x03 * 2, 0xC37E);

    /* poke */
    c8_poke4(font_addr + font_size * 2, 0x00 * 4, 0x7EDBDBDB);
    c8_poke4(font_addr + font_size * 2, 0x01 * 4, 0xFFBDC37E);
}

static void c8_update(void)
{
    /* body */
}

static void c8_draw(void)
{
    /* program */
    c8_color(0x08);
    c8_put(0, 0, 0x00);
    c8_put(1, 0, 0x01);
    c8_put(2, 0, 0x02);
}