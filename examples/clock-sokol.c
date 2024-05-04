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

static char dstr[10];
static char tstr[8];

void c8_update(void)
{
    /* body */
}

void c8_draw(void)
{
    /* program */
    /* year-month-day */
    /* hour-minute-second */
    snprintf(&dstr[0], sizeof(dstr), "%d-%d-%d", c8_stat(0xa), c8_stat(0xb), c8_stat(0xc));
    snprintf(&tstr[0], sizeof(tstr), "%d:%d:%d", c8_stat(0xd), c8_stat(0xe), c8_stat(0xf));

    c8_color(0x01);
    c8_print(0, 0, "DATE");
    c8_print(0, 1, "TIME");

    c8_color(0x07);
    c8_print(5, 0, dstr);
    c8_print(5, 1, tstr);
}