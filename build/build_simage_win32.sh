#!/bin/bash

# This script will download, (mercurial) checkout and build simage
# from scratch, including all dependent libraries.

# Before running this script, please
# - install cygwin, including wget

# ToDo:
#
# - Copy all license-related docs for all dependant libs 
#   to the installation dir
#

# FIXME 2003-10-10 thammer:
# Verify that everything works if cygwin was built with Unix-style newlines
# instead of dos-style. Check lars' windows-packages stuff for unix2dos


SIMAGE_CRT_LIST="md mdd"
SIMAGE_SOUND_LIST="no"

SIMAGE_PARAMS_OK="no"

if test $# -eq 1; then
  SIMAGE_PARAMS_OK="yes"
fi

if test $# -eq 4; then
  SIMAGE_PARAMS_OK="yes"
fi

if test "X$SIMAGE_PARAMS_OK" = "Xno"; then
  echo "Usage: build_simage_win32.sh <target dir>"
  echo "       build_simage_sin32.sh <sdk dir> <checkout dir> <config dir>"
  echo "                             <inst dir>"
  echo ""
  echo "Ex:    build_simage_win32.sh /cygdrive/c/simage"
  echo ""
  echo "       If the first syntax is used, target dir will be created, and it"
  echo "       will contain four subdirectories named \"sdk\", \"checkout\","
  echo "       \"config\", and \"install\"."
  echo "       If the second syntax is used, the four specified directories"
  echo "       are used instead of one top-level directory (target dir)."
  echo ""
  echo "       The script will download and build simage and all dependent"
  echo "       libraries. If something fails during this process, try fixing"
  echo "       the problem based on the error messages, then _delete_ the"
  echo "       directories affected by the error. This is important because"
  echo "       the script has not been built to be able to continue after"
  echo "       any error."
  echo ""
  echo "       After the script has completed, the \"install\" directory will"
  echo "       contain a binary release of simage, including all dependant"
  echo "       libraries. This release consists of 8 different configurations"
  echo "       using four different CRTs (MT MTD MD MDD), with and without"
  echo "       sound support."
  echo ""
  echo "       Note: The files:"
  echo "               * libungif_win32.diff"
  echo "               * libungif_win32.mak"
  echo "       must be in the current working directory."
  echo ""
  exit
fi

if test $# -eq 1; then
  if ! test -d $1; then
    echo "[SIMAGE]      Creating directory $1"
    mkdir $1
  fi
  SIMAGE_SDK=$1/sdk
  SIMAGE_CHECKOUT=$1/checkout
  SIMAGE_CONFIGURE=$1/config
  SIMAGE_INSTALL=$1/install
fi

if test $# -eq 4; then
  SIMAGE_SDK=$1
  SIMAGE_CHECKOUT=$2
  SIMAGE_CONFIGURE=$3
  SIMAGE_INSTALL=$4
fi

if ! test -d $SIMAGE_CHECKOUT; then
  echo "[SIMAGE]   Creating directory $SIMAGE_CHECKOUT"
  mkdir $SIMAGE_CHECKOUT
fi

if ! test -d $SIMAGE_CHECKOUT; then
  echo "[SIMAGE]   Couldn\'t create directory $SIMAGE_CHECKOUT. Aborting."
  exit
fi

if ! test -d $SIMAGE_CONFIGURE; then
  echo "[SIMAGE]   Creating directory $SIMAGE_CONFIGURE"
  mkdir $SIMAGE_CONFIGURE
fi

if ! test -d $SIMAGE_CONFIGURE; then
  echo "[SIMAGE]   Couldn\'t create directory $SIMAGE_CONFIGURE. Aborting."
  exit
fi

if ! test -d $SIMAGE_INSTALL; then
  echo "[SIMAGE]   Creating directory $SIMAGE_INSTALL"
  mkdir $SIMAGE_INSTALL
fi

if ! test -d $SIMAGE_INSTALL; then
  echo "[SIMAGE]   Couldn\'t create directory $SIMAGE_INSTALL. Aborting."
  exit
fi

function set_simage_param_crt()
{
  if test "X$SIMAGE_CRT" = "Xmt"; then SIMAGE_PARAMCRT="MT"; fi
  if test "X$SIMAGE_CRT" = "Xmtd"; then SIMAGE_PARAMCRT="MTd"; fi
  if test "X$SIMAGE_CRT" = "Xmd"; then SIMAGE_PARAMCRT="MD"; fi
  if test "X$SIMAGE_CRT" = "Xmdd"; then SIMAGE_PARAMCRT="MDd"; fi
}

# Verify SDKs

if ! /usr/bin/bash ./build_simage_libs_win32.sh $SIMAGE_SDK; then
  exit 1
fi

SIMAGE_SDK=$SIMAGE_SDK/dist

# hg update simage

if ! test -d $SIMAGE_CHECKOUT/simage; then
  echo "[SIMAGE]      Checking out simage from scratch"
  cd $SIMAGE_CHECKOUT
  hg clone https://hg.sim.no/simage/default simage
else
  echo "[SIMAGE]      Updating simage"
  cd $SIMAGE_CHECKOUT/simage
  hg update
fi

# configure and make simage

echo "[SIMAGE]      Configuring simage"

for SIMAGE_SOUND in $SIMAGE_SOUND_LIST; do

  if test "X$SIMAGE_SOUND" = "Xyes"; then
    SIMAGE_SOUND_DIRNAME="with_sound"
    SIMAGE_SOUND_NAME="s_"
  else
    SIMAGE_SOUND_DIRNAME="without_sound"
    SIMAGE_SOUND_NAME=""
  fi

  if ! test -d $SIMAGE_CONFIGURE/simage; then
    echo "[SIMAGE]         Creating directory $SIMAGE_CONFIGURE/simage"
    mkdir $SIMAGE_CONFIGURE/simage
  fi

  if ! test -d $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME; then
    echo "[SIMAGE]         Creating directory $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME"
    mkdir $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME
  fi

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do

    if ! test -d $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME/$SIMAGE_CRT; then
      echo "[SIMAGE]         Creating directory $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME/$SIMAGE_CRT"
      mkdir $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME/$SIMAGE_CRT
    fi

    cd $SIMAGE_CONFIGURE/simage/$SIMAGE_SOUND_DIRNAME/$SIMAGE_CRT

    if test -e "config.status"; then
      echo "[SIMAGE]         Rechecking existing configuration in simage/$SIMAGE_SOUND_DIRNAME/$SIMAGE_CRT"
      ./config.status --recheck && ./config.status
    else
      echo "[SIMAGE]         Configuring from scratch in simage/$SIMAGE_SOUND_DIRNAME/$SIMAGE_CRT"

      SIMAGE_SUFFIX=_$SIMAGE_SOUND_NAME$SIMAGE_CRT
      if test "X$SIMAGE_CRT" = "Xmd"; then
        SIMAGE_SUFFIX=
      elif test "X$SIMAGE_CRT" = "Xmdd"; then
        SIMAGE_SUFFIX=d
      fi
  
      SIMAGE_EXTRAFLAGS1=""
      if test "X$SIMAGE_CRT" = "Xmd"; then
        SIMAGE_EXTRAFLAGS1="--disable-debug --disable-symbols"
      elif test "X$SIMAGE_CRT" = "Xmt"; then
        SIMAGE_EXTRAFLAGS1="--disable-debug --disable-symbols"
      fi

      SIMAGE_EXTRAFLAGS2=""
      if test "X$SIMAGE_SOUND" = "Xyes"; then
        SIMAGE_EXTRAFLAGS2="--with-oggvorbis=$SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/ --with-libsndfile=$SIMAGE_SDK/libsndfile/$SIMAGE_CRT"
      fi

      $SIMAGE_CHECKOUT/simage/configure --with-jpeg=$SIMAGE_SDK/libjpeg/$SIMAGE_CRT --with-zlib=$SIMAGE_SDK/zlib/$SIMAGE_CRT --with-png=$SIMAGE_SDK/libpng/$SIMAGE_CRT --with-tiff=$SIMAGE_SDK/libtiff/$SIMAGE_CRT --with-ungif=$SIMAGE_SDK/libungif/$SIMAGE_CRT --with-pic --with-rgb --with-targa --with-avienc --enable-dependency-tracking --with-msvcrt=$SIMAGE_CRT --with-suffix=$SIMAGE_SUFFIX --with-alternate=$SIMAGE_SOUND_NAME$SIMAGE_CRT --prefix=$SIMAGE_INSTALL --enable-static $SIMAGE_EXTRAFLAGS1 $SIMAGE_EXTRAFLAGS2

    fi

    echo "[SIMAGE]         Making install in simage/$SIMAGE_CRT"
    make install

  done # for SIMAGE_CRT

done # for SIMAGE_SOUND

echo "[SIMAGE]   All systems ready to go"


