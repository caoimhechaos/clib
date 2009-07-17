SUBDIRS=	array hash t include/clib
lib_LTLIBRARIES=	libclib.la
libclib_la_SOURCES=
libclib_la_LIBADD=	hash/libhash.la array/libarray.la
libclib_la_LDFLAGS=	-version-info 0:1:0
libclib_la_includedir=		${includedir}/clib
libclib_la_include_HEADERS=	clib.h
