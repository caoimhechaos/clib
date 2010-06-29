#ifndef HAVE_CLIB_ARRAY_H
#define HAVE_CLIB_ARRAY_H

/**
 * Data structures
 */
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

#endif
