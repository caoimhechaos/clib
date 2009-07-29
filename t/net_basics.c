#include "clib_internal.h"
#include "testsuite.h"

#include <stdio.h>

int
main(void)
{
	struct sockaddr_storage *sa;
	int ntest = 0, exitval = 0, special;
	void *ptr;
	uint32_t ip;

	test_expect_int(int, (special = c_str2sockaddr("127.0.0.1:53", &sa)), 0);\
	if (special)
		fprintf(stderr, "Error was: %s\n", gai_strerror(special));
	test_expect_int(int, sa->ss_family, AF_INET);
	test_expect_str("reserialized IP address", (ptr = c_sockaddr2str(sa)), "127.0.0.1");
	if (ptr)
		free(ptr);
	free(sa);

	test_expect_int(int, (special = c_str2sockaddr("[::1]:53", &sa)), 0);\
	if (special)
		fprintf(stderr, "Error was: %s\n", gai_strerror(special));
	test_expect_int(int, sa->ss_family, AF_INET6);
	test_expect_str("reserialized IP address", (ptr = c_sockaddr2str(sa)), "::1");
	if (ptr)
		free(ptr);
	free(sa);

	exit(exitval ? EXIT_FAILURE : EXIT_SUCCESS);
}
