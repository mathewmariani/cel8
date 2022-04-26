## System

##### cel8.stat( n )
Get system information where `n` is:

```lua
0x0       -- version string
0x1       -- frame time
0x2..0x3  -- mouse x, y
0x4..0x9  -- GMT time: year, month, day, hour, minute, second
0xA..0xF  -- Local time: ...
```

##### cel8.rnd( )
Returns a 16-bit pseudo random number.

The random number generator is mapped to adresses `0x0040..0x0043`. Where the first two bytes represent the seed, and the last two are the high/low values of the ouput.

##### cel8.time( )
Returns the value of a timer, in seconds, with an unspecified starting time.

Multiple calls of `time( )` during the same frame will always return the same result.