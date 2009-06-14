#!/bin/sh
aclocal
autoheader
autoconf
libtoolize -c
automake --foreign --add-missing --copy
./configure "$@"
