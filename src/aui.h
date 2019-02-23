/*
 * aui.h
 * 2019-02-23
 *
 * Arbitrary unsigned integer library
 * Copyright (c) 2019 Johan Palm <johan@pij.se>
 * All rights reserved.
 */

#ifndef AUI_H
#define AUI_H
#include <limits.h>
#include <stddef.h>

#define AUI_TYPE unsigned char
#define AUI_TYPE_BIT CHAR_BIT
#define AUI_TYPE_MAX UCHAR_MAX
#define AUI_SIZTOLEN(size) \
	((size) / sizeof(AUI_TYPE) + !!((size) % sizeof(AUI_TYPE)))

struct aui {
	AUI_TYPE *array;
	unsigned int length;
	struct aui *next;
};

void aui_init(void);

AUI_TYPE aui_msbb(AUI_TYPE binary);
AUI_TYPE aui_lsbb(AUI_TYPE binary);

unsigned long aui_msba(const struct aui *x);
unsigned long aui_lsba(const struct aui *y);

struct aui *aui_alloc(unsigned int length);
void aui_free(struct aui *x);

struct aui *aui_pull(unsigned int length);
void aui_push(struct aui *x);
void aui_wipe(void);

void aui_seti(struct aui *x, unsigned long value);
int aui_sets(struct aui *x, const char *string, const char *set, int base);

unsigned long aui_geti(const struct aui *x);
int aui_gets(char *string, size_t size, const char *set, int base, const struct aui *x);

void aui_asgn(struct aui *x, const struct aui *y);
void aui_swap(struct aui *x, struct aui *y);

int aui_eq(const struct aui *x, const struct aui *y);
int aui_neq(const struct aui *x, const struct aui *y);
int aui_lt(const struct aui *x, const struct aui *y);
int aui_lte(const struct aui *x, const struct aui *y);
int aui_gt(const struct aui *x, const struct aui *y);
int aui_gte(const struct aui *x, const struct aui *y);
int aui_eval(const struct aui *x);

void aui_one(struct aui *x);
void aui_two(struct aui *x);
void aui_and(struct aui *x, const struct aui *y);
void aui_ior(struct aui *x, const struct aui *y);
void aui_xor(struct aui *x, const struct aui *y);
void aui_shl(struct aui *x, unsigned long shift);
void aui_shr(struct aui *x, unsigned long shift);

void aui_inc(struct aui *x);
void aui_dec(struct aui *x);
int aui_add(struct aui *x, const struct aui *y);
int aui_sub(struct aui *x, const struct aui *y);
int aui_mul(struct aui *x, const struct aui *y);
int aui_div(struct aui *x, const struct aui *y);
int aui_mod(struct aui *x, const struct aui *y);

#endif
