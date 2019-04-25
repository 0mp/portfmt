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
 *    notice, this list of conditions and the following disclaimer.
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

#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "target.h"

struct Target {
	char *name;
	char *deps;
};

struct Target *
target_new(char *buf) {
	struct Target *target = xmalloc(sizeof(struct Target));

	char *after_target = memchr(buf, ':', strlen(buf));
	if (after_target == NULL || after_target < buf) {
		errx(1, "invalid target: %s", buf);
	}

	target->name = xmalloc(strlen(buf));
	strncpy(target->name, buf, after_target - buf);
	target->name = str_trim(target->name);

	target->deps = xstrdup(after_target + 1);

	return target;
}

void
target_free(struct Target *target)
{
	free(target->name);
	free(target->deps);
	free(target);
}

char *
target_name(struct Target *target)
{
	assert(target != NULL);
	return target->name;
}

