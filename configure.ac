#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([clib], [0.1.99], [tonnerre@NetBSD.org])
AC_CONFIG_SRCDIR([clib.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([string.h strings.h memory.h stdlib.h unistd.h queue.h sys/types.h sys/queue.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([hash/Makefile t/Makefile Makefile])
AC_OUTPUT
