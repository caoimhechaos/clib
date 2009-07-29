/*-
 * Copyright (c) 2009 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Caoimhe Chaos <caoimhechaos@protonmail.com>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "clib_internal.h"
#include <stdio.h>

#ifdef HAVE_GETADDRINFO
/**
 * str2addrinfo -	convert a given string of the type hostname:port to a
 *			struct addrinfo (basically a list of sockaddrs).
 *
 * Returns a getaddrinfo error code, or 0 upon success.
 */
int
c_str2addrinfo(char *str, struct addrinfo **addr)
{
	int err;
	char *hostport = strdup(str), *horig = hostport, *portptr;

	if (!hostport)
		return EAI_MEMORY;

	if (*hostport == '[')
	{
		hostport++;
		portptr = strchr(hostport, ']');

		if (!portptr)
		{
			free(horig);
			return EAI_NONAME;
		}
		*portptr = '\0';

		if (*++portptr == ':')
			portptr++;
	}
	else
	{
		portptr = strchr(hostport, ':');

		if (!portptr)
		{
			free(horig);
			return EAI_NONAME;
		}

		*portptr++ = '\0';
	}

	if ((err = getaddrinfo(hostport, portptr, NULL, addr)))
	{
		free(horig);
		return err;
	}
	free(horig);

	return 0;
}

/**
 * str2sockaddr -	convert a given string of the type hostname:port to a
 *			struct sockaddr.
 *
 * Returns a getaddrinfo error code, or 0 upon success.
 *
 * Please note that this function only ever fetches the first offered address
 * entry; there's no way to get any other members without going through
 * str2addrinfo().
 */
int
c_str2sockaddr(char *str, struct sockaddr_storage **res)
{
	struct addrinfo *addr;
	int err;

	err = c_str2addrinfo(str, &addr);
	if (err)
		return err;

	*res = malloc(sizeof(struct sockaddr_storage));
	if (!*res)
		return EAI_MEMORY;

	memset(*res, 0, sizeof(struct sockaddr_storage));

	memcpy(*res, addr->ai_addr, addr->ai_addrlen);

	freeaddrinfo(addr);

	return 0;
}
#endif

#ifdef HAVE_GETNAMEINFO
char *
c_sockaddr2str(struct sockaddr_storage *sa)
{
	char astr[INET6_ADDRSTRLEN];
	char pstr[8];
	int res;

	memset(astr, 0, INET6_ADDRSTRLEN);
	memset(pstr, 0, 8);

	res = getnameinfo(sa, sizeof(struct sockaddr_storage),
			astr, INET6_ADDRSTRLEN, pstr, 32, NI_NUMERICHOST | NI_NUMERICSERV);
	if (res)
		return NULL;

	/* Leave strdup() error handling to the caller. Muahahah */
	return strdup(astr);
}
#endif
