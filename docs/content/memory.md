## Memory
cel8 has <4kb of RAM: see layout below.
This memory can be accessed with `cel8.peek`, `cel8.poke`, `cel8.memcpy`, `cel8.memset`.

### Memory Layout
```lua
0x0000                          -- color mapping
0x000F                          -- color palette
0x003F                          -- draw color
0x0040                          -- random state
0x0044                          -- unused
0x0050                          -- font atlas
0x0450                          -- screen buffer
```

The screen buffer is composed of interleved data; the first two bytes of memory represent the color and the glyph, respectively

##### cel8.peek( addr [, n] )
Read `n` bytes from an address in ram. If `n` is not specified the `peek()` will return the first byte.

For example, to read the first two bytes of video memory:

```lua
local a, b = peek(0x0450, 2)
```

##### cel8.poke( addr [, value [, ...]] )
Write one or more bytes to an address in ram. If more than one parameter is provided the bytes will be written sequentially.

##### cel8.peek2( addr [, n] )
##### cel8.poke2( addr [, value [, ...]] )
##### cel8.peek4( addr [, n] )
##### cel8.poke4( addr [, value [, ...]] )
16-bit and 32-bit variations of `peek( addr [, n] )` and `poke( addr [, value [, ...]] )` are also offered.

##### cel8.memcpy( dst, src, len )
Copies the values of `len` bytes from the location pointed to by `src` directly to the memory block pointed to by `dst`.

##### cel8.memset( dst, value, len )
Sets the first `len` bytes of the block of memory pointed by `dst` to the specified `value`.