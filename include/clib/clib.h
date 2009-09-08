#ifndef HAVE_CLIB_H
#define HAVE_CLIB_H 1

/**
 * Function types
 */
typedef uint32_t	(*c_hashfunc)(const void *input);
typedef int		(*c_equalfunc)(const void *a, const void *b);
typedef void		(*c_destructor)(void *what);
typedef int		(*c_htcallback)(const void *entry, const void *userdata);
typedef int		(*c_arcallback)(const void *entry, const void *userdata);
typedef	ssize_t		(*c_resizestrategy)(ssize_t old, ssize_t new);

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

struct c_array
{
	/**
	 * length:	The number of entries currently in the array.
	 * size:	The number of entries currently allocated.
	 */
	ssize_t		a_len, a_size;
	c_resizestrategy resizer;
	c_destructor	destructor;
	const void **   a_values;
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

/* Create a new array */
extern struct c_array *c_array_new(c_resizestrategy strategy);
extern struct c_array *c_array_new_complex(c_resizestrategy strategy,
			c_destructor value_destructor);
extern void c_array_destroy(struct c_array *a);

/* Add new entries to an array */
extern int c_array_insert(struct c_array *a, int key, const void *value);
extern int c_array_replace(struct c_array *a, int key, const void *value);
extern int c_array_push(struct c_array *a, const void *value);
extern int c_array_unshift(struct c_array *a, const void *value);

/* Lookup entries from an array */
extern void *c_array_get(struct c_array *a, int key);
extern int c_array_foreach(struct c_array *a, c_arcallback cb, const void *userdata);

/* Remove entries from an array */
extern int c_array_remove(struct c_array *a, int key);
extern void *c_array_pop(struct c_array *a);
extern void *c_array_shift(struct c_array *a);

/* Some default functions for hashes et cetera. */
extern int c_stringequals(const void *a, const void *b);
extern uint32_t c_stringhash(const void *input);
extern uint32_t c_dummyhash(const void *input);

/* Sizing strategies */
extern ssize_t c_resize_minimal(ssize_t old, ssize_t new);
extern ssize_t c_resize_linear(ssize_t old, ssize_t new);
extern ssize_t c_resize_quadratic(ssize_t old, ssize_t new);

/* Default destructors */
extern void c_dummydestructor(void *addr);

/**
 * Network function prototypes.
 */
extern int c_str2sockaddr(char *str, struct sockaddr_storage **res);
extern char *c_sockaddr2str(struct sockaddr_storage *);
extern int c_bind2addrinfo(int sockfd, struct addrinfo *addr);
extern int c_connect2addrinfo(int sockfd, struct addrinfo *addr);

#endif /* HAVE_CLIB_H */
