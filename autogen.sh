#!/bin/sh

# Author: Morten Eriksen, <mortene@sim.no>. Loosely based on Ralph
# Levien's script for Gnome.

DIE=0

PROJECT=simage

# FIXME: check for minimum version number? 19990822 mortene.
AUTOCONF_VER=2.13
if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
    echo
    echo "You must have autoconf version $AUTOCONF_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
    echo "Get ftp://ftp.gnu.org/pub/gnu/autoconf-*.tar.gz"
    DIE=1
fi

LIBTOOL_VER=1.3.4
if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
    echo
    echo "You must have libtool version $LIBTOOL_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz"
    DIE=1
fi

# FIXME: check for minimum version number? 19990822 mortene.
(automake --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have automake installed to generate"
	echo "configure information and Makefiles for $PROJECT."
        echo "Get ftp://ftp.gnu.org/pub/gnu/automake-*.tar.gz"
        DIE=1
}

# FIXME: check for more tools? 19990822 mortene.

if test "$DIE" -eq 1; then
        exit 1
fi

echo "Running aclocal (generating aclocal.m4)..."
# /usr/local/share/aclocal is default install directory for libtool.m4
# from the ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz distribution.
# mortene.
aclocal
# -I /usr/local/share/aclocal

echo "Running autoheader (generating config.h.in)..."
autoheader

echo "Running automake (generating the Makefile.in files)..."
#automake --include-deps
automake --add-missing

echo "Running autoconf (generating ./configure and the Makefile files)..."
autoconf

echo
echo "Now type './configure' and 'make' to compile $PROJECT."
