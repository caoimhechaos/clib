noinst_LTLIBRARIES=	libempty.la
libempty_la_SOURCES=	empty.c

TESTS=			hash_basics
EXTRA_DIST=		${TESTS}

AM_CPPFLAGS=		-I${top_srcdir}
#LDFLAGS+=		${top_srcdir}/libclib.la

CLEANFILES=		${TESTS} ${TESTS:=.o}

hash_basics:	hash_basics.o
	${LINK} hash_basics.o ${top_srcdir}/libclib.la
