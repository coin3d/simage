#!/bin/sh

# Author: Morten Eriksen, <mortene@sim.no>. Loosely based on Ralph
# Levien's script for Gnome.

DIE=0

PROJECT=simage

# FIXME: check for minimum version number? 19990822 mortene.
(autoconf --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have autoconf installed to generate"
	echo "configure information and Makefiles for $PROJECT."
        echo "Get ftp://ftp.gnu.org/pub/gnu/autoconf-*.tar.gz"
        DIE=1
}

# FIXME: check for minimum version number? 19990822 mortene.
(libtool --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have libtool installed to generate"
	echo "configure information and Makefiles for $PROJECT."
        echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz"
        DIE=1
}

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

echo "Running aclocal..."
# /usr/local/share/aclocal is default install directory for libtool.m4
# from the ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz distribution.
# mortene.
aclocal -I /usr/local/share/aclocal

echo
echo "Running autoheader..."
autoheader

echo
echo "Running automake..."
echo "(NB: if you're compiling with gcc, you might wish to"
echo "run automake by hand without the --include-deps argument"
echo "to get dependency tracking. 19990908 mortene.)"
automake --include-deps

echo
echo "Running autoconf..."
autoconf

echo
echo "Now type './configure' and 'make' to compile $PROJECT."
