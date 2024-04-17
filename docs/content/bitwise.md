## Bitwise
Bitwise operation operates on a bit string at the level of its individual bits.

There are 9 bitwise functions available in cel8:

```lua
cel8.band( x, y )      -- bitwise and
cel8.bor( x, y )       -- bitwise or
cel8.bxor( x, y )      -- bitwise xor
cel8.bnot( x )         -- bitwise not
cel8.shl( x, n )       -- arithmetic shift left
cel8.shr( x, n )       -- arithmetic shift right
cel8.lshr( x, n )      -- logical shift left
cel8.rotl( x, n )      -- rotate left
cel8.rotr( x, n )      -- rotate right
```