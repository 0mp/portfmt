/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2019 Tobias Kortkamp <tobik@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "config.h"

#include <sys/types.h>
#include <assert.h>
#include <err.h>
#include <stdlib.h>

#include "array.h"

struct Array {
	void **buf;
	size_t cap;
	size_t len;
	size_t value_size;
};

struct Array *
array_new(size_t value_size)
{
	struct Array *array = malloc(sizeof(struct Array));
	if (array == NULL) {
		err(1, "malloc");
	}

	array->cap = 1024;
	array->len = 0;
	array->value_size = value_size;

	array->buf = reallocarray(NULL, array->cap, value_size);
	if (array->buf == NULL) {
		err(1, "malloc");
	}

	return array;
}

void
array_append(struct Array *array, void *v)
{
	assert(array->cap > 0);
	assert(array->cap > array->len);

	array->buf[array->len++] = v;
	if (array->len >= array->cap) {
		size_t new_cap = array->cap * 2;
		assert(new_cap > array->cap);
		void **new_array = reallocarray(array->buf, new_cap, array->value_size);
		if (new_array == NULL) {
			err(1, "reallocarray");
		}
		array->buf = new_array;
		array->cap = new_cap;
	}
}

void *
array_get(struct Array *array, size_t i)
{
	if (i < array->len) {
		return array->buf[i];
	}
	return NULL;
}

size_t
array_len(struct Array *array)
{
	return array->len;
}

void
array_sort(struct Array *array, int (*compar)(const void *, const void *))
{
	qsort(array->buf, array->len, array->value_size, compar);
}

void
array_truncate(struct Array *array)
{
	array->len = 0;
}