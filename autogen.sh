#! /bin/sh
# autogen.sh -- generate configure and build files for simage source hierarchy
# 
# Authors:
#   Morten Eriksen, <mortene@sim.no>.
#   Lars J. Aas, <larsa@sim.no>.
#

PROJECT=simage

wd=`echo $0 | sed 's,[^\\/]*$,,g'`;
cd $wd
if test ! -f autogen.sh; then
  echo "unexpected problem with your shell - bailing out"
  exit 1
fi

AUTOCONF_VER=2.49b  # Autoconf from CVS
AUTOMAKE_VER=1.4a   # Automake from CVS
LIBTOOL_VER=1.3.5

MACRODIR=cfg/m4
AUTOMAKE_ADD=

if test x"$1" = x"--clean"; then
  rm -f config.h.in configure stamp-h*
  find . -name Makefile.in | xargs rm
  exit
elif test x"$1" = x"--add"; then
  AUTOMAKE_ADD=""
fi

echo "Checking installed configuration tools..."

if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
  cat <<END

  You must have autoconf version $AUTOCONF_VER installed to generate the
  configure script and Makefile templates for $PROJECT.

  Autoconf $AUTOCONF_VER is the CVS development version of Autoconf, which you
  can retrieve from here:

    :pserver:anoncvs@subversions.gnu.org:/cvs (no password, module "autoconf")
END
  DIE=true
fi

if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
  cat <<END

  You must have automake version $AUTOMAKE_VER installed to generate the
  configure script and Makefile templates for $PROJECT.

  Automake $AUTOMAKE_VER is the CVS developmentn version of Automake, which you
  can retrieve from here:

    :pserver:anoncvs@anoncvs.cygnus.com:/cvs/automake (no password, module automake)
END
    DIE=true
fi

if test -z "`libtool --version | egrep \"$LIBTOOL_VER\" 2> /dev/null`"; then
  cat <<END

  You must have libtool version $LIBTOOL_VER installed to generate the
  configure script and Makefile templates for $PROJECT.  You can fetch it from
  here:

    ftp://ftp.gnu.org/pub/libtool/libtool-${LIBTOOL_VER}.tar.gz
END
    DIE=true
fi

# The separate $MACRODIR module was added late in the project, and
# since we need to do a cvs checkout to obtain it (cvs update won't do
# with modules), we run this check.

if test ! -d $MACRODIR; then
  cvs -z3 checkout -P simage-macros
  if test ! -d $MACRODIR; then
    cat <<END

  Couldn't fetch $MACRODIR module!

  Directory ``$MACRODIR'' (a separate CVS module) seems to be missing.
  You probably checked out $PROJECT before ``$MACRODIR'' was added.
  Run 'cvs -d :pserver:cvs@cvs.sim.no:/export/cvsroot co simage-macros'
  to try again.
END
    DIE=true
  fi
fi

# abnormal exit?
${DIE=false} && echo "" && exit 1

# generate aclocal.m4
echo "Running aclocal..."
aclocal -I $MACRODIR

# generate config.h.in
echo "Running autoheader..."
autoheader

# generate Makefile.in templates
echo "Running automake..."
automake $AUTOMAKE_ADD

# generate configure
echo "Running autoconf..."
autoconf

echo "Done."

