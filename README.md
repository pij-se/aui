
Last updated 2019-02-24.
# aui
A program library in C for basic **a**rbitrary **u**nsigned **i**nteger representation, memory management, assignment, conversion, comparison, bit manipulation, bit indexing, and mathematical operations. Developed by Johan Palm, published under the MIT license.

## Limitations
See the function descriptions below for:
* `AUI_TYPE aui_msbn(AUI_TYPE native)`
* `unsigned long aui_msba(const struct aui *x)`

## Reliability
While I have strived to not rely on, as per the C standard, undefined, unspecified or implementation-defined behaviour, I do not guarantee that the library is void of such code.

In addition, while I have tested the code for errors using [test.c](https://github.com/pij-se/aui/edit/master/src/test.c), I encourage you to write your own or to run the same test using your target compiler, on your target platform.

## Performance
I plan on running performance tests for comparison with the [GNU Multiple Precision Arithmetic Library](https://gmplib.org/). If you get to it before me, please make a pull request.

## Usage

### Configuration
In [aui.h](https://github.com/pij-se/aui/edit/master/src/aui.h) you'll find:
```
#define AUI_TYPE unsigned char
#define AUI_TYPE_BIT CHAR_BIT
#define AUI_TYPE_MAX UCHAR_MAX
```
These definitions determine
* the native data type primarily used for the array in `struct aui` to store arbitrary unsigned integer values,
* the size in bits of the native data type, and
* the maximum value of the native data type.

It is recommended to redefine `AUI_TYPE`, `AUI_TYPE_BIT` and `AUI_TYPE_MAX` to use the target compiler and platform's fastest unsigned integer data type. Typically `unsigned int`.

### Function and macro descriptions

#### `AUI_SIZTOLEN(size)`
Calculate the length of an array of data type `AUI_TYPE` to represent unsigned integer values of at least `size` bytes.

#### `void aui_init(void)`
- **Call once before other function calls.**
- Asserts:
`AUI_TYPE_BIT` must match the size in bits of `AUI_TYPE`.
`AUI_TYPE_MAX` must match the maximum value of `AUI_TYPE`.
`AUI_TYPE_BIT` must be a power of two.

Computes a de Bruijn sequence and generates lookup tables for most and least significant bit indexing. The values are stored in the `static` global variables `shift` and `debruijn`, and arrays `most` and `least`.

#### `AUI_TYPE aui_msbn(AUI_TYPE native)`
* Asserts:
`native` must be larger than 0.
* Returns:
The index of the least significant bit in `native`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT - 1`.
* Limitations:
The current implementation supports `AUI_TYPE` bit sizes up to and including 512 bits. This is for futurerproofing and can easily be increased further.

Indexes the most significant bit in the variable `native`.

#### `AUI_TYPE aui_lsbn(AUI_TYPE native)`
* Asserts:
`native` must be larger than 0.
* Returns:
The index of the least significant bit in `native`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT - 1`.

Indexes the least significant bit in the variable `native`.

#### `unsigned long aui_msba(const struct aui *x)`
* Asserts:
`x` must not be a pointer to `NULL`.
* Returns:
The index of the most significant bit in the arbitrary unsigned integer data structure pointed to by `x`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT * x->length - 1`.
If the arbitrary unsigned integer value pointed to by`x` is equal to 0, `ULONG_MAX` is returned.
* Limitations:
In the extreme case that `x->array` consists of more than `ULONG_MAX / AUI_TYPE_BIT` 

