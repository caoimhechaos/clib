SUBDIRS=	array hash net rbtree t include/clib
lib_LTLIBRARIES=	libclib.la
libclib_la_SOURCES=
libclib_la_LIBADD=	${top_builddir}/hash/libhash.la		\
	${top_builddir}/array/libarray.la			\
	${top_builddir}/net/libnet.la				\
	${top_builddir}/rbtree/librbtree.la
libclib_la_LDFLAGS=	-version-info 0:2:0
