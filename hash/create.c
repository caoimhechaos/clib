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
 * Create a hash table with a default size (currently 12 bit) and use
 * free() as a destructor. This is generally desirable for things like
 * string -> string hashes.
 */
struct c_hashtable *
c_hashtable_new(c_hashfunc hash, c_equalfunc equals)
{
	/* Create an 8-bit hash table by default; this will use
	   2^12 bytes on 32-bit archs and 2^16 on 64-bit. */
	return c_hashtable_new_complex(hash, equals, free, free, 12);
}

/**
 * Create a hash table.
 */
struct c_hashtable *
c_hashtable_new_complex(c_hashfunc hash, c_equalfunc equals,
	c_destructor key_destructor, c_destructor value_destructor,
	uint8_t nbits)
{
	struct c_hashtable *h = malloc(sizeof(struct c_hashtable));
	uint32_t htbl_size = 1;
	int i, j;

	if (!h) return NULL;

	h->h_hash = hash;
	h->h_equals = equals;
	h->h_keydestr = key_destructor;
	h->h_valdestr = value_destructor;

	h->h_outbits = nbits;

	for (i = 0; i < nbits; i++)
		htbl_size *= 2;

	h->h_size = htbl_size;

	h->m_values = malloc(htbl_size * sizeof(struct c_hashtable_value *));
	if (!h->m_values)
		goto out_free;

	memset(h->m_values, 0, htbl_size * sizeof(struct c_hashtable_value *));

	/*
	for (i = 0; i < htbl_size; i++)
	{
		h->m_values[i] = malloc(sizeof(struct c_hashtable_value));

		if (!h->m_values[i])
			goto out_free_values;

		memset(h->m_values[i], 0, sizeof(struct c_hashtable_value));
	}
	*/

	return h;

out_free_values:
	/*
	for (j = 0; j < i; j++)
		free(h->m_values[j]);
	*/

	free(h->m_values);
out_free:
	free(h);
	return NULL;
}

/**
 * Destroy the hash table again.
 */
void
c_hashtable_destroy(struct c_hashtable *h)
{
	/**
	 * FIXME: Not sure this is enough, should be checked with a memory
	 * leak tester.
	 */
	c_hashtable_remove_all(h);
	free(h);
}
