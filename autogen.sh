#! /bin/sh
# autogen.sh -- generate configure and build files for simage source hierarchy
# 
# Authors:
#   Morten Eriksen, <mortene@sim.no>.
#   Lars J. Aas, <larsa@sim.no>.
#

DIE=false

# Autoconf snapshot from ftp://alpha.gnu.org/pub/gnu/autoconf/autoconf-2.49a.tar.gz
AUTOCONF_VER=2.49a
AUTOMAKE_VER=1.4a   # Automake from CVS
LIBTOOL_VER=1.3.5

PROJECT=simage
MACRODIR=conf-macros
AUTOMAKE_ADD=

if test "$1" = "--clean"; then
  rm -f aclocal.m4 \
        config.guess \
        config.h.in \
        config.sub \
        configure \
        depcomp \
        install-sh \
        ltconfig \
        ltmain.sh \
        missing \
        mkinstalldirs \
        stamp-h*
  exit
elif test "$1" = "--add"; then
  AUTOMAKE_ADD="--add-missing --gnu --copy"
fi


echo "Checking the installed configuration tools..."

if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
  cat <<EOF

  You must have autoconf version $AUTOCONF_VER installed to
  generate configure information and Makefiles for $PROJECT.

  The Autoconf version we are using is a development pre-release
  of the next version of Autoconf.  You can get it here:

  ftp://alpha.gnu.org/gnu/autoconf/autoconf-2.49a.tar.gz
EOF
  DIE=true
fi

if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
    echo
    echo "You must have automake version $AUTOMAKE_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "The Automake version we are using is a development version"
    echo "\"frozen\" from the CVS repository. You can get it here:"
    echo ""
    echo "   ftp://ftp.sim.no/pub/coin/automake-1.4a-coin.tar.gz"
    echo ""
    DIE=true
fi

if test -z "`libtool --version | egrep \"$LIBTOOL_VER\" 2> /dev/null`"; then
    echo
    echo "You must have libtool version $LIBTOOL_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "Get ftp://ftp.gnu.org/pub/libtool/libtool-${LIBTOOL_VER}.tar.gz"
    echo ""
    DIE=true
fi


# The separate $MACRODIR module was added late in the project, and
# since we need to do a cvs checkout to obtain it (cvs update won't do
# with modules), we run this check.

if test ! -d $MACRODIR
then
    cvs -z3 checkout -P $MACRODIR
    if test ! -d $MACRODIR; then
	echo "Couldn't fetch $MACRODIR module!"
        echo
        echo "Directory ``$MACRODIR'' (a separate CVS module) seems to be missing."
        echo "You probably checked out $PROJECT before ``$MACRODIR'' was added."
        echo "Run 'cvs -d :pserver:cvs@cvs.sim.no:/export/cvsroot co $MACRODIR'"
        echo "to try again."
	DIE=true
    fi
fi

$DIE && exit 1

echo "Running aclocal (generating aclocal.m4)..."
aclocal -I $MACRODIR

echo "Running autoheader (generating config.h.in)..."
autoheader

echo "Running automake (generating the Makefile.in files)..."
automake $AUTOMAKE_ADD

echo "Running autoconf (generating ./configure and the Makefile files)..."
autoconf

echo
echo "Done. Now run './configure' and 'make install' to build $PROJECT."
echo

