## Graphics
cel8 has a fixed capacity of 128 8x8 pixel glyphs.

All drawing operations in cel8 are subject to the current color. The current color is seperated into background and foreground. These are determined, respectively, by the high and low nibbles of the set color.

##### void c8_cls(u8 clr, u8 chr)
Clears the screen to a specified color and character.

##### void c8_color(u8 clr)
Set the current colour to be used by drawing functions.

For example, to set the color for the next drawing operation to (bg: dark_blue, fg: red).

```c
c8_color(0x18);
c8_print(0, 0, "@");
```

##### void c8_fill(u8 x, u8 y, u8 w, u8 h, u8 chr)
Set the characters in the rectangle [`x`, `y`, `w`, `h`] to `chr`.

##### void c8_put(u8 x, u8 y, char c)
Places a glyph at [`x`, `y`].

##### void c8_get( x, y )
Returns the character at [`x`, `y`].

##### void c8_print(u8 x, u8 y, const char *str)
Places text at [`x`, `y`].