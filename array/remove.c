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

#include "clib_internal.h"

/**
 * Simply remove an entry from an array.
 */
static int
_c_array_remove(struct c_array *a, int key, int dodestroy)
{
	ssize_t newsz;
	if (!a || key < 0 || key > a->a_len)
		return 0;

	if (key < a->a_len)
	{
		if (dodestroy)
			a->destructor(a->a_values[key]);

		memmove(a->a_values + key * sizeof(void *),
			a->a_values + (key + 1) * sizeof(void *),
			a->a_len - key - 1);
	}

	newsz = a->resizer(a->a_len, a->a_len - 1);
	if (newsz != a->a_size)
	{
		void *newptr = realloc(a->a_values, newsz);

		/* Not enough memory to shrink...? */
		if (newptr == NULL)
			return 0;
	}

	a->a_len--;

	return 1;
}

int
c_array_remove(struct c_array *a, int key)
{
	return _c_array_remove(a, key, 1);
}

/**
 * Remove the last entry of an array and return it.
 *
 * Please note that this does not destruct the value.
 */
void *
c_array_pop(struct c_array *a)
{
	void *retval = c_array_get(a, a->a_len - 1);

	if (!_c_array_remove(a, a->a_len - 1, 0))
		return NULL;

	return retval;
}

/**
 * Remove the first entry of an array and return it.
 *
 * Please note that this does not destruct the value.
 */
void *
c_array_shift(struct c_array *a)
{
	void *retval = c_array_get(a, 0);

	if (!_c_array_remove(a, 0, 0))
		return NULL;

	return retval;
}
