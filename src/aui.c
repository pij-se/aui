/*
 * aui.c
 * 2019-02-23
 *
 * Arbitrary unsigned integer library
 * Copyright (c) 2019 Johan Palm <johan@pij.se>
 * Published under the MIT license.
 */

#include "aui.h"
#include <assert.h>
#include <stdlib.h>
#include <signal.h>

static AUI_TYPE shift;
static AUI_TYPE debruijn;
static AUI_TYPE most[AUI_TYPE_BIT];
static AUI_TYPE least[AUI_TYPE_BIT];
static struct aui *list = NULL;

static int chartoint(char character, const char *set, int base)
{
	assert(set);
	assert(base >= 2);
	assert(base <= CHAR_MAX);

	while (base--)
		if (set[base] == character)
			return base;
	return base;
}

void aui_init(void)
{
	unsigned int i;
	AUI_TYPE mask;

	assert(AUI_TYPE_BIT == (sizeof(AUI_TYPE) * CHAR_BIT));
	assert(AUI_TYPE_MAX == ((AUI_TYPE)-1));
	shift = 0;
	i = AUI_TYPE_BIT;
	while (i >>= 1)
		shift++;
	assert(AUI_TYPE_BIT == (1 << shift));
	shift = AUI_TYPE_BIT - shift;
	debruijn = 0;
	i = AUI_TYPE_BIT;
	while (i--)
		most[i] = !i;
	mask = AUI_TYPE_MAX >> shift;
	i = shift;
	while (i--) {
		debruijn = (debruijn << 1) | 1;
		if (most[debruijn & mask])
			debruijn ^= 1;
		most[debruijn & mask] = 1;
	}
	i = AUI_TYPE_BIT;
	while (i--) {
		most[(AUI_TYPE)((AUI_TYPE_MAX >> (AUI_TYPE_BIT - 1 - i)) * debruijn) >> shift] = i;
		least[(AUI_TYPE)((1 << i) * debruijn) >> shift] = i;
	}
	return;
}

AUI_TYPE aui_msbb(AUI_TYPE binary)
{
	assert(binary);
	binary |= binary >> 1;
#if (AUI_TYPE_BIT >= 4)
	binary |= binary >> 2;
#if (AUI_TYPE_BIT >= 8)
	binary |= binary >> 4;
#if (AUI_TYPE_BIT >= 16)
	binary |= binary >> 8;
#if (AUI_TYPE_BIT >= 32)
	binary |= binary >> 16;
#if (AUI_TYPE_BIT >= 64)
	binary |= binary >> 32;
#if (AUI_TYPE_BIT >= 128)
	binary |= binary >> 64;
#if (AUI_TYPE_BIT >= 256)
	binary |= binary >> 128;
#if (AUI_TYPE_BIT >= 512)
	binary |= binary >> 256;
	/* ... */
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
	return most[(AUI_TYPE)(binary * debruijn) >> shift];
}

AUI_TYPE aui_lsbb(AUI_TYPE binary)
{
	assert(binary);
	return least[(AUI_TYPE)((binary & (-binary)) * debruijn) >> shift];
}

unsigned long aui_msba(const struct aui *x)
{
	unsigned int i;

	assert(x);
	i = x->length;
	while (i && !x->array[--i]);
	if (x->array[i])
		return (AUI_TYPE_BIT * i + aui_msbb(x->array[i]));
	return ULONG_MAX;
}

unsigned long aui_lsba(const struct aui *x)
{
	unsigned int i;

	assert(x);
	i = 0;
	while ((i < x->length) && !x->array[i])
		i++;
	if ((i < x->length) && x->array[i])
		return (AUI_TYPE_BIT * i + aui_lsbb(x->array[i]));
	return ULONG_MAX;
}

struct aui *aui_alloc(unsigned int length)
{
	struct aui *x;

	assert(length);
	if (!(x = malloc(sizeof(*x))))
		return NULL;
	if (!(x->array = malloc(sizeof(*x->array) * length))) {
		free(x);
		return NULL;
	}
	x->length = length;
	return x;
}

void aui_free(struct aui *x)
{
	assert(x);
	free(x->array);
	free(x);
	return;
}

struct aui *aui_pull(unsigned int length)
{
	struct aui *x;
	struct aui *y;

	assert(length);
	x = list;
	while (x && (x->length > length)) {
		y = x;
		x = x->next;
	}
	if (x && (x->length == length)) {
		if (x == list)
			list = list->next;
		else
			y->next = x->next;
		return x;
	}
	return aui_alloc(length);
}

void aui_push(struct aui *x)
{
	struct aui *y;

	assert(x);
	x->next = list;
	while (x->next && (x->next->length > x->length)) {
		y = x->next;
		x->next = x->next->next;
	}
	if (x->next == list)
		list = x;
	else
		y->next = x;
	return;
}

void aui_wipe(void)
{
	struct aui *x;

	while ((x = list)) {
		list = list->next;
		aui_free(x);
	}
	return;
}

void aui_seti(struct aui *x, unsigned long value)
{
	unsigned int i;

	assert(x);
	i = x->length;
	while (i > (sizeof(value) / sizeof(*x->array)))
		x->array[--i] = 0;
	while (i--)
		x->array[i] = value >> (AUI_TYPE_BIT * i);
	return;
}

int aui_sets(struct aui *x, const char *string, const char *set, int base)
{
	struct aui *a;
	struct aui *b;
	int result;
	int value;
	size_t i;

	assert(x);
	assert(string);
	assert(set);
	assert(base >= 2);
	assert(base <= CHAR_MAX);
	if (!(a = aui_pull(AUI_SIZTOLEN(sizeof(*set)))))
		return 0;
	result = 0;
	if (!(b = aui_pull(x->length)))
		goto push_a;
	aui_seti(x, 0);
	aui_seti(a, base);
	i = 0;
	while ((value = chartoint(string[i], set, base)) >= 0) {
		aui_seti(b, value);
		if (!aui_mul(x, a))
			goto push_b;
		if (!aui_add(x, b))
			goto push_b;
		i++;
	}
	result = 1;
push_b:
	aui_push(b);
push_a:
	aui_push(a);
	return result;
}

unsigned long aui_geti(const struct aui *x)
{
	unsigned long value;
	unsigned int i;

	assert(x);
	value = 0;
	i = sizeof(value) / sizeof(*x->array);
	i = (x->length < i) ? x->length : i;
	while (i--)
		value |= (unsigned long)x->array[i] << (AUI_TYPE_BIT * i);
	return value;
}

int aui_gets(char *string, size_t size, const char *set, int base, const struct aui *x)
{
	struct aui *a;
	struct aui *b;
	struct aui *c;
	int result;
	size_t i;
	size_t j;
	char character;

	assert(string);
	assert(size);
	assert(set);
	assert(base >= 2);
	assert(base <= CHAR_MAX);
	assert(x);
	if (!(a = aui_pull(AUI_SIZTOLEN(sizeof(*set)))))
		return 0;
	result = 0;
	if (!(b = aui_pull(x->length)))
		goto push_a;
	if (!(c = aui_pull(x->length)))
		goto push_b;
	aui_seti(a, base);
	aui_asgn(b, x);
	i = 0;
	while (aui_eval(b) && (i < size)) {
		aui_asgn(c, b);
		if (!aui_mod(c, a))
			goto push_c;
		if (!aui_div(b, a))
			goto push_c;
		string[i++] = set[aui_geti(c)];
	}
	string[i] = '\0';
	if (i--) {
		j = 0;
		while (j < i) {
			character = string[i];
			string[i--] = string[j];
			string[j++] = character;
		}
	}
	result = 1;
push_c:
	aui_push(c);
push_b:
	aui_push(b);
push_a:
	aui_push(a);
	return result;
}

void aui_asgn(struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = x->length;
	while (i > y->length)
		x->array[--i] = 0;
	while (i--)
		x->array[i] = y->array[i];
	return;
}

void aui_swap(struct aui *x, struct aui *y)
{
	unsigned int i;
	AUI_TYPE *array;
	AUI_TYPE binary;

	assert(x);
	assert(y);
	if (x->length == y->length) {
		array = x->array;
		x->array = y->array;
		y->array = array;
		return;
	}
	i = (x->length > y->length) ? x->length : y->length;
	while (i > y->length)
		x->array[--i] = 0;
	while (i > x->length)
		y->array[--i] = 0;
	while (i--) {
		binary = x->array[i];
		x->array[i] = y->array[i];
		y->array[i] = binary;
	}
	return;
}

int aui_eq(const struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = (x->length > y->length) ? x->length : y->length;
	while (i > y->length)
		if (x->array[--i])
			return 0;
	while (i > x->length)
		if (y->array[--i])
			return 0;
	while (i--)
		if (x->array[i] != y->array[i])
			return 0;
	return 1;
}

int aui_neq(const struct aui *x, const struct aui *y)
{
	assert(x);
	assert(y);
	return !aui_eq(x, y);
}

int aui_lt(const struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = (x->length > y->length) ? x->length : y->length;
	while (i > y->length)
		if (x->array[--i])
			return 0;
	while (i > x->length)
		if (y->array[--i])
			return 1;
	while (i--)
		if (x->array[i] != y->array[i])
			return (x->array[i] < y->array[i]);
	return 0;
}

int aui_lte(const struct aui *x, const struct aui *y)
{
	assert(x);
	assert(y);
	return !aui_gt(x, y);
}

int aui_gt(const struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = (x->length > y->length) ? x->length : y->length;
	while (i > y->length)
		if (x->array[--i])
			return 1;
	while (i > x->length)
		if (y->array[--i])
			return 0;
	while (i--)
		if (x->array[i] != y->array[i])
			return (x->array[i] > y->array[i]);
	return 0;
}

int aui_gte(const struct aui *x, const struct aui *y)
{
	assert(x);
	assert(y);
	return !aui_lt(x, y);
}

int aui_eval(const struct aui *x)
{
	unsigned int i;

	assert(x);
	i = 0;
	while (i < x->length)
		if (x->array[i++])
			return 1;
	return 0;
}

void aui_one(struct aui *x)
{
	unsigned int i;

	assert(x);
	i = x->length;
	while (i--)
		x->array[i] = ~x->array[i];
	return;
}

void aui_two(struct aui *x)
{
	assert(x);
	aui_one(x);
	aui_inc(x);
	return;
}

void aui_and(struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = x->length;
	while (i > y->length)
		x->array[--i] = 0;
	while (i--)
		x->array[i] &= y->array[i];
	return;
}

void aui_ior(struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = (x->length < y->length) ? x->length : y->length;
	while (i--)
		x->array[i] |= y->array[i];
	return;
}

void aui_xor(struct aui *x, const struct aui *y)
{
	unsigned int i;

	assert(x);
	assert(y);
	i = (x->length < y->length) ? x->length : y->length;
	while (i--)
		x->array[i] ^= y->array[i];
	return;
}

void aui_shl(struct aui *x, unsigned long shift)
{
	AUI_TYPE bits;
	AUI_TYPE delta;
	unsigned int i;
	unsigned int j;

	assert(x);
	if (!shift)
		return;
	bits = shift % AUI_TYPE_BIT;
	shift /= AUI_TYPE_BIT;
	i = x->length;
	if (shift >= x->length)
		goto while_fill;
	delta = AUI_TYPE_BIT - bits;
	j = i - shift - 1;
	if (delta != AUI_TYPE_BIT) {
		goto while_shift;
		while (j) {
			x->array[i] |= x->array[--j] >> delta;
while_shift:
			x->array[--i] = x->array[j] << bits;
		}
	} else {
		goto while_zero;
		while (j) {
			x->array[i] = 0, j--;
while_zero:
			x->array[--i] = x->array[j] << bits;
		}
	}
while_fill:
	while (i--)
		x->array[i] = 0;
	return;
}

void aui_shr(struct aui *x, unsigned long shift)
{
	AUI_TYPE bits;
	AUI_TYPE delta;
	unsigned int i;
	unsigned int j;

	assert(x);
	if (!shift)
		return;
	bits = shift % AUI_TYPE_BIT;
	shift /= AUI_TYPE_BIT;
	i = 0;
	if (shift >= x->length)
		goto while_fill;
	delta = AUI_TYPE_BIT - bits;
	j = shift;
	if (delta != AUI_TYPE_BIT) {
		goto while_shift;
		while (j < x->length) {
			x->array[i++] |= x->array[j] << delta;
while_shift:
			x->array[i] = x->array[j++] >> bits;
		}
	} else {
		goto while_zero;
		while (j < x->length) {
			x->array[i++] = 0;
while_zero:
			x->array[i] = x->array[j++] >> bits;
		}
	}
	i++;
while_fill:
	while (i < x->length)
		x->array[i++] = 0;
	return;
}

void aui_inc(struct aui *x)
{
	unsigned int i;
	AUI_TYPE index;

	assert(x);
	i = 0;
	while ((i < x->length) && (x->array[i] == AUI_TYPE_MAX))
		i++;
	if (i < x->length) {
		index = aui_lsbb(~x->array[i]);
		x->array[i] = ((x->array[i] >> index) | 1) << index;
	}
	while (i--)
		x->array[i] = 0;
	return;
}

void aui_dec(struct aui *x)
{
	unsigned int i;
	AUI_TYPE index;

	assert(x);
	i = 0;
	while ((i < x->length) && (x->array[i] == 0))
		i++;
	if (i < x->length) {
		index = aui_lsbb(x->array[i]);
		x->array[i] ^= (AUI_TYPE_MAX >> (AUI_TYPE_BIT - 1 - index));
	}
	while (i--)
		x->array[i] = AUI_TYPE_MAX;
	return;
}

int aui_add(struct aui *x, const struct aui *y)
{
	unsigned int length;
	struct aui *a;
	struct aui *b;
	int result;

	assert(x);
	assert(y);
	length = (x->length > y->length) ? x->length : y->length;
	if (!(a = aui_pull(length)))
		return 0;
	result = 0;
	if (!(b = aui_pull(length)))
		goto push_a;
	aui_asgn(a, y);
	while (aui_eval(a)) {
		aui_asgn(b, x);
		aui_and(b, a);
		aui_shl(b, 1);
		aui_xor(x, a);
		aui_asgn(a, b);
	}
	result = 1;
	aui_push(b);
push_a:
	aui_push(a);
	return result;
}

int aui_sub(struct aui *x, const struct aui *y)
{
	unsigned int length;
	struct aui *a;
	struct aui *b;
	int result;

	assert(x);
	assert(y);
	length = (x->length > y->length) ? x->length : y->length;
	if (!(a = aui_pull(length)))
		return 0;
	result = 0;
	if (!(b = aui_pull(length)))
		goto push_a;
	aui_asgn(a, y);
	aui_two(a);
	while (aui_eval(a)) {
		aui_asgn(b, x);
		aui_and(b, a);
		aui_shl(b, 1);
		aui_xor(x, a);
		aui_asgn(a, b);
	}
	result = 1;
	aui_push(b);
push_a:
	aui_push(a);
	return result;
}

int aui_mul(struct aui *x, const struct aui *y)
{
	unsigned int length;
	struct aui *a;
	struct aui *b;
	int result;

	assert(x);
	assert(y);
	length = (x->length > y->length) ? x->length : y->length;
	if (!(a = aui_pull(length)))
		return 0;
	result = 0;
	if (!(b = aui_pull(length)))
		goto push_a;
	aui_asgn(a, y);
	aui_seti(b, 0);
	aui_swap(x, b);
	while (aui_eval(a)) {
		if ((a->array[0] & 1) && !aui_add(x, b))
			goto push_b;
		aui_shl(b, 1);
		aui_shr(a, 1);
	}
	result = 1;
push_b:
	aui_push(b);
push_a:
	aui_push(a);
	return result;
}

int aui_div(struct aui *x, const struct aui *y)
{
	unsigned long msby;
	unsigned long msbx;
	unsigned int length;
	struct aui *a;
	struct aui *b;
	int result;

	assert(x);
	assert(y);
	if ((msby = aui_msba(y)) == ULONG_MAX) {
		raise(SIGFPE);
		return 0;
	}
	if (((msbx = aui_msba(x)) == ULONG_MAX) || (msbx <= msby)) {
		aui_seti(x, aui_gte(x, y));
		return 1;
	}
	length = (x->length > y->length) ? x->length : y->length;
	if (!(a = aui_pull(length)))
		return 0;
	result = 0;
	if (!(b = aui_pull(length)))
		goto push_a;
	aui_asgn(a, x);
	aui_seti(x, 0);
	while (aui_gte(a, y)) {
		msbx = aui_msba(a) - msby;
		aui_asgn(b, y);
		aui_shl(b, msbx);
		if (aui_lt(a, b)) {
			aui_shr(b, 1);
			msbx--;
		}
		x->array[msbx / AUI_TYPE_BIT] |= 1 << (msbx % AUI_TYPE_BIT);
		if (!aui_sub(a, b))
			goto push_b;
	}
	result = 1;
push_b:
	aui_push(b);
push_a:
	aui_push(a);
	return result;
}

int aui_mod(struct aui *x, const struct aui *y)
{
	unsigned long msby;
	unsigned long msbx;
	unsigned int length;
	struct aui *a;
	int result;

	assert(x);
	assert(y);
	if ((msby = aui_msba(y)) == ULONG_MAX) {
		raise(SIGFPE);
		return 0;
	}
	if (((msbx = aui_msba(x)) == ULONG_MAX) || (msbx < msby))
		return 1;
	length = (x->length > y->length) ? x->length : y->length;
	if (!(a = aui_pull(length)))
		return 0;
	result = 0;
	while (aui_gte(x, y)) {
		aui_asgn(a, y);
		aui_shl(a, aui_msba(x) - msby);
		if (aui_lt(x, a))
			aui_shr(a, 1);
		if (!aui_sub(x, a))
			goto push_a;
	}
	result = 1;
push_a:
	aui_push(a);
	return result;
}
