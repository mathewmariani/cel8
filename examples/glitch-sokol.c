#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

/* std */
#include <stdio.h>
#include <stdlib.h>

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
}

void c8_update(void)
{
    // int8_t n = rand() % (0x10 * 0x10);
    // c8_poke(C8_MEM_VRAM_ADDR + (n * 2) + 0, rand() % 0xff);
    // c8_poke(C8_MEM_VRAM_ADDR + (n * 2) + 1, rand() % 0x7f);
}

void c8_draw(void)
{
    /* body */
}