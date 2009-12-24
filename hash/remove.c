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
 * Retrieve the first record with the specified key. Otherwise, return NULL.
 */
int
c_hashtable_remove(struct c_hashtable *h, const void *key)
{
	uint32_t hvalue, hvalue_lookup;
	int ndeleted = 0;
	struct c_hashtable_value *val, *oldval = NULL;

	if (!h)
		return 0;

	hvalue = h->h_hash(key);
	hvalue_lookup = hvalue % h->h_size;

	SLIST_FOREACH(val, &h->m_values[hvalue_lookup], v_vlist)
	{
		if (!h->h_equals(val->key, key))
		{
			oldval = val;
			continue;
		}

		if (oldval)
			SLIST_REMOVE_HEAD(&h->m_values[hvalue_lookup], v_vlist);
		else
			SLIST_REMOVE_AFTER(oldval, v_vlist);

		if (h->h_keydestr)
			h->h_keydestr(val->key);
		if (h->h_valdestr)
			h->h_valdestr(val->value);
	}

	return ndeleted;
}

/**
 * Remove all entries in the hash table.
 */
int
c_hashtable_remove_all(struct c_hashtable *h)
{
	uint32_t i;
	struct c_hashtable_value *val;

	if (!h)
		return 0;

	for (i = 0; i < h->h_size; i++)
	{
		SLIST_FOREACH(val, &h->m_values[i], v_vlist)
		{
			SLIST_REMOVE_HEAD(&h->m_values[i], v_vlist);

			if (h->h_keydestr)
				h->h_keydestr(val->key);
			if (h->h_valdestr)
				h->h_valdestr(val->value);
		}
	}

	return 1;
}
