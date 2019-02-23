Last updated 2019-02-23.
# aui
A program library in C for basic arbitrary unsigned integer representation, memory management, assignment, conversion, comparison, bit manipulation, bit indexing, and mathematical operations. Developed by Johan Palm, published under the MIT license.

## Limitations
TODO

## Performance
TODO

## Usage

### Configuration
In [aui.h](https://github.com/pij-se/aui/edit/master/src/aui.h) you'll find:
```
#define AUI_TYPE unsigned char
#define AUI_TYPE_BIT CHAR_BIT
#define AUI_TYPE_MAX UCHAR_MAX
```
These definitions determine the native data type used to represent arbitrary unsigned integer values, the size in bits of the native data type, and the maximum value of the native data type.

It is recommended to redefine `AUI_TYPE`, `AUI_TYPE_BIT` and `AUI_TYPE_MAX` to use the fastest unsigned integer data type for the intended compiler and platform with a known exact size.

**IMPORTANT** `AUI_TYPE` must be an unsigned integer data type.
**IMPORTANT** [test.c](https://github.com/pij-se/aui/edit/master/src/test.c) assumes the definitions above.

### Testing
TODO

### Function descriptions
TODO
