/*
 * test.c
 * 2019-02-25
 *
 * Arbitrary unsigned integer library
 * Copyright (c) 2018 Johan Palm <johan@pij.se>
 * Published under the MIT license.
 */

#include "aui.h"
#include <assert.h>
#include <stdio.h>
#include <stddef.h>

unsigned long msb(unsigned long binary)
{
	unsigned long i;

	assert(binary);
	i = 0;
	while (binary >>= 1)
		i++;
	return i;
}

unsigned long lsb(unsigned long binary)
{
	unsigned long i;

	assert(binary);
	i = 0;
	while (!(binary & 1)) {
		binary >>= 1;
		i++;
	}
	return i;
}

void init_msbn_lsbn(void)
{
	AUI_TYPE i;
	AUI_TYPE j;
	AUI_TYPE k;

	printf("init_msbn_lsbn: start\n");
	aui_init();
	i = AUI_TYPE_MAX; /* test all possible values */
	do {
		if ((j = aui_msbn(i)) != (k = msb(i)))
			printf("aui_msbn(%u) != msb(%u): %u != %u\n", i, i, j, k);
		if ((j = aui_lsbn(i)) != (k = lsb(i)))
			printf("aui_lsbn(%u) != lsb(%u): %u != %u\n", i, i, j, k);
	} while (--i);
	printf("init_msbn_lsbn: finish\n");
	return;
}

void alloc_free(void)
{
	struct aui *x;
	unsigned int length;

	printf("alloc_free: start\n");
	length = 1024; /* an arbitrary number for testing */
	do {
		if (!(x = aui_alloc(length)))
			printf("aui_alloc(%u) == NULL\n", length);
		aui_free(x);
	} while (--length);
	printf("alloc_free: finish\n");
	return;
}

void pull_push_wipe(void)
{
	struct aui *x;
	struct aui *y;
	unsigned int lengthx;
	unsigned int lengthy;

	printf("pull_push_wipe: start\n");
	lengthx = 1024; /* an arbitrary number for testing */
	do {
		lengthy = 1024; /* an arbitrary number for testing */
		do {
			if (!(x = aui_pull(lengthx))) {
				printf("(x = aui_pull(%u)) == NULL\n", lengthx);
				return;
			}
			if (!(y = aui_pull(lengthy))) {
				printf("(y = aui_pull(%u)) == NULL\n", lengthy);
				aui_push(x);
				aui_wipe();
				return;
			}
			aui_push(x);
			aui_push(y);
		} while (--lengthy);
		if (!(lengthx % 10))
			printf("pull_push_wipe: %u\n", lengthx);
	} while (--lengthx);
	aui_wipe();
	printf("pull_push_wipe: finish\n");
	return;
}

void seti_geti(void)
{
	struct aui *x;
	unsigned int i;
	unsigned int resultx;

	printf("seti_geti: start\n");
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) failed\n");
		return;
	}
	i = UINT_MAX;
	do {
		aui_seti(x, i);
		resultx = aui_geti(x);
		if (i != resultx) {
			printf("aui_seti(x, %u) or aui_geti(x): %u != %u\n", i, i, resultx);
			getchar();
		}
		if (!(i % 100000000))
			printf("seti_geti: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_wipe();
	printf("seti_geti: finish\n");
	return;
}

void asgn_swap(void)
{
	struct aui *x;
	struct aui *y;
	unsigned char i;
	unsigned char j;
	unsigned char resulti;
	unsigned char resultj;
	unsigned short k;
	unsigned short l;
	unsigned short resultk;
	unsigned short resultl;

	printf("asgn_swap: start\n");
	/* char char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...): failed\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...): failed\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		aui_asgn(y, x);
		j = aui_geti(y);
		if (i != j) {
			printf("aui_asgn(y, x) [char]: %u != %u\n", i, j);
			getchar();
		}
		j = UCHAR_MAX;
		do {
			aui_seti(x, i);
			aui_seti(y, j);
			aui_swap(x, y);
			resulti = aui_geti(x);
			resultj = aui_geti(y);
			if (resulti != j) {
				printf("aui_swap(x, y) [char]: %u != %u\n", resulti, j);
				getchar();
			}
			if (resultj != i) {
				printf("aui_swap(x, y) [char]: %u != %u\n", resultj, i);
				getchar();
			}
		} while (j--);
		if (!(i % 10))
			printf("asgn_swap [char]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...): failed\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...): failed\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	k = USHRT_MAX;
	do {
		aui_seti(x, k);
		aui_asgn(y, x);
		l = aui_geti(y);
		if (k != l) {
			printf("aui_asgn(y, x) [short]: %u != %u\n", k, l);
			getchar();
		}
		l = USHRT_MAX;
		do {
			aui_seti(x, k);
			aui_seti(y, l);
			aui_swap(x, y);
			resultk = aui_geti(x);
			resultl = aui_geti(y);
			if (resultk != l) {
				printf("aui_swap(x, y) [short]: %u != %u\n", resultk, l);
				getchar();
			}
			if (resultl != k) {
				printf("aui_swap(x, y) [short]: %u != %u\n", resultl, k);
				getchar();
			}
		} while (l--);
		if (!(k % 100))
			printf("asgn_swap [short]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	/* char short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...): failed\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...): failed\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		aui_asgn(y, x);
		l = aui_geti(y);
		if (i != l) {
			printf("aui_asgn(y, x) [char short]: %u != %u\n", i, l);
			getchar();
		}
		l = USHRT_MAX;
		do {
			aui_seti(x, i);
			aui_seti(y, l);
			aui_swap(x, y);
			resulti = aui_geti(x);
			resultl = aui_geti(y);
			if (resulti != (unsigned char)l) {
				printf("aui_swap(x, y) [char short 1]: %u != %u\n", resulti, (unsigned char)l);
				getchar();
			}
			if (resultl != i) {
				printf("aui_swap(x, y) [char short 2]: %u != %u\n", resultl, i);
				getchar();
			}
		} while (l--);
		if (!(i % 10))
			printf("asgn_swap [char short]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...): failed\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...): failed\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	k = USHRT_MAX;
	do {
		aui_seti(x, k);
		aui_asgn(y, x);
		j = aui_geti(y);
		if ((unsigned char)k != j) {
			printf("aui_asgn(y, x) [short char]: %u != %u\n", k, j);
			getchar();
		}
		j = UCHAR_MAX;
		do {
			aui_seti(x, k);
			aui_seti(y, j);
			aui_swap(x, y);
			resultk = aui_geti(x);
			resultj = aui_geti(y);
			if (resultk != j) {
				printf("aui_swap(x, y) [short char 1]: %u != %u\n", resultk, j);
				getchar();
			}
			if (resultj != (unsigned char)k) {
				printf("aui_swap(x, y) [short char 2]: %u != %u\n", resultj, (unsigned char)k);
				getchar();
			}
		} while (j--);
		if (!(k % 100))
			printf("asgn_swap [short char]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	aui_wipe();
	printf("asgn_swap: finish\n");
	return;
}

void eq_neq_lt_lte_gt_gte_eval(void)
{
	struct aui *x;
	struct aui *y;
	unsigned char i;
	unsigned char j;
	unsigned short k;
	unsigned short l;
	int result;

	printf("eq_neq_lt_lte_gt_gte_eval: start\n");
	/* char char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char char] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [char char] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		result = aui_eval(x);
		if ((!!i) != result) {
			printf("aui_eval(%u) [char char]\n", i);
			getchar();
		}
		j = UCHAR_MAX;
		do {
			aui_seti(y, j);
			result = aui_eq(x, y);
			if ((i == j) != result) {
				printf("aui_eq(%u, %u) [char char]\n", i, j);
				getchar();
			}
			result = aui_neq(x, y);
			if ((i != j) != result) {
				printf("aui_neq(%u, %u) [char char]\n", i, j);
				getchar();
			}
			result = aui_lt(x, y);
			if ((i < j) != result) {
				printf("aui_lt(%u, %u) [char char]\n", i, j);
				getchar();
			}
			result = aui_lte(x, y);
			if ((i <= j) != result) {
				printf("aui_lte(%u, %u) [char char]\n", i, j);
				getchar();
			}
			result = aui_gt(x, y);
			if ((i > j) != result) {
				printf("aui_gt(%u, %u) [char char]\n", i, j);
				getchar();
			}
			result = aui_gte(x, y);
			if ((i >= j) != result) {
				printf("aui_gte(%u, %u) [char char]\n", i, j);
				getchar();
			}
		} while (j--);
		if (!(i % 10))
			printf("eq_neq_lt_lte_gt_gte_eval [char char]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...) [short short] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...) [short short] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	k = USHRT_MAX;
	do {
		aui_seti(x, k);
		result = aui_eval(x);
		if ((!!k) != result) {
			printf("aui_eval(%u) [short short]\n", k);
			getchar();
		}
		l = USHRT_MAX;
		do {
			aui_seti(y, l);
			result = aui_eq(x, y);
			if ((k == l) != result) {
				printf("aui_eq(%u, %u) [short short]\n", k, l);
				getchar();
			}
			result = aui_neq(x, y);
			if ((k != l) != result) {
				printf("aui_neq(%u, %u) [short short]\n", k, l);
				getchar();
			}
			result = aui_lt(x, y);
			if ((k < l) != result) {
				printf("aui_lt(%u, %u) [short short]\n", k, l);
				getchar();
			}
			result = aui_lte(x, y);
			if ((k <= l) != result) {
				printf("aui_lte(%u, %u) [short short]\n", k, l);
				getchar();
			}
			result = aui_gt(x, y);
			if ((k > l) != result) {
				printf("aui_gt(%u, %u) [short short]\n", k, l);
				getchar();
			}
			result = aui_gte(x, y);
			if ((k >= l) != result) {
				printf("aui_gte(%u, %u) [short short]\n", k, l);
				getchar();
			}
		} while (l--);
		if (!(k % 100))
			printf("eq_neq_lt_lte_gt_gte_eval [short short]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	/* char short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char short] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...) [char short] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		l = USHRT_MAX;
		do {
			aui_seti(y, l);
			result = aui_eq(x, y);
			if ((i == l) != result) {
				printf("aui_eq(%u, %u) [char short]\n", i, l);
				getchar();
			}
			result = aui_neq(x, y);
			if ((i != l) != result) {
				printf("aui_neq(%u, %u) [char short]\n", i, l);
				getchar();
			}
			result = aui_lt(x, y);
			if ((i < l) != result) {
				printf("aui_lt(%u, %u) [char short]\n", i, l);
				getchar();
			}
			result = aui_lte(x, y);
			if ((i <= l) != result) {
				printf("aui_lte(%u, %u) [char short]\n", i, l);
				getchar();
			}
			result = aui_gt(x, y);
			if ((i > l) != result) {
				printf("aui_gt(%u, %u) [char short]\n", i, l);
				getchar();
			}
			result = aui_gte(x, y);
			if ((i >= l) != result) {
				printf("aui_gte(%u, %u) [char short]\n", i, l);
				getchar();
			}
		} while (l--);
		if (!(i % 10))
			printf("eq_neq_lt_lte_gt_gte_eval [char short]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...) [short char] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [short char] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	k = USHRT_MAX;
	do {
		aui_seti(x, k);
		j = UCHAR_MAX;
		do {
			aui_seti(y, j);
			result = aui_eq(x, y);
			if ((k == j) != result) {
				printf("aui_eq(%u, %u) [short char]\n", k, j);
				getchar();
			}
			result = aui_neq(x, y);
			if ((k != j) != result) {
				printf("aui_neq(%u, %u) [short char]\n", k, j);
				getchar();
			}
			result = aui_lt(x, y);
			if ((k < j) != result) {
				printf("aui_lt(%u, %u) [short char]\n", k, j);
				getchar();
			}
			result = aui_lte(x, y);
			if ((k <= j) != result) {
				printf("aui_lte(%u, %u) [short char]\n", k, j);
				getchar();
			}
			result = aui_gt(x, y);
			if ((k > j) != result) {
				printf("aui_gt(%u, %u) [short char]\n", k, j);
				getchar();
			}
			result = aui_gte(x, y);
			if ((k >= j) != result) {
				printf("aui_gte(%u, %u) [short char]\n", k, j);
				getchar();
			}
		} while (j--);
		if (!(k % 100))
			printf("eq_neq_lt_lte_gt_gte_eval [short char]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	aui_wipe();
	printf("eq_neq_lt_lte_gt_gte_eval: finish\n");
	return;
}

void msba_lsba(void)
{
	struct aui *x;
	unsigned char i;
	unsigned short j;
	unsigned int resulta;
	unsigned int resultb;

	printf("msba_lsba: start\n");
	aui_init();
	/* char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char] == NULL\n");
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		resulta = aui_msba(x);
		resultb = msb(i);
		if (resulta != resultb) {
			printf("aui_msba(%u) [char]: %u != %u\n", i, resulta, resultb);
			getchar();
		}
		resulta = aui_lsba(x);
		resultb = lsb(i);
		if (resulta != resultb) {
			printf("aui_lsba(%u) [char]: %u != %u\n", i, resulta, resultb);
			getchar();
		}
		if (!(i % 10))
			printf("msba_lsba [char]: %u\n", i);
	} while (--i);
	aui_push(x);
	/* short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [short] == NULL\n");
		return;
	}
	j = USHRT_MAX;
	do {
		aui_seti(x, j);
		resulta = aui_msba(x);
		resultb = msb(j);
		if (resulta != resultb) {
			printf("aui_msba(%u) [short]: %u != %u\n", j, resulta, resultb);
			getchar();
		}
		resulta = aui_lsba(x);
		resultb = lsb(j);
		if (resulta != resultb) {
			printf("aui_lsba(%u) [short]: %u != %u\n", j, resulta, resultb);
			getchar();
		}
		if (!(j % 100))
			printf("msba_lsba [short]: %u\n", j);
	} while (--j);
	aui_push(x);
	aui_wipe();
	printf("msba_lsba: finish\n");
	return;
}

void one_two_and_ior_xor(void)
{
	struct aui *x;
	struct aui *y;
	unsigned char i;
	unsigned char j;
	unsigned char resultc;
	unsigned short k;
	unsigned short l;
	unsigned short results;

	printf("one_two_and_ior_xor: start\n");
	aui_init();
	/* char char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char char] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [char char] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		aui_one(x);
		resultc = aui_geti(x);
		if (resultc != (unsigned char)~i) {
			printf("aui_one(%u) [char char]: %u != %u\n", i, resultc, (unsigned char)~i);
			getchar();
		}
		aui_seti(x, i);
		aui_two(x);
		resultc = aui_geti(x);
		if (resultc != (unsigned char)((~i)+1)) {
			printf("aui_two(%u) [char char]: %u != %u\n", i, resultc, (unsigned char)((~i)+1));
			getchar();
		}
		j = UCHAR_MAX;
		do {
			aui_seti(y, j);
			/* and */
			aui_seti(x, i);
			aui_and(x, y);
			resultc = aui_geti(x);
			if (resultc != (i & j)) {
				printf("aui_and(%u, %u) [char char]: %u != %u\n", i, j, resultc, (i & j));
				getchar();
			}
			/* ior */
			aui_seti(x, i);
			aui_ior(x, y);
			resultc = aui_geti(x);
			if (resultc != (i | j)) {
				printf("aui_ior(%u, %u) [char char]: %u != %u\n", i, j, resultc, (i | j));
				getchar();
			}
			/* xor */
			aui_seti(x, i);
			aui_xor(x, y);
			resultc = aui_geti(x);
			if (resultc != (i ^ j)) {
				printf("aui_xor(%u, %u) [char char]: %u != %u\n", i, j, resultc, (i ^ j));
				getchar();
			}
		} while (j--);
		if (!(i % 10))
			printf("one_two_and_ior_xor [char char]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...) [short short] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...) [short short] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	k = USHRT_MAX;
	do {
		aui_seti(x, k);
		aui_one(x);
		results = aui_geti(x);
		if (results != (unsigned short)~k) {
			printf("aui_one(%u) [short short]: %u != %u\n", k, results, (unsigned short)~k);
			getchar();
		}
		aui_seti(x, k);
		aui_two(x);
		results = aui_geti(x);
		if (results != (unsigned short)((~k)+1)) {
			printf("aui_two(%u) [short short]: %u != %u\n", i, results, (unsigned short)((~k)+1));
			getchar();
		}
		l = USHRT_MAX;
		do {
			aui_seti(y, l);
			/* and */
			aui_seti(x, k);
			aui_and(x, y);
			results = aui_geti(x);
			if (results != (k & l)) {
				printf("aui_and(%u, %u) [short short]: %u != %u\n", k, j, results, (k & l));
				getchar();
			}
			/* ior */
			aui_seti(x, k);
			aui_ior(x, y);
			results = aui_geti(x);
			if (results != (k | l)) {
				printf("aui_ior(%u, %u) [short short]: %u != %u\n", k, l, results, (k | l));
				getchar();
			}
			/* xor */
			aui_seti(x, k);
			aui_xor(x, y);
			results = aui_geti(x);
			if (results != (k ^ l)) {
				printf("aui_xor(%u, %u) [short short]: %u != %u\n", k, l, results, (k ^ l));
				getchar();
			}
		} while (l--);
		if (!(k % 100))
			printf("one_two_and_ior_xor [short short]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	/* char short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char short] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...) [char short] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	i = UCHAR_MAX;
	do {
		l = USHRT_MAX;
		do {
			aui_seti(y, l);
			/* and */
			aui_seti(x, i);
			aui_and(x, y);
			resultc = aui_geti(x);
			if (resultc != (i & l)) {
				printf("aui_and(%u, %u) [char short]: %u != %u\n", i, l, resultc, (i & l));
				getchar();
			}
			/* ior */
			aui_seti(x, i);
			aui_ior(x, y);
			resultc = aui_geti(x);
			if (resultc != (unsigned char)(i | l)) {
				printf("aui_ior(%u, %u) [char short]: %u != %u\n", i, l, resultc, (unsigned char)(i | l));
				getchar();
			}
			/* xor */
			aui_seti(x, i);
			aui_xor(x, y);
			resultc = aui_geti(x);
			if (resultc != (unsigned char)(i ^ l)) {
				printf("aui_xor(%u, %u) [char short]: %u != %u\n", i, l, resultc, (unsigned char)(i ^ l));
				getchar();
			}
		} while (l--);
		if (!(i % 10))
			printf("one_two_and_ior_xor [char short]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...) [short char] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [short char] == NULL\n");
		aui_push(x);
		aui_wipe();
		return;
	}
	k = USHRT_MAX;
	do {
		j = UCHAR_MAX;
		do {
			aui_seti(y, j);
			/* and */
			aui_seti(x, k);
			aui_and(x, y);
			results = aui_geti(x);
			if (results != (k & j)) {
				printf("aui_and(%u, %u) [short char]: %u != %u\n", k, j, results, (k & j));
				getchar();
			}
			/* ior */
			aui_seti(x, k);
			aui_ior(x, y);
			results = aui_geti(x);
			if (results != (k | j)) {
				printf("aui_ior(%u, %u) [short char]: %u != %u\n", k, j, results, (k | j));
				getchar();
			}
			/* xor */
			aui_seti(x, k);
			aui_xor(x, y);
			results = aui_geti(x);
			if (results != (k ^ j)) {
				printf("aui_xor(%u, %u) [short char]: %u != %u\n", k, j, results, (k ^ j));
				getchar();
			}
		} while (j--);
		if (!(k % 100))
			printf("one_two_and_ior_xor [short char]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	aui_wipe();
	printf("one_two_and_ior_xor: finish\n");
	return;
}

void shl_shr_inc_dec(void)
{
	struct aui *x;
	unsigned char i;
	unsigned short j;
	unsigned int result;

	printf("shl_shr_inc_dec: start\n");
	aui_init();
	/* char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(1) == NULL\n");
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		aui_inc(x);
		result = aui_geti(x);
		if (result != (unsigned char)(i + 1)) {
			printf("aui_inc(%u) == %u\n", i, result);
			getchar();
		}
		aui_seti(x, i);
		aui_dec(x);
		result = aui_geti(x);
		if (result != (unsigned char)(i - 1)) {
			printf("aui_dec(%u) == %u\n", i, result);
			getchar();
		}
		j = CHAR_BIT;
		while (j--) {
			aui_seti(x, i);
			aui_shl(x, j);
			result = aui_geti(x);
			if (result != (unsigned char)(i << j)) {
				printf("aui_shl(%u, %u): %u != %u\n", i, j, result, (unsigned char)(i << j));
				getchar();
			}
			aui_seti(x, i);
			aui_shr(x, j);
			result = aui_geti(x);
			if (result != (i >> j)) {
				printf("aui_shr(%u, %u): %u != %u\n", i, j, result, (i >> j));
				getchar();
			}
		}
		if (!(i % 10))
			printf("shl_shr_inc_dec: %u\n", i);
	} while (i--);
	aui_push(x);
	/* short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(1) == NULL\n");
		return;
	}
	j = USHRT_MAX;
	do {
		aui_seti(x, j);
		aui_inc(x);
		result = aui_geti(x);
		if (result != (unsigned short)(j + 1)) {
			printf("aui_inc(%u) == %u\n", j, result);
			getchar();
		}
		aui_seti(x, j);
		aui_dec(x);
		result = aui_geti(x);
		if (result != (unsigned short)(j - 1)) {
			printf("aui_dec(%u) == %u\n", j, result);
			getchar();
		}
		i = CHAR_BIT * sizeof(j);
		while (i--) {
			aui_seti(x, j);
			aui_shl(x, i);
			result = aui_geti(x);
			if (result != (unsigned short)(j << i)) {
				printf("aui_shl(%u, %u): %u != %u\n", j, i, result, (unsigned short)(j << i));
				getchar();
			}
			aui_seti(x, j);
			aui_shr(x, i);
			result = aui_geti(x);
			if (result != (j >> i)) {
				printf("aui_shr(%u, %u): %u != %u\n", j, i, result, (j >> i));
				getchar();
			}
		}
		if (!(j % 100))
			printf("shl_shr_inc_dec: %u\n", j);
	} while (j--);
	aui_push(x);
	aui_wipe();
	printf("shl_shr_inc_dec: finish\n");
	return;
}

void add_sub_mul_div_mod(void)
{
	struct aui *x;
	struct aui *y;
	unsigned char i;
	unsigned char j;
	unsigned char resultc1;
	unsigned char resultc2;
	unsigned short k;
	unsigned short l;
	unsigned short results1;
	unsigned short results2;

	printf("add_sub_mul_div_mod: start\n");
	aui_init();
	/* char char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char char] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [char char] == NULL\n");
		return;
	}
	i = UCHAR_MAX;
	do {
		j = UCHAR_MAX;
		do {
			aui_seti(y, j);
			/* add */
			aui_seti(x, i);
			aui_add(x, y);
			resultc1 = aui_geti(x);
			resultc2 = i + j;
			if (resultc1 != resultc2) {
				printf("aui_add(%u, %u) [char char]: %u != %u\n", i, j, resultc1, resultc2);
				getchar();
			}
			/* sub */
			aui_seti(x, i);
			aui_sub(x, y);
			resultc1 = aui_geti(x);
			resultc2 = i - j;
			if (resultc1 != resultc2) {
				printf("aui_sub(%u, %u) [char char]: %u != %u\n", i, j, resultc1, resultc2);
				getchar();
			}
			/* mul */
			aui_seti(x, i);
			aui_mul(x, y);
			resultc1 = aui_geti(x);
			resultc2 = i * j;
			if (resultc1 != resultc2) {
				printf("aui_mul(%u, %u) [char char]: %u != %u\n", i, j, resultc1, resultc2);
				getchar();
			}
			if (j) {
				/* div */
				aui_seti(x, i);
				aui_div(x, y);
				resultc1 = aui_geti(x);
				resultc2 = i / j;
				if (resultc1 != resultc2) {
					printf("aui_div(%u, %u) [char char]: %u != %u\n", i, j, resultc1, resultc2);
					getchar();
				}
				/* mod */
				aui_seti(x, i);
				aui_mod(x, y);
				resultc1 = aui_geti(x);
				resultc2 = i % j;
				if (resultc1 != resultc2) {
					printf("aui_mod(%u, %u) [char char]: %u != %u\n", i, j, resultc1, resultc2);
					getchar();
				}
			}
		} while (j--);
		if (!(i % 10))
			printf("add_sub_mul_div_mod [char char]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short short */
	aui_init();
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...) [short short] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...) [short short] == NULL\n");
		return;
	}
	k = USHRT_MAX;
	do {
		l = USHRT_MAX;
		do {
			aui_seti(y, l);
			/* add */
			aui_seti(x, k);
			aui_add(x, y);
			results1 = aui_geti(x);
			results2 = k + l;
			if (results1 != results2) {
				printf("aui_add(%u, %u) [short short]: %u != %u\n", k, l, results1, results2);
				getchar();
			}
			/* sub */
			aui_seti(x, k);
			aui_sub(x, y);
			results1 = aui_geti(x);
			results2 = k - l;
			if (results1 != results2) {
				printf("aui_sub(%u, %u) [short short]: %u != %u\n", k, l, results1, results2);
				getchar();
			}
			/* mul */
			aui_seti(x, k);
			aui_mul(x, y);
			results1 = aui_geti(x);
			results2 = k * l;
			if (results1 != results2) {
				printf("aui_mul(%u, %u) [short short]: %u != %u\n", k, l, results1, results2);
				getchar();
			}
			if (l) {
				/* div */
				aui_seti(x, k);
				aui_div(x, y);
				results1 = aui_geti(x);
				results2 = k / l;
				if (results1 != results2) {
					printf("aui_div(%u, %u) [short short]: %u != %u\n", k, l, results1, results2);
					getchar();
				}
				/* mod */
				aui_seti(x, k);
				aui_mod(x, y);
				results1 = aui_geti(x);
				results2 = k % l;
				if (results1 != results2) {
					printf("aui_mod(%u, %u) [short short]: %u != %u\n", k, l, results1, results2);
					getchar();
				}
			}
		} while (l--);
		if (!(k % 100))
			printf("add_sub_mul_div_mod [short short]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	/* char short */
	aui_init();
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char short] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(l))))) {
		printf("aui_pull(...) [char short] == NULL\n");
		return;
	}
	i = UCHAR_MAX;
	do {
		l = USHRT_MAX;
		do {
			aui_seti(y, l);
			/* add */
			aui_seti(x, i);
			aui_add(x, y);
			resultc1 = aui_geti(x);
			resultc2 = i + l;
			if (resultc1 != resultc2) {
				printf("aui_add(%u, %u) [char short]: %u != %u\n", i, l, resultc1, resultc2);
				getchar();
			}
			/* sub */
			aui_seti(x, i);
			aui_sub(x, y);
			resultc1 = aui_geti(x);
			resultc2 = i - l;
			if (resultc1 != resultc2) {
				printf("aui_sub(%u, %u) [char short]: %u != %u\n", i, l, resultc1, resultc2);
				getchar();
			}
			/* mul */
			aui_seti(x, i);
			aui_mul(x, y);
			resultc1 = aui_geti(x);
			resultc2 = i * l;
			if (resultc1 != resultc2) {
				printf("aui_mul(%u, %u) [char short]: %u != %u\n", i, l, resultc1, resultc2);
				getchar();
			}
			if (l) {
				/* div */
				aui_seti(x, i);
				aui_div(x, y);
				resultc1 = aui_geti(x);
				resultc2 = i / l;
				if (resultc1 != resultc2) {
					printf("aui_div(%u, %u) [char short]: %u != %u\n", i, l, resultc1, resultc2);
					getchar();
				}
				/* mod */
				aui_seti(x, i);
				aui_mod(x, y);
				resultc1 = aui_geti(x);
				resultc2 = i % l;
				if (resultc1 != resultc2) {
					printf("aui_mod(%u, %u) [char short]: %u != %u\n", i, l, resultc1, resultc2);
					getchar();
				}
			}
		} while (l--);
		if (!(i % 10))
			printf("add_sub_mul_div_mod [char short]: %u\n", i);
	} while (i--);
	aui_push(x);
	aui_push(y);
	/* short char */
	aui_init();
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(k))))) {
		printf("aui_pull(...) [short char] == NULL\n");
		return;
	}
	if (!(y = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [short char] == NULL\n");
		return;
	}
	k = USHRT_MAX;
	do {
		j = UCHAR_MAX;
		do {
			aui_seti(y, j);
			/* add */
			aui_seti(x, k);
			aui_add(x, y);
			results1 = aui_geti(x);
			results2 = k + j;
			if (results1 != results2) {
				printf("aui_add(%u, %u) [short char]: %u != %u\n", k, j, results1, results2);
				getchar();
			}
			/* sub */
			aui_seti(x, k);
			aui_sub(x, y);
			results1 = aui_geti(x);
			results2 = k - j;
			if (results1 != results2) {
				printf("aui_sub(%u, %u) [short char]: %u != %u\n", k, j, results1, results2);
				getchar();
			}
			/* mul */
			aui_seti(x, k);
			aui_mul(x, y);
			results1 = aui_geti(x);
			results2 = k * j;
			if (results1 != results2) {
				printf("aui_mul(%u, %u) [short char]: %u != %u\n", k, j, results1, results2);
				getchar();
			}
			if (j) {
				/* div */
				aui_seti(x, k);
				aui_div(x, y);
				results1 = aui_geti(x);
				results2 = k / j;
				if (results1 != results2) {
					printf("aui_div(%u, %u) [short char]: %u != %u\n", k, j, results1, results2);
					getchar();
				}
				/* mod */
				aui_seti(x, k);
				aui_mod(x, y);
				results1 = aui_geti(x);
				results2 = k % j;
				if (results1 != results2) {
					printf("aui_mod(%u, %u) [short char]: %u != %u\n", k, j, results1, results2);
					getchar();
				}
			}
		} while (j--);
		if (!(k % 100))
			printf("add_sub_mul_div_mod [short char]: %u\n", k);
	} while (k--);
	aui_push(x);
	aui_push(y);
	aui_wipe();
	printf("add_sub_mul_div_mod: finish\n");
	return;
}

void sets_gets(void)
{
	#define BASE85 85
	#define STRMAX 100
	struct aui *x;
	unsigned char i;
	unsigned short j;
	unsigned long result;
	char string[STRMAX];
	char set[BASE85] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
		'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
		'y', 'z', '!', '#', '$', '%', '&', '(', ')', '*',
		'+', '-', ';', '<', '=', '>', '?', '@', '^', '_',
		'`','{', '|', '}', '~'
	};

	printf("sets_gets: start\n");
	aui_init();
	/* char */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(i))))) {
		printf("aui_pull(...) [char] == NULL\n");
		return;
	}
	i = UCHAR_MAX;
	do {
		aui_seti(x, i);
		j = BASE85;
		do {
			if (!aui_gets(string, STRMAX, set, j, x)) {
				printf("aui_gets(...) [char] == FALSE\n");
				getchar();
				continue;
			}
			if (!(aui_sets(x, string, set, j))) {
				printf("aui_sets(...) [char] == FALSE\n");
				getchar();
				continue;
			}
			result = aui_geti(x);
			if (result != i) {
				printf("aui_gets(...) or aui_sets(...) [char]: %u != %u\n", result, i);
				getchar();
			}
		} while (--j > 2);
		if (!(i % 10))
			printf("sets_gets [char]: %u\n", i);
	} while (i--);
	aui_push(x);
	/* short */
	if (!(x = aui_pull(AUI_SIZTOLEN(sizeof(j))))) {
		printf("aui_pull(...) [short] == NULL\n");
		return;
	}
	j = USHRT_MAX;
	do {
		aui_seti(x, j);
		i = BASE85;
		do {
			if (!aui_gets(string, STRMAX, set, i, x)) {
				printf("aui_gets(...) [short] == FALSE\n");
				getchar();
				continue;
			}
			if (!(aui_sets(x, string, set, i))) {
				printf("aui_sets(...) [short] == FALSE\n");
				getchar();
				continue;
			}
			result = aui_geti(x);
			if (result != j) {
				printf("aui_gets(...) or aui_sets(...) [short]: %u != %u\n", result, j);
				getchar();
			}
		} while (--i > 2);
		if (!(j % 100))
			printf("sets_gets [short]: %u\n", j);
	} while (j--);
	aui_push(x);
	aui_wipe();
	printf("sets_gets: finish\n");
	return;
}

int main(void)
{
	/*init_msbn_lsbn();
	alloc_free();
	pull_push_wipe();
	seti_geti();
	asgn_swap();
	eq_neq_lt_lte_gt_gte_eval();
	msba_lsba();
	one_two_and_ior_xor();
	shl_shr_inc_dec();
	add_sub_mul_div_mod();*/
	sets_gets();
	return 0;
}
