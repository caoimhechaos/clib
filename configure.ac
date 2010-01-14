#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([clib], [0.2.2], [caoimhechaos@protonmail.com])
AC_CONFIG_SRCDIR([clib_internal.h])
AC_CONFIG_HEADERS([config.h include/clib/queue.h])
AM_INIT_AUTOMAKE

AC_DEFUN([CL_IFDEF], [
AC_MSG_CHECKING([for definition of $1])
AC_LINK_IFELSE([
AC_LANG_PROGRAM([
#ifdef HAVE_QUEUE_H
#include <queue.h>
#endif
#ifdef HAVE_SYS_QUEUE_H
#include <sys/queue.h>
#endif
#ifdef HAVE_NBCOMPAT_QUEUE_H
#include <nbcompat/queue.h>
#endif
#ifndef $1
choke me
#endif
])
], [AC_MSG_RESULT(yes)], [AC_DEFINE($2, $3, $4)
AC_MSG_RESULT(no)])
])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([nbcompat], [main], [LIBS="-lnbcompat $LIBS"])

# Checks for header files.
AC_CHECK_HEADERS([string.h strings.h memory.h stdlib.h unistd.h queue.h	\
	sys/types.h sys/queue.h nbcompat/queue.h assert.h stddef.h	\
	sys/socket.h netdb.h])
CL_IFDEF(SLIST_HEAD, [SLIST_HEAD(name,type)],
	[struct name { struct type *slh_first; /* first element */ }],
	[Singly-linked list head])
CL_IFDEF(SLIST_INIT, [SLIST_INIT(head)], [(head)->slh_first = NULL;],
	[Initialize the singly linked list header])
CL_IFDEF(SLIST_INSERT_AFTER, [SLIST_INSERT_AFTER(slistelm, elm, field)],
	[do { (elm)->field.sle_next = (slistelm)->field.sle_next; (slistelm)->field.sle_next = (elm); } while (/*CONSTCOND*/0)],
	[Insert element after current element])
CL_IFDEF(SLIST_INSERT_HEAD, [SLIST_INSERT_HEAD(head, elm, field)],
	[do { (elm)->field.sle_next = (head)->slh_first; (head)->slh_first = (elm); } while (/*CONSTCOND*/0)],
	[Insert element to the list head])
CL_IFDEF(SLIST_REMOVE_HEAD, [SLIST_REMOVE_HEAD(head, field)],
	[do { (head)->slh_first = (head)->slh_first->field.sle_next; } while (/*CONSTCOND*/0)],
	[Remove the head entry of a singly linked list])
CL_IFDEF(SLIST_REMOVE, [SLIST_REMOVE(head, elm, type, field)],
	[do { if ((head)->slh_first == (elm)) { SLIST_REMOVE_HEAD((head), field); } else { struct type *curelm = (head)->slh_first; while(curelm->field.sle_next != (elm)) curelm = curelm->field.sle_next; curelm->field.sle_next = curelm->field.sle_next->field.sle_next; } } while (/*CONSTCOND*/0)],
	[Remove any entry of a singly linked list])
CL_IFDEF(SLIST_REMOVE_AFTER, [SLIST_REMOVE_AFTER(slistelm, field)],
	[do { (slistelm)->field.sle_next = SLIST_NEXT(SLIST_NEXT((slistelm), field), field); } while (/*CONSTCOND*/0)],
	[Remove slist element after the current one])
CL_IFDEF(SLIST_FOREACH, [SLIST_FOREACH(var, head, field)],
	[for((var) = (head)->slh_first; (var); (var) = (var)->field.sle_next)],
	[Iterate over singly linked lists])

CL_IFDEF(SLIST_EMPTY, [SLIST_EMPTY(head)], [((head)->slh_first == NULL)],
	[Return true if singly-linked list head is empty, false otherwise])
CL_IFDEF(SLIST_FIRST, [SLIST_FIRST(head)], [((head)->slh_first)],
	[Returns the first element of the singly-linked list])
CL_IFDEF(SLIST_NEXT, [SLIST_NEXT(elm, field)], [((elm)->field.sle_next)],
	[Returns the next element of the singly-linked list])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_TYPES([struct sockaddr_storage], [],
 [AC_DEFINE_UNQUOTED([sockaddr_storage], [sockaddr],
  [Socket address structure])], [[#include <sys/socket.h>]])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset memmove strdup getaddrinfo freeaddrinfo	\
	gai_strerror strchr inet_ntop])

INCLUDES="-I\${top_srcdir} -I\${top_srcdir}/include"
AC_SUBST(INCLUDES)

AC_CONFIG_FILES([include/clib/Makefile array/Makefile hash/Makefile
	net/Makefile t/Makefile Makefile])
AC_OUTPUT
