# aui
This README is work in progress.
Last updated 2021-11-15.

aui --- short for **a**rbitrary **u**nsigned **i**ntegers --- is a programming library written in ANSI C for memory management, assignment, conversion, comparison, bit manipulation, bit indexing and mathematical operations of and on data structures representing arbitrary-length unsigned integers.

## License
This software is published under the MIT License.

## Limitations
See the implementation of the following two functions:
```
aui_msbn()
aui_msba()
```

## Reliability
It's my intention to keep the code void of - as per the C standard - undefined, unspecified or implementation-defined behaviour, however I do not guarantee that such is the case.

Further more, while I successfully tested the library using [test.c](https://github.com/pij-se/aui/tree/master/src/test.c), I can't guarantee the reliability of the code. I strongly recommend that you write and run your own test, using your target compiler and platform.

## Performance
I'm planning on running a performance test for comparison with the [GNU Multiple Precision Arithmetic Library](https://gmplib.org/). If you beat me to it, please make a pull request.

## Usage
Change the following definitions in [aui.h](https://github.com/pij-se/aui/tree/master/src/aui.h) to your preference:
```
#define AUI_TYPE unsigned char /* the fastest, largest, or smallest native unsigned integer type */
#define AUI_TYPE_BIT CHAR_BIT /* the number of bits in AUI_TYPE */
#define AUI_TYPE_MAX UCHAR_MAX /* the max value of AUI_TYPE */
```
Call `aui_init()` (at least) once before any other functions; it sets up the global de Bruijn sequence and lookup tables for most- and least significant bit indexing, which in turn is used by many of the other functions.

Use `aui_alloc()` or `aui_pull()` to allocate or retreive arbitrary unsigned integer data structures of the desired size, and `aui_free()` to free them,  or `aui_push()` to put them back on the global list for later use.

Assign a value using `aui_seti()` or `aui_sets()` for assignment using native unsigned integers or strings respectively. Call `aui_geti()` or `aui_gets()` to convert the value back to native unsigned integer or string.

All comparison operations (`aui_eq(); aui_neq(); aui_lt(); aui_lte(); aui_gt(); aui_gte()`) should perform identical to their native equivalents (`x == y; x != y; x < y; x <= y; x > y; x >= y`).

All bit manipulation (`aui_one(); aui_two(); aui_and(); aui_ior(); aui_xor(); aui_shl(); aui_shr()`) and mathematical operations (`aui_inc(); aui_dec(); aui_add(); aui_sub(); aui_mul(); aui_div(); aui_mod()`) should perform identical to their native compound assignment equivalents (`x = ~x; x = ~x + 1; x = x & y; x = x | y; x = x ^ y; x = x << y; x = x >> y` and `x = x++; x = x--; x += y; x -= y; x *= y; x /= y; x %= y`)

Before returning, call `aui_wipe()` to free any temporarily allocated arbitrary unsigned integer data structures.

**Copyright (c) Johan Palm**
