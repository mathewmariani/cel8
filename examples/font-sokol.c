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

    /* program */
    int i;
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            i = x + 16 * y;
            c8_poke(0x0450 + (i * 2) + 0, rand() % 0xff);
            c8_poke(0x0450 + (i * 2) + 1, i);
        }
    }
}

void c8_update(void)
{
    /* body */
}

void c8_draw(void)
{
    /* body */
}