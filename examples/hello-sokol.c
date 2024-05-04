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
}

void c8_update(void)
{
    /* body */
}

int i = 0;
void c8_draw(void)
{
    c8_frame();
    c8_color(0x80);
    c8_print(2, 7, "Hello");
    c8_color(0x08);
    c8_print(8, 7, "World!");
    c8_color(0x05);
    c8_print(2, 8, "------------");
    c8_color(0x06);
    c8_print(2 + (int)(i) % 12, 8, "-");
    i = i + 0.25;
}