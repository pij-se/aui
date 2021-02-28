
Last updated 2019-02-24.
# aui
A program library in C for basic **a**rbitrary **u**nsigned **i**nteger representation, memory management, assignment, conversion, comparison, bit manipulation, bit indexing, and mathematical operations. Developed by Johan Palm, published under the MIT license.

## Limitations
See the descriptions of the following functions:
- `AUI_TYPE aui_msbn(AUI_TYPE native)`
- `unsigned long aui_msba(const struct aui *x)`

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
Wipes `list` of allocated arbitrary unsigned integer data structured previously pushed to the list.

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


---


Last updated: 2021-02-21
# aui
aui --- arbitrary unsigned integers --- is a program library written in ANSI C that enables representation of arbitrary-length unsigned integers accompanied by memory management, assignment, conversion, bit manipulation, bit indexing and mathematical operations.

Developed by Johan Palm. Published under the MIT license.

## Limitations
See the descriptions of the following functions:
* `AUI_TYPE aui_msbn(AUI_TYPE native) {...}`
* `unsigned long aui_msba(const struct aui *x) {...}`
* **TODO**

In short, to avoid issues imposed by the aforementioned functions, the maximum length of the array in an arbitrary unsigned integer data structure should be limited to **TODO**.

## Reliability
As stated by the license, the software is provided without warranty of any kind. With that said, the code was written to be void of --- as per the ANSI C standard --- undefined, unspecified and implementation-defined behaviour, and should thus be fully deterministic, assuming that the code as such, the target compiler, and the target platform are void of bugs.

It is strongly recommended to thoroughly test the code using the target compiler on the target platform.

## Performance
Performance tests are yet to be carried out.

## Usage
### Configuration
For best performance it is recommended to change the definition of `AUI_TYPE`, `AUI_TYPE_BIT` and `AUI_TYPE_MAX` to use the target compiler's and target platform's fastest and/or largest unsigned integer data type.
* `AUI_TYPE` defines the data type.
* `AUI_TYPE_BIT` defines the number of bits in the `AUI_TYPE` type.
* `AUI_TYPE_MAX` defines the maximum value that can be represented by the `AUI_TYPE` type.

The data type defined by `AUI_TYPE` is used for the array in `struct aui` to contain the binary representation of an arbitrary unsigned integer.

### Macros
* **`AUI_SIZTOLEN(size)`**

	Calculates the length of an array of data type `AUI_TYPE` necessary to hold at least `size` bytes.

	Example:
	```
	// allocate an aui struct that can represent at least 50 bytes
	x = aui_alloc(AUI_SIZTOLEN(50));
	```

### Functions
* **`void aui_init(void)`**
	* **Call once before calls to other functions.**
	* Assertions: `AUI_TYPE_BIT` must equal the size of `AUI_TYPE` in bits. `AUI_TYPE_MAX` must equal the maximum value of `AUI_TYPE`. `AUI_TYPE_BIT` must be a power of two.
	* Returns: None.

	Computes a *de Bruijn* sequence and generates lookup tables for most and least significant bit indexing.

* **`AUI_TYPE aui_msbn(AUI_TYPE native)`**
	* **The implementation currently supports `AUI_TYPE` sizes up to 512 bits. This can be easily extended if/when needed.**
	* Assertions: `native` must be larger than 0.
	* Returns: The index of the most significant bit in `native`, where the least significant bit has index 0.

	Indexes the most significant bit in `native`.

* **`AUI_TYPE aui_lsbn(AUI_TYPE native)`**
	**Return: The index of the least significant bit in `native`, where the least significant bit has index 0.**

	Indexes the least significant bit in `native`.

* **`unsigned long aui_msba(const struct aui *x)`**
	**Return: The index of the most significant bit in**

---

Last updated: 2021-02-27
# aui
aui (short for arbitrary unsigned integers) is a program library written in ANSI C that enables representation of arbitrary-length unsigned integers, accompanied by memory management, assignment, conversion, bit manipulation, bit indexing, and mathematical functions.

Developed by Johan Palm. Published under the MIT license.

## Limitations
See the descriptions of the following functions:
* `aui_msbn()`
* `aui_msba()`
* `aui_lsba()`
* **TODO**

In short, to avoid issues imposed by the aforementioned functions the following restrictions must be applied:
* The maximum size of an array in bits in an arbitrary unsigned integer structure should not be greater than `ULONG_MAX - 1`. In effect, the maximum size of an array in bytes in an arbitrary unsigned integer structure is thus limited to `(unsigned long)(((ULONG_MAX - 1) / AUI_TYPE_BIT) / CHAR_BIT)`.
* If the size of `AUI_TYPE` is greater than 512 bits (i.e. if `AUI_TYPE_BIT` is greater than 512) then the implementation of `aui_msbn()` must be modified.

## Reliability
As stated by the license, the software is provided as is, without warranty of any kind.

The code was written to be void of --- as per the ANSI C standard --- undefined, unspecified and implementation-defined behaviour, and *may* thus be deterministic, assuming that the code as such, the target compiler, and the target platform are void of bugs, and assuming that calls to memory allocation functions are guaranteed to be successful or are otherwise avoided.

It is recommended to thoroughly test the code using the target compiler on the target platform.

## Performance
Performance tests are yet to be carried out.

## Usage
### Quick start
The following example shows how to use each function, and their equivalent operators/implementations in C.
```
int main(int argc, char *argv[])
{
	unsigned long a;
	unsigned long b;
	unsigned long c;

	struct aui *x = aui_alloc(AUI_SIZTOLEN(sizeof(a)));
	struct aui *y = aui_alloc(AUI_SIZTOLEN(sizeof(b)));
	struct aui *z = aui_alloc(AUI_SIZTOLEN(sizeof(c)));

	// TODO

	return 0;
}
```

### Configuration
For best performance it is recommended to change the definition of `AUI_TYPE`, `AUI_TYPE_BIT` and `AUI_TYPE_MAX` to use the target compiler's and target platform's fastest and/or largest unsigned integer data type.
* `AUI_TYPE` defines the data type of the array in `struct aui`.
* `AUI_TYPE_BIT` defines the number of bits in the `AUI_TYPE` type.
* `AUI_TYPE_MAX` defines the maximum value that can be represented by the `AUI_TYPE` type.

The data type defined by `AUI_TYPE` is used for the array in `struct aui` to hold the binary representation of an arbitrary unsigned integer.

### Macros
* **`AUI_SIZTOLEN(size)`**

	Calculates the smallest length of an array of data type `AUI_TYPE` necessary to hold at least `size` bytes.

	Example:
	```
	// allocate an aui struct that can represent at least 50 bytes
	x = aui_alloc(AUI_SIZTOLEN(50));
	```

### Functions
* **`void aui_init(void)`**
	* **Call once before calls to other functions.**
	* Assertions: `AUI_TYPE_BIT` must equal the size of `AUI_TYPE` in bits. `AUI_TYPE_MAX` must equal the maximum value of `AUI_TYPE`. `AUI_TYPE_BIT` must be a power of two.
	* Returns: None.

	Computes a *de Bruijn* sequence and generates lookup tables for most and least significant bit indexing.

* **`AUI_TYPE aui_msbn(AUI_TYPE native)`**
	* **The implementation currently supports `AUI_TYPE` sizes up to 512 bits. This can be easily extended if needed.**
	* Assertions: `native` must be greater than 0.
	* Returns: The index of the most significant set bit in `native`, where the least significant bit has index 0.

	Indexes the most significant set bit in `native`.

* **`AUI_TYPE aui_lsbn(AUI_TYPE native)`** 
	* Assertions: `native` must be greater than 0.
	* Returns: The index of the least significant set bit in `native`, where the least significant bit has index 0.

	Indexes the least significant set bit in `native`.

* **`unsigned long aui_msba(const struct aui *x)`**
	* **`ULONG_MAX` is the reserved return value if an arbitrary unsigned integer structure pointed to by `x` represents the value 0 (where no bits are set; where bit indexing is not possible).**
	* Assertions: `x` must not be a pointer to `NULL`.
	* Returns: The index of the most significant set bit in the arbitrary unsigned integer structure pointed to by `x`. If the value represented by `x` equals 0, `ULONG_MAX` is returned.

	Indexes the most significant set bit in `x`.

* **`unsigned long aui_lsba(const struct aui *x)`**
	* **`ULONG_MAX` is the reserved return value if an arbitrary unsigned integer structure pointed to by `x` represents the value 0 (where no bits are set; where bit indexing is not possible).**
	* Assertions: `x` must not be a pointer to `NULL`.
	* Returns: The index of the least significant set bit in the arbitrary unsigned integer structure pointed to by `x`. If the value represented by `x` equals 0, `ULONG_MAX` is returned.

	Indexes the least significant set bit in `x`.

* **`struct aui *aui_alloc(unsigned int length)`**
	* Assertions: `length` must be greater than 0.
	* Returns: If successful, a pointer to an arbitrary unsigned integer structure with an array of type `AUI_TYPE` with the length `length`. If unsuccessful, `NULL`.

	Allocates memory for an arbitrary unsigned integer structure.

	The value represented by the arbitrary unsigned integer structure pointed to by the returned pointer is arbitrary.

* **`void aui_free(struct aui *x)`**
	* Assertions: `x` must not be a pointer to `NULL`.
	* Returns: None.

	Frees the memory of an arbitrary unsigned integer structure pointed to by `x`.

* **`struct aui *aui_pull(unsigned int length)`**
	* Assertions: `length` must be greater than 0.
	* Returns: If successful, a pointer to an arbitrary unsigned integer structure with an array of type `AUI_TYPE` with the length `length`. If unsuccessful, `NULL`.

	'Pulls' a previously allocated arbitrary unsigned integer structure from an internal sorted linked list, whose array of type `AUI_TYPE` has the length `length`. If none exists, calls `aui_alloc()`.

	The value represented by the arbitrary unsigned integer structure pointed to by the returned pointer is arbitrary.

* **`void aui_push(struct aui *x)`**
	* Assertions: `x` must not be a pointer to `NULL`.
	* Returns: None.

	'Pushes' an allocated arbitrary unsigned integer structure to an internal sorted linked list for reuse.

* **`void aui_wipe(void)`**
	* Assertions: None.
	* Returns: None.

	'Wipes' (frees) the memory of every arbitrary unsigned integer structure in the internal linked list (which is used by `aui_pull()` and `aui_push()`.

* **`void aui_seti(struct aui *x, unsigned long value)`**
	* Assertions: `x` must not be a pointer to `NULL`.
	* Returns: None.

	Assign the value of the native variable `value` to the arbitrary unsigned integer structure pointed to by `x`.

	If the value of `value` is too large to be represented in the arbitrary unsigned integer structure the most significant bit(s) of `value` that don't fit are discarded.

* **`int aui_sets(struct aui *x, const char *string, const char *set, int base)`**
	* Assertions: `x` must not be a pointer to `NULL`. `string` must not be a pointer to `NULL`. `set` must not be a pointer to `NULL`. `base` must be greater than or equal to 2. `base` must be less than or equal to `CHAR_MAX`.
	* Returns: 1 (true) if successful, otherwise 0 (false).

	Assigns the value represented by the string `string`, with the base (radix) `base` of the character set `set`, to the arbtirary unsigned integer structure pointed to by `x`.

	**TODO**

	Example:
	```
	int binary_base = 2;
	char binary_set[binary_base] = {'0', '1'};
	struct aui *x = aui_alloc(1);
	aui_sets(x, "01101001", binary_set, binary_base);
	```

* **`unsigned long aui_geti(const struct aui *x)`**
	* Assertions: `x` must not be a pointer to `NULL`.
	* Returns: The value represented by the arbitrary unsigned integer structure pointed to by `x`.

	Returns the value represented by the arbitrary unsigned integer structure pointed to by `x`.

	If the value is too large to fit in an `unsigned long`, the most significant bit(s) of the value that don't fit are discarded.

* **`int aui_gets(char *string, size_t size, const char *set, int base, const struct aui *x)`**
	* Assertions: `string` must not be a pointer to `NULL`. `size` must be greater than 0. `set` must not be a pointer to `NULL`. `base` must be greater than or equal to 2. `base` mut be less than or equal to `CHAR_MAX`. `x` must not be a pointer to `NULL`.
	* Returns: 1 (true) if successful, otherwise 0 (false).

	Assigns the value represented by the arbitrary unsigned integer structure pointed to by `x` to the string `string` with the size `size` in bytes, with the base (radix) `base` of the character set `set`.

	If the value represented by `x` is too large to be represented by `string` the most significant bit(s) that don't fit are discared.

	**TODO: Fix implementation.**

* **`void aui_asgn(struct aui *x, const struct aui *y)`**
	* Assertions: `x` must not be a pointer to `NULL`. `y` must not be a pointer to `NULL`.
	* Returns: None.

	Assigns the value represented by the arbitrary unsigned integer structure pointed to by `y` to that of `x`.

	If the value represented by `y` is too large to be represented by `x` the most significant bit(s) that don't fit are discarded.
