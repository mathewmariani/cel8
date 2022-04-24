# cel8 
A *tiny* framework for grid-based games in lua.

## Documentation

### Callbacks:
```
load( )                          : called after initialization
update( )                        : called every frame
draw( )                          : called every frame
keydown( key )                   : called when a key is pressed
keyup( key )                     : called when a key is released
```

### Functions:
```
peek( addr [, n] )               : reads n 8-bit values from memory
peek2( addr [, n] )              : reads n 16-bit values from memory
peek4( addr [, n] )              : reads n 32-bit values from memory
poke( addr [, value [, ...]] )   : writes one or more 8-bit values to memory
poke2( addr [, value [, ...]] )  : writes one or more 16-bit values to memory
poke4( addr [, value [, ...]] )  : writes one or more 32-bit values to memory
memcpy( dst, src, len )          : copies a block of memory to another block
memset( dst, value, len )        : sets a block of memory to a specified value

cls( clr, chr )                  : clears the screen to a specified color and char
color( clr )                     : sets the draw color
fill( x, y, w, h, chr )          : sets the characters in the rectangle to specified char
put( x, y, str )                 : places text at (x, y)
get( x, y )                      : current environment
stat( n )                        : system information
rnd( )                           : pseudo random number
time( )                          : time since start

band( x, y )                     : bitwise and
bor( x, y )                      : bitwise or
bxor( x, y )                     : bitwise xor
bnot( x )                        : bitwise not
shl( x, n )                      : arithmetic shift left
shr( x, n )                      : arithmetic shift right
lshr( x, n )                     : logical shift left
rotl( x, n )                     : rotate left
rotr( x, n )                     : rotate right
```

## Memory Map
```
0x0000                           : color mapping
0x000F                           : color palette
0x003F                           : draw color
0x0040                           : random state
0x0044                           : unused
0x0050                           : font atlas
0x0450                           : screen buffer
```

See the **[cel8.h](src/cel8.h?raw=1)** header for a more complete documentation.

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.