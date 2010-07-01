#ifndef HAVE_CLIB_RBTREE_H
#define HAVE_CLIB_RBTREE_H 1

struct c_rbtree_entry
{
	struct c_rbtree_entry *rb_left, *rb_right, *rb_parent;
	int		rb_color;
	void *		rb_key;
	void *		rb_value;
};

struct c_rbtree
{
	c_equalfunc	rb_compare;

	c_destructor	rb_keydestr;
	c_destructor	rb_valdestr;

	struct c_rbtree_entry	*head;
};

/**
 * Function prototypes for data types
 */
/* Create a new rbtree */
extern struct c_rbtree *c_rbtree_new(c_equalsfunc compare);
extern struct c_rbtree *c_rbtree_new_complex(c_equalsfunc compare,
	c_destructor keydestr, c_destructor valdestr);
extern void c_rbtree_destroy(struct c_rbtree *t);

/* Add/remove rbtree entries */
extern int c_rbtree_insert(struct c_rbtree *t, const void *key,
			   const void *value);
extern int c_rbtree_replace(struct c_rbtree *t, const void *key,
			    const void *value);

/* Lookup functions */
extern int c_rbtree_lookup(struct c_rbtree *t, const void *key);
extern int c_rbtree_lookup_cb(struct c_rbtree *t, const void *key,
	c_htcallback cb, const void *userdata);
extern int c_rbtree_foreach_dfs(struct c_rbtree *t, c_htcallback cb,
	const void *userdata);
extern int c_rbtree_foreach_bfs(struct c_rbtree *t, c_htcallback cb,
	const void *userdata);

/* Remove functions */
extern int c_rbtree_remove(struct c_rbtree *t, const void *key);
extern int c_rbtree_remove_all(struct c_rbtree *t);

#endif /* HAVE_CLIB_RBTREE_H */
