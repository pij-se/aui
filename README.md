# aui
This README is work in progress.

Last updated 2021-11-16.

aui - short for **a**rbitrary **u**nsigned **i**ntegers - is a programming library written in ANSI C for memory management, assignment, conversion, comparison, bit manipulation, bit indexing and mathematical operations of and on data structures representing arbitrary-length unsigned integers.

## License
This software is published under the MIT License.

## Limitations
See the implementation of the following two functions:
```
aui_msbn()
aui_msba()
```

## Reliability
It's my intention to keep the code void of - as per the C standard - undefined, unspecified or implementation-defined behaviour, however I do not guarantee that such is the case. While I have succressfully tested the code using [test.c](https://github.com/pij-se/aui/tree/master/src/test.c), I do not guarantee that the code will always work as intended - you are strongly encouraged to write and run your own test, using your target compiler and platform.

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

Use `aui_alloc()` to allocate arbitrary unsigned integer data structures, or `aui_pull()`  retreive arbitrary unsigned integer data structures from the global linked list (if existing, otherwise allocate), and `aui_free()` to free them,  or `aui_push()` to put them back on the global list for later use.

Assign a value using `aui_seti()` or `aui_sets()` for assignment using native unsigned integers or strings respectively. Call `aui_geti()` or `aui_gets()` to convert the value back to native unsigned integer or string. Use `aui_asgn()` and `aui_swap()` to assign and swap values between arbitrary unsigned integer data structures.

All comparison operations should perform identical to their native equivalents:

```
aui_eq(x, y); /* x == y */
aui_neq(x, y); /* x != y */
aui_lt(x, y); /* x < y */
aui_lte(x, y); /* x <= y */
aui_gt(x, y); /* x > y */
aui_gte(x, y); /* x >= y */
aui_eval(x); /* !!x */
```

All bit manipulation and mathematical operations should perform identical to their native compound assignment equivalents:

```
aui_one(x); /* x = ~x */
aui_two(x); /* x = ~x + 1 */
aui_and(x, y); /* x &= y */
aui_ior(x, y); /* x |= y */
aui_xor(x, y); / x ^= y* */
aui_shl(x, i); /* x <<= i */
aui_shr(x, i); /* x >>= i */

aui_inc(x); /* x++ */
aui_dec(x); /* x-- */
aui_add(x, y); /* x += y */
aui_sub(x, y); /* x -= y */
aui_mul(x, y); /* x *= y */
aui_div(x, y); /* x /= y */
aui_mod(x, y); /* x %= y */
```

Before returning, call `aui_wipe()` and `aui_free()` to free any allocated arbitrary unsigned integer data structures.

**Copyright (c) Johan Palm**
