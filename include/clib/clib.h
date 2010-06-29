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

#include <clib/hash.h>
#include <clib/array.h>

/**
 * Function prototypes
 */
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
extern int c_str2addrinfo(char *str, struct addrinfo **addr);
extern char *c_sockaddr2str(struct sockaddr_storage *);
extern int c_bind2addrinfo(int sockfd, struct addrinfo *addr);
extern int c_connect2addrinfo(int sockfd, struct addrinfo *addr);

#endif /* HAVE_CLIB_H */
