#ifndef HAVE_CLIB_HASH_H
#define HAVE_CLIB_HASH_H 1

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
 * Function prototypes for data types
 */
/* Create or destroy a new hash table. */
extern struct c_hashtable *c_hashtable_new(c_hashfunc hash,
	c_equalfunc equals);
extern struct c_hashtable *c_hashtable_new_complex(c_hashfunc hash,
	c_equalfunc equals, c_destructor key_destructor,
	c_destructor value_destructor, uint8_t nbits);
extern void c_hashtable_destroy(struct c_hashtable *h);

/* Add/remove hash table entries. */
extern int c_hashtable_insert(struct c_hashtable *h, const void *key,
	const void *value);
extern int c_hashtable_replace(struct c_hashtable *h, const void *key,
	const void *value);

/* Lookup functions. */
extern void *c_hashtable_lookup(struct c_hashtable *h, const void *key);
extern int c_hashtable_lookup_cb(struct c_hashtable *h, const void *key,
	c_htcallback cb, const void *userdata);
extern int c_hashtable_foreach(struct c_hashtable *h, c_htcallback cb,
	const void *userdata);

/* Remove functions. */
extern int c_hashtable_remove(struct c_hashtable *h, const void *key);
extern int c_hashtable_remove_all(struct c_hashtable *h);

#endif /* HAVE_CLIB_HASH_H */
