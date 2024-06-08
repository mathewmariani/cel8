## Graphics
cel8 has a fixed capacity of 128 8x8 pixel glyphs.

All drawing operations in cel8 are subject to the current color. The current color is seperated into background and foreground. These are determined, respectively, by the high and low nibbles of the set color.

##### void c8_cls(uint8_t clr, uint8_t chr)
Clears the screen to a specified color and character.

##### void c8_color(uint8_t clr)
Set the current colour to be used by drawing functions.

For example, to set the color for the next drawing operation to (bg: dark_blue, fg: red).

```c
c8_color(0x18);
c8_print(0, 0, "@");
```

##### void c8_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t chr)
Set the characters in the rectangle [`x`, `y`, `w`, `h`] to `chr`.

##### void c8_put(uint8_t x, uint8_t y, char c)
Places a glyph at [`x`, `y`].

##### void c8_get( x, y )
Returns the character at [`x`, `y`].

##### void c8_print(uint8_t x, uint8_t y, const char *str)
Places text at [`x`, `y`].