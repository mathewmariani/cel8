## Graphics
cel8 has a fixed capacity of 128 8x8 pixel glyphs.

All drawing operations in cel8 are subject to the current color. The current color is seperated into background and foreground. These are determined, respectively, by the high and low nibbles of the set color.

##### cel8.cls( clr, chr )
Clears the screen to a specified color and character.

##### cel8.color( clr )
Set the current colour to be used by drawing functions.

For example, to set the color for the next drawing oepration to (bg: dark_blue, fg: red).

```lua
function cel8.draw()
  cel8.color(0x18)
  cel8.put(0, 0, '@')
end
```

##### cel8.fill( x, y, w, h, chr )
Set the characters in the rectangle [`x`, `y`, `w`, `h`] to `chr`.

##### cel8.put( x, y, str )
Places text at [`x`, `y`].

##### cel8.get( x, y )
Returns the character at [`x`, `y`].
