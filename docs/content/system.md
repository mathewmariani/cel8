## System

##### void c8_stat(u8 n)
Gets the system information where `n` can be:

```c
/*
    0x0       : version string
    0x1       : frame time
    0x2..0x3  : mouse x, y
    0x4..0x9  : GMT time: year, month, day, hour, minute, second
    0xA..0xF  : Local time: ...
*/
```

##### void c8_rnd(void)
Returns a 16-bit pseudo random number.

The random number generator is mapped to adresses `0x0040..0x0043`. Where the first two bytes represent the seed, and the last two are the high/low values of the ouput.

Calling `c8_rnd()` will generate a new a random number.

##### void c8_time(void)
Returns the value of a timer, in seconds, with an unspecified starting time.

Multiple calls of `c8_time()` during the same frame will always return the same result.