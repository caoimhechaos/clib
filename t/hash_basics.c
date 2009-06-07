#include "clib_internal.h"
#include "testsuite.h"

#include <stdio.h>

int
main(void)
{
	int ntest = 0, exitval = 0;
	struct c_hashtable *h;

	/* Create hashtable. */
	testresult("create hashtable",
		h = c_hashtable_new(c_stringhash, c_stringequals));

	/* Insert data. */
	testresult("insert hashtable",
		c_hashtable_insert(h, strdup("charly"), strdup("bacon")));

	/* Fetch data. */
	testresult("lookup hashtable 1",
		c_stringequals(c_hashtable_lookup(h, "charly"), "bacon"));

	/* Replace it. */
	testresult("replace hashtable",
		c_hashtable_replace(h, "charly", strdup("fries")));

	/* Fetch it again. */
	testresult("lookup hashtable 2",
		c_stringequals(c_hashtable_lookup(h, "charly"), "fries"));

	/* Remove it all. */
	testresult("clear hashtable", c_hashtable_remove_all(h));

	/* Destroy hashtable. */
	c_hashtable_destroy(h);
	testresult("destroy hashtable", 1);

	exit(exitval ? EXIT_FAILURE : EXIT_SUCCESS);
}
