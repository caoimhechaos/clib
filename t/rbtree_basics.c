#include "clib_internal.h"
#include "testsuite.h"

#include <stdio.h>

int
main(void)
{
	int ntest = 0, exitval = 0;
	struct c_rbtree *r;

	/* Create hashtable. */
	testresult("create rbtree",
		r = c_rbtree_new(c_stringequals));

	/* Insert data. */
	testresult("insert rbtree 1",
		c_rbtree_insert(r, strdup("charly"), strdup("bacon")));
	testresult("insert rbtree 2",
		c_rbtree_insert(r, strdup("charlene"), strdup("apple")));

	/* Fetch data. */
	test_expect_str("lookup rbtree 1",
		c_rbtree_lookup(r, "charly"), "bacon");
	test_expect_str("lookup rbtree 2",
		c_rbtree_lookup(r, "charlene"), "apple");

	/* Replace it. */
	testresult("replace rbtree existing",
		c_rbtree_replace(r, strdup("charly"), strdup("fries")));

	/* Fetch it again. */
	test_expect_str("lookup rbtree 3",
		c_rbtree_lookup(r, "charly"), "fries");
	test_expect_str("lookup rbtree 4",
		c_rbtree_lookup(r, "charlene"), "apple");

	/* Insert new data with replace. */
	testresult("replace rbtree new",
		c_rbtree_replace(r, strdup("elephant"), strdup("peanuts")));

	/* Fetch that too. */
	test_expect_str("lookup rbtree 5",
		c_rbtree_lookup(r, "elephant"), "peanuts");

	/* Remove it all. - not implemented yet */
	/* testresult("clear rbtree", c_rbtree_remove_all(r)); */

	/* Destroy rbtree. - not implemented yet */
	/* c_rbtree_destroy(r); */
	/* testresult("destroy rbtree", 1); */

	exit(exitval ? EXIT_FAILURE : EXIT_SUCCESS);
}
