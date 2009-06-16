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
 * Insert a new record into the hash table. If an entry with the same
 * key already exists, the currently specified entry is prepended before
 * it.
 */
int
c_hashtable_insert(struct c_hashtable *h, const void *key, const void *value)
{
	uint32_t hvalue, hvalue_lookup;
	struct c_hashtable_value *newval;

	if (!h)
		return 0;

	hvalue = h->h_hash(key);
	hvalue_lookup = hvalue % h->h_size;

	newval = malloc(sizeof(struct c_hashtable_value));
	if (!newval)
		return 0;

	SLIST_INSERT_HEAD(&h->m_values[hvalue_lookup], newval, v_vlist);
	newval->key = (void *)key;
	newval->value = (void *)value;

	return 1;
}

/**
 * Replace the first hash table entry currently associated with the current
 * key with the specified entry. If no key is found it is simply added.
 */
int
c_hashtable_replace(struct c_hashtable *h, const void *key, const void *value)
{
	uint32_t hvalue, hvalue_lookup;
	struct c_hashtable_value *newval;

	if (!h)
		return 0;

	hvalue = h->h_hash(key);
	hvalue_lookup = hvalue % h->h_size;

	SLIST_FOREACH(newval, &h->m_values[hvalue_lookup], v_vlist)
	{
		if (!h->h_equals(newval->key, key))
			continue;

		if (h->h_valdestr)
			h->h_valdestr(newval->value);

		newval->value = (void *)value;
		return 1;
	}

	/* Nothing found, guess we need to add a new entry. */
	newval = malloc(sizeof(struct c_hashtable_value));
	if (!newval)
		return 0;

	SLIST_INSERT_HEAD(&h->m_values[hvalue_lookup], newval, v_vlist);
	newval->key = (void *)key;
	newval->value = (void *)value;

	return 1;
}
