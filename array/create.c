/*-
 * Copyright (c) 2009 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Tonnerre Lombard <tonnerre@NetBSD.org>.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <unistd.h>

#include "clib_internal.h"

/**
 * Create a dynamically resizing array the short way (no destructors).
 */
struct c_array *
c_array_new(c_resizestrategy strategy)
{
	return c_array_new_complex(strategy, c_dummydestructor);
}

/**
 * Create a dynamically resizing array.
 */
struct c_array *
c_array_new_complex(c_resizestrategy strategy, c_destructor destructor)
{
	struct c_array *a = malloc(sizeof(struct c_array));

	if (!a) return NULL;

	a->a_len = 0;
	a->resizer = strategy;
	a->destructor = destructor;

	a->a_size = strategy(0, 0);
	if (a->a_size <= 0) a->a_size = 1;

	a->a_values = malloc(a->a_size * sizeof(void *));
	if (!a->a_values)
	{
		free(a);
		return NULL;
	}

	memset(a->a_values, 0, (a->a_size ? a->a_size : 1) * sizeof(void *));

	return a;
}

/**
 * Destroy the array again.
 */
void
c_array_destroy(struct c_array *a)
{
	unsigned int i;

	for (i = 0; i < a->a_len; i++)
		a->destructor(a->a_values[i]);

	free(a->a_values);
	free(a);
}
