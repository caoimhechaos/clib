#include "clib_internal.h"
#include "testsuite.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int
main(void)
{
	int ntest = 0, exitval = 0, serverfd, clientfd;

	struct addrinfo *addr;
	test_expect_int(int, c_str2addrinfo("localhost:33994", &addr), 0);
	testresult("addr != NULL", (addr != NULL));

	fputs("Testing AF_INET...\n", stderr);

	testresult("socket() returns an fd >= 0", (serverfd = socket(AF_INET, SOCK_STREAM, 0)) >= 0);

	test_expect_int(int, c_bind2addrinfo(serverfd, addr), 0);

	test_expect_int(int, listen(serverfd, 5), 0);

	testresult("socket() returns an fd >= 0", (clientfd = socket(AF_INET, SOCK_STREAM, 0)) >= 0);

	testresult("addr != NULL", (addr != NULL));
	test_expect_int(int, c_connect2addrinfo(clientfd, addr), 0);

	fputs("Testing AF_INET6...\n", stderr);

	testresult("socket() returns an fd >= 0", (serverfd = socket(AF_INET6, SOCK_STREAM, 0)) >= 0);

	test_expect_int(int, c_bind2addrinfo(serverfd, addr), 0);

	test_expect_int(int, listen(serverfd, 5), 0);

	testresult("socket() returns an fd >= 0", (clientfd = socket(AF_INET6, SOCK_STREAM, 0)) >= 0);

	testresult("addr != NULL", (addr != NULL));
	test_expect_int(int, c_connect2addrinfo(clientfd, addr), 0);


	if (addr)
		free(addr);

	exit(exitval ? EXIT_FAILURE : EXIT_SUCCESS);
}
