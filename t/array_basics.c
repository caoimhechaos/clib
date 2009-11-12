#include "clib_internal.h"
#include "testsuite.h"

#include <stdio.h>
#include <string.h>

int
main(void)
{
	int ntest = 0, exitval = 0;
	struct c_array *a;
	char *str = NULL;

	/* Create array. */
	testresult("create array",
		(a = c_array_new(c_resize_minimal)));
	test_expect_int(ssize_t, a->a_len, 0);

	/* Insert data. */
	testresult("push array 1",
		c_array_push(a, strdup("charly")));
	test_expect_int(ssize_t, a->a_len, 1);

	testresult("push array 2",
		c_array_push(a, strdup("charlene")));
	test_expect_int(ssize_t, a->a_len, 2);

	/* Fetch data. */
	test_expect_str("get array 1", c_array_get(a, 0), "charly");
	test_expect_str("get array 2", c_array_get(a, 1), "charlene");

	/* Replace it. */
	testresult("replace array existing",
		c_array_replace(a, 0, strdup("fries")));
	test_expect_int(ssize_t, a->a_len, 2);

	/* Fetch it again. */
	test_expect_str("get array 3", c_array_get(a, 0), "fries");
	test_expect_str("get array 4", c_array_get(a, 1), "charlene");

	/* Insert new data with replace. */
	testresult("replace array new",
		c_array_replace(a, 2, strdup("peanuts")));
	test_expect_int(ssize_t, a->a_len, 3);

	/* Fetch that too. */
	test_expect_str("get array 5", c_array_get(a, 2), "peanuts");

	/* Unshift data into the array. */
	testresult("unshift array",
		c_array_unshift(a, strdup("elephants")));
	test_expect_int(ssize_t, a->a_len, 4);

	/* Verify data. */
	test_expect_str("get array 6", c_array_get(a, 0), "elephants");
	test_expect_str("get array 7", c_array_get(a, 1), "fries");
	test_expect_str("get array 8", c_array_get(a, 3), "peanuts");

	/* Insert new data before old data */
	testresult("insert array middle",
		   c_array_insert(a, 1, strdup("bacon")));
	test_expect_int(ssize_t, a->a_len, 5);

	/* Verify data. */
	test_expect_str("get array 9", c_array_get(a, 1), "bacon");
	test_expect_str("get array 10", c_array_get(a, 3), "charlene");
	test_expect_str("get array 11", c_array_get(a, 4), "peanuts");

	/* Pop the last element */
	test_expect_str("pop array 1", (str = c_array_pop(a)), "peanuts");
	if (str) free(str);
	test_expect_str("pop array 2", (str = c_array_pop(a)), "charlene");
	if (str) free(str);

	/* Remove it all. */
	/* FIXME: not implemented
	testresult("clear array", c_array_clear(a), 1);
	*/

	/* Destroy array. */
	c_array_destroy(a);
	testresult("destroy array", 1);

	exit(exitval ? EXIT_FAILURE : EXIT_SUCCESS);
}
