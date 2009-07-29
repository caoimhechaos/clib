#include "clib_internal.h"
#include "testsuite.h"

#include <stdio.h>

int
main(void)
{
	struct sockaddr_storage *sa;
	int ntest = 0, exitval = 0, special;
	uint32_t ip;

	test_expect_int(int, (special = c_str2sockaddr("127.0.0.1:53", &sa)), 0);\
	if (special)
		fprintf(stderr, "Error was: %s\n", gai_strerror(special));
	test_expect_int(int, sa->ss_family, AF_INET);
	test_expect_int(uint32_t, ((struct sockaddr_in *) sa)->sin_addr.s_addr, 0x7F000001);
	test_expect_str("reserialized IP address", c_sockaddr2str(sa), "127.0.0.1");

	exit(exitval ? EXIT_FAILURE : EXIT_SUCCESS);
}
