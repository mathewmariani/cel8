## Memory
cel8 has <4kb of RAM: see layout below.
This memory can be accessed with the functions `c8_peek`, `c8_poke`, `c8_memcpy`, `c8_memset`.

### Memory Layout
```c
/*
    0x0000: color mapping
    0x000F: color palette
    0x003F: draw color
    0x0040: random state
    0x0044: unused
    0x0050: font atlas
    0x0450: screen buffer
*/
```

The screen buffer is composed of interleved data; the first two bytes of memory represent the color and the glyph, respectively

##### uint8_t c8_peek(const u32 addr, const u32 index)
Read `n` bytes from an address in ram. If `n` is not specified the `peek()` will return the first byte.

For example, to read the first two bytes of video memory:

```c
uint8_t value = c8_peek(0x0450, 0x00);
```

##### void c8_poke(const u32 addr, const u32 index, const uint8_t value)
Write a single bytes to an address in ram.

##### uint16_t c8_peek2(const u32 addr, const u32 index)
##### void c8_poke2(const u32 addr, const u32 index, const uint8_t value)
##### u32 c8_peek4(const u32 addr, const u32 index)
##### void c8_poke4(const u32 addr, const u32 index, const uint16_t value)
16-bit and 32-bit variations of `c8_peek` and `c8_poke` are also offered.

##### void c8_memcpy(void *dst, const void *src, size_t len)
Copies the values of `len` bytes from the location pointed to by `src` directly to the memory block pointed to by `dst`.

##### void c8_memset(void *dst, int value, size_t len)
Sets the first `len` bytes of the block of memory pointed by `dst` to the specified `value`.