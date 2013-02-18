noinst_LTLIBRARIES=	libempty.la
libempty_la_SOURCES=	empty.c

TESTS=			hash_basics array_basics net_basics
TESTS+=			net_bindconnect

EXTRA_DIST=		${TESTS}

AM_CPPFLAGS=		-I${top_srcdir}
#LDFLAGS+=		${top_builddir}/libclib.la

CLEANFILES=		${TESTS} ${TESTS:=.o}

hash_basics:	hash_basics.o
	${LINK} hash_basics.o ${top_builddir}/libclib.la

array_basics:	array_basics.o
	${LINK} array_basics.o ${top_builddir}/libclib.la

rbtree_basics:	rbtree_basics.o
	${LINK} rbtree_basics.o ${top_builddir}/libclib.la

net_basics:	net_basics.o
	${LINK} net_basics.o ${top_builddir}/libclib.la

net_bindconnect: net_bindconnect.o
	${LINK} net_bindconnect.o ${top_builddir}/libclib.la
