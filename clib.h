#ifndef HAVE_CLIB_H
#define HAVE_CLIB_H 1

/**
 * Function types
 */
typedef uint32_t	(*c_hashfunc)(const void *input);
typedef int		(*c_equalfunc)(const void *a, const void *b);
typedef void		(*c_destructor)(void *what);

/**
 * Data structures
 */
struct c_hashtable_value
{
	SLIST_ENTRY(c_hashtable_value) v_vlist;
	void *		key;
	void *		value;
};

struct c_hashtable
{
	uint8_t		h_outbits;
	uint32_t	h_size;
	c_hashfunc	h_hash;
	c_equalfunc	h_equals;

	c_destructor	h_keydestr;
	c_destructor	h_valdestr;

	SLIST_HEAD(, c_hashtable_value) * m_values;
};

/**
 * Function prototypes
 */

/* Create a new hash table. */
struct c_hashtable *c_hashtable_new(c_hashfunc hash, c_equalfunc equals);
struct c_hashtable *c_hashtable_new_complex(c_hashfunc hash, c_equalfunc equals,
	c_destructor key_destructor, c_destructor value_destructor,
	uint8_t nbits);

/* Add/remove hash table entries. */
int c_hashtable_insert(struct c_hashtable *h, const void *key, const void *value);
int c_hashtable_replace(struct c_hashtable *h, const void *key, const void *value);

#endif /* HAVE_CLIB_H */
