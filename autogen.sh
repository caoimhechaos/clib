#!/bin/sh
aclocal
autoheader
autoconf
automake --foreign --add-missing --copy
libtoolize -c
./configure "$@"
