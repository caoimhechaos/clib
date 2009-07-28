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
 * Insert a new record into the array. If an entry with the specified ID
 * exists already, the entries starting from that entry will be moved back
 * by one.
 */
int
c_array_insert(struct c_array *a, int key, const void *value)
{
	ssize_t newlen;
	if (!a)
		return 0;

	newlen = (key > a->a_len ? key : a->a_len) + 1;

	if (newlen > a->a_size)
	{
		ssize_t newsz = a->resizer(a->a_size, newlen);
		void *newptr;

		if (newsz <= 0) newsz = 1;

		if ((newptr = realloc(a->a_values, newsz * sizeof(void *))) == NULL)
			return 0;

		a->a_values = newptr;
		a->a_size = newsz;

		assert(a->a_size >= a->a_len);

		memset(a->a_values + a->a_len, 0,
			 (a->a_size - a->a_len) * sizeof(void *));
	}

	if (key < a->a_len)
	{
		memmove(a->a_values + (key + 1), a->a_values + key,
			(a->a_len - key) * sizeof(void *));
		a->a_len++;
	}
	else if (key > a->a_len)
	{
		memset(a->a_values + a->a_len, 0,
			(key - a->a_len) * sizeof(void *));
		a->a_len = key + 1;
	}
	else
		a->a_len++;

	a->a_values[key] = value;

	return 1;
}

/**
 * Replace the first hash table entry currently associated with the current
 * key with the specified entry. If no key is found it is simply added.
 */
int
c_array_replace(struct c_array *a, int key, const void *value)
{
	if (key >= a->a_len)
		return c_array_insert(a, key, value);

	a->destructor(a->a_values[key]);
	a->a_values[key] = value;

	return 1;
}

/**
 * Append element to the end of the array.
 */
int
c_array_push(struct c_array *a, const void *value)
{
	return c_array_insert(a, a->a_len, value);
}

/**
 * Prepend element to the beginning of the array.
 */
int
c_array_unshift(struct c_array *a, const void *value)
{
	return c_array_insert(a, 0, value);
}
