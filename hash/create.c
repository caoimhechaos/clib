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

	memset(h->m_values, 0, htbl_size);

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
