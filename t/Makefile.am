noinst_LTLIBRARIES=	libempty.la
libempty_la_SOURCES=	empty.c

TESTS=			hash_basics
EXTRA_DIST=		${TESTS}

CPPFLAGS+=		${DEFS} ${DEFAULT_INCLUDES} ${INCLUDES} ${AM_CPPFLAGS} ${AM_CFLAGS} ${CFLAGS} -I${top_srcdir}
#LDFLAGS+=		${top_srcdir}/libclib.la

CLEANFILES=		${TESTS} ${TESTS:=.o}

hash_basics:	hash_basics.o
	${LINK} hash_basics.o ${top_srcdir}/libclib.la
