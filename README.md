
Last updated 2019-02-24.
# aui
A program library in C for basic **a**rbitrary **u**nsigned **i**nteger representation, memory management, assignment, conversion, comparison, bit manipulation, bit indexing, and mathematical operations. Developed by Johan Palm, published under the MIT license.

## Limitations
See the descriptions of the following functions:
* `AUI_TYPE aui_msbn(AUI_TYPE native)`
* `unsigned long aui_msba(const struct aui *x)`

In conclusion, to remain safe and avoid any issues due to limitations imposed by certain functions, the maximum length of the array in an arbitrary unsigned integer data structure should be limited to

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

#### Size to length: `AUI_SIZTOLEN(size)`
Calculate the length of an array of data type `AUI_TYPE` to represent unsigned integer values of at least `size` bytes.

#### Initialise: `void aui_init(void)`
- **Call once before other function calls.**
- Asserts: `AUI_TYPE_BIT` must match the size in bits of `AUI_TYPE`. `AUI_TYPE_MAX` must match the maximum value of `AUI_TYPE`. `AUI_TYPE_BIT` must be a power of two.

Computes a de Bruijn sequence and generates lookup tables for most and least significant bit indexing. The values are stored in the `static` global variables `shift` and `debruijn`, and arrays `most` and `least`.

#### Most significant bit of a native variable: `AUI_TYPE aui_msbn(AUI_TYPE native)`
* Asserts: `native` must be larger than 0.
* Returns: The index of the least significant bit in `native`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT - 1`.
* Limitations: The current implementation supports `AUI_TYPE` bit sizes up to and including 512 bits. This is for futurerproofing and can easily be increased further.

Indexes the most significant bit in the variable `native`.

#### Least significant bit of a native variable: `AUI_TYPE aui_lsbn(AUI_TYPE native)`
* Asserts: `native` must be larger than 0.
* Returns: The index of the least significant bit in `native`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT - 1`.

Indexes the least significant bit in the variable `native`.

#### Most significant bit of an arbitrary unsigned integer: `unsigned long aui_msba(const struct aui *x)`
* Asserts: `x` must not be a pointer to `NULL`.
* Returns: The index of the most significant bit in the arbitrary unsigned integer data structure pointed to by `x`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT * x->length - 1`.
If the arbitrary unsigned integer value pointed to by`x` is equal to 0, `ULONG_MAX` is returned.
* Limitations: Since `ULONG_MAX` is reserved for signalling an arbitrary unsigned integer with a value of zero (no bits set), the maximum number of bits per arbitrary unsigned integer should be `ULONG_MAX - 1`.

Indexes the most significant but in the arbitrary unsigned integer data structure pointed to by `x`.

#### Least significant bit of an arbitrary unsigned integer: `unsigned long aui_lsba(const struct aui *x)`
* Asserts: `x` must not be a pointer to `NULL`.
* Returns: The index of the least significant bit in the arbitrary unsigned integer data structure pointed to by `x`, where the least significant bit has index 0, and the most significant bit has index `AUI_TYPE_BIT * x->length - 1`.
If the arbitrary unsigned integer value pointed to by`x` is equal to 0, `ULONG_MAX` is returned.
* Limitations: Since `ULONG_MAX` is reserved for signalling an arbitrary unsigned integer with a value of zero (no bits set), the maximum number of bits per arbitrary unsigned integer should be `ULONG_MAX - 1`.

Indexes the least significant but in the arbitrary unsigned integer data structure pointed to by `x`.

#### Allocate: `struct aui *aui_alloc(unsigned int length)`
* Asserts: `length` must be larger than 0.
* Returns: If successful, returns a pointer to a newly allocated unitialised arbitrary unsigned integer data structure capable of representing exactly `sizeof(AUI_TYPE) * length` bytes. If not successful, returns `NULL`.

Allocates memory for an arbitrary unsigned integer data structure and its array of type `AUI_TYPE` and length `length` for storing an arbitrary unsigned integer value.

#### Free: `void aui_free(struct aui *x)`
* Asserts: `x` must not be a pointer to `NULL`.
Limitations: `x` must be a pointer to a previously allocated arbitrary unsigned integer data structure, where `x->array` is a pointer to a previously and separately allocated array.

Frees previously allocated memory of an arbitrary unsigned integer data structure (preferably made by `aui_alloc()`).

#### Pull: `struct aui *aui_pull(unsigned int length)`
* Asserts: `length` must be larger than 0.
* Returns: If successful, returns a pointer to a newly or previously allocated arbitrary unsigned integer capable of representing exactly `sizeof(AUI_TYPE) * length` bytes. If not successful, returns `NULL`.

Pulls a previously allocated arbitrary unsigned integer data structure from `list`. Alternatively allocates a new arbitrary unsigned integer data structure using `aui_alloc()`.

#### Push: `void aui_push(struct aui *x)`
* Asserts: `x` must not be a pointer to `NULL`.

Pushes a previously allocated arbitrary unsigned integer data structure to `list`.

#### Wipe: `void aui_wipe(void)`
Wipes `list` from previously allocated arbitrary unsigned integer data structured pushed to the list.

#### Set from integer: `void aui_seti(struct aui *x, unsigned long value)`
* Asserts: `x` must not be a pointer to `NULL`.

Assigns the value of `value` to `x`, as long as `x` is large enough. Otherwise, `x` will equal the value of `value` modulus the maximum value of `x`.

#### Set from string: `int aui_sets(struct aui *x, const char *string, const char *set, int base)`
* Asserts: `x` must not be a pointer to `NULL`. `string` must not be a pointer to `NULL`. `set` must not be a pointer to `NULL`. `base` must be a value between 2 and `CHAR_MAX`.
* Returns: `true` if successful, otherwise `false`.

Assigns the value represented by string `string` using the character set `set` to represent values in base `base` to the arbitrary unsigned integer `x`.

#### Get integer: `unsigned long aui_geti(const struct aui *x)`
* Asserts: `x` must not be a pointer to `NULL`.
* Returns: The value of `x`. If the value of `x` is larger than `ULONG_MAX`, the value returned will equal the value of `x` modulus `ULONG_MAX`.
