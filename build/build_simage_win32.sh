#!/bin/bash

# This script will download, (cvs) checkout and build simage
# from scratch, including all dependent libraries.

# Before running this script, please
# - install cygwin, including wget
# - make sure you've done cvs login to cvs.coin3d.org

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
  echo "       \"configure\", and \"install\"."
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
  SIMAGE_CONFIGURE=$1/configure
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

echo "[SIMAGE]   Verifying SDKs..."

if ! test -d $SIMAGE_SDK; then
  echo "[SIMAGE]      Creating directory for SDKs - $SIMAGE_SDK"
  mkdir $SIMAGE_SDK
fi

############# libungif
# http://prtr-13.ucsc.edu/~badger/software/libungif/index.shtml
# 

echo "[SIMAGE]      Verifying libungif..."

if ! test -e $SIMAGE_SDK/libungif-4.1.0.tar; then
  if ! test -e $SIMAGE_SDK/libungif-4.1.0.tar.gz; then
    echo "[SIMAGE]         Downloading libungif"
    wget --directory-prefix=$SIMAGE_SDK ftp://prtr-13.ucsc.edu/pub/libungif/libungif-4.1.0.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libungif"
  gunzip $SIMAGE_SDK/libungif-4.1.0.tar.gz
fi

if ! test -d $SIMAGE_SDK/libungif; then
  echo "[SIMAGE]         Extracting libungif"
  mkdir $SIMAGE_SDK/libungif
  mkdir $SIMAGE_SDK/libungif/temp

  if ! test -e ./libungif_win32.mak; then
    echo "[SIMAGE]       File ./libungif_win32.mak is missing."
    echo "[SIMAGE]       Please make sure this file is in current working directory."
    echo "[SIMAGE]       Aborting."
    exit;
  fi

  cp ./libungif_win32.mak $SIMAGE_SDK/libungif/temp

  if ! test -e ./libungif_win32.diff; then
    echo "[SIMAGE]       File ./libungif_win32.diff is missing."
    echo "[SIMAGE]       Please make sure this file is in current working directory."
    echo "[SIMAGE]       Aborting."
    exit;
  fi

  cp ./libungif_win32.diff $SIMAGE_SDK/libungif/temp

  cd $SIMAGE_SDK/libungif/temp
  tar xf $SIMAGE_SDK/libungif-4.1.0.tar

  cd libungif-4.1.0/lib
  /usr/bin/patch -p0 < ./../../libungif_win32.diff

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libungif/$SIMAGE_CRT
    cp -R $SIMAGE_SDK/libungif/temp/libungif-4.1.0/* $SIMAGE_SDK/libungif/$SIMAGE_CRT
    cp $SIMAGE_SDK/libungif/temp/libungif_win32.diff $SIMAGE_SDK/libungif/$SIMAGE_CRT
    cp $SIMAGE_SDK/libungif/temp/libungif_win32.mak $SIMAGE_SDK/libungif/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/libungif/$SIMAGE_CRT/include
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/libungif/temp
  
  echo "[SIMAGE]         Modifying libungif configuration and make files"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cp $SIMAGE_SDK/libungif/$SIMAGE_CRT/lib/*.h $SIMAGE_SDK/libungif/$SIMAGE_CRT/include
    set_simage_param_crt
    cat $SIMAGE_SDK/libungif/$SIMAGE_CRT/libungif_win32.mak | sed -e "s/\/MT /\/$SIMAGE_PARAMCRT /g" | sed -e "s/\/MTd /\/$SIMAGE_PARAMCRT /g" > $SIMAGE_SDK/libungif/$SIMAGE_CRT/makefile.vc_$SIMAGE_CRT

  done
fi

if ! test -e $SIMAGE_SDK/libungif/md/lib/ungif.lib; then
  echo "[SIMAGE]         Making libungif"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libungif/$SIMAGE_CRT

    if test "X$SIMAGE_CRT" = "Xmt"; then 
        SIMAGE_PARAMDEBUG="libungif - Win32 Release"; 
    fi
    if test "X$SIMAGE_CRT" = "Xmtd"; then 
        SIMAGE_PARAMDEBUG="libungif - Win32 Debug"; 
    fi
    if test "X$SIMAGE_CRT" = "Xmd"; then 
        SIMAGE_PARAMDEBUG="libungif - Win32 Release"; 
    fi
    if test "X$SIMAGE_CRT" = "Xmdd"; then 
        SIMAGE_PARAMDEBUG="libungif - Win32 Debug"; 
    fi

    which nmake
    echo nmake -f makefile.vc_$SIMAGE_CRT $SIMAGE_PARAMDEBUG
    nmake -f makefile.vc_$SIMAGE_CRT CFG="$SIMAGE_PARAMDEBUG"
    cp libungif.lib lib/ungif.lib
  done
fi

if ! test -e $SIMAGE_SDK/libungif/md/lib/ungif.lib; then
  echo "[SIMAGE]         Failed to make libungif. Aborting."
  exit;
fi

echo "[SIMAGE]      libungif verified OK"

############# libjpeg

echo "[SIMAGE]      Verifying libjpeg..."

if ! test -e $SIMAGE_SDK/jpegsrc.v6b.tar; then
  if ! test -e $SIMAGE_SDK/jpegsrc.v6b.tar.gz; then
    echo "[SIMAGE]         Downloading libjpeg"
    wget --directory-prefix=$SIMAGE_SDK http://www.ijg.org/files/jpegsrc.v6b.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libjpeg"
  gunzip $SIMAGE_SDK/jpegsrc.v6b.tar.gz
fi

if ! test -d $SIMAGE_SDK/libjpeg; then
  echo "[SIMAGE]         Extracting libjpeg"
  mkdir $SIMAGE_SDK/libjpeg
  mkdir $SIMAGE_SDK/libjpeg/temp
  cd $SIMAGE_SDK/libjpeg/temp
  tar xf $SIMAGE_SDK/jpegsrc.v6b.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libjpeg/$SIMAGE_CRT
    cp $SIMAGE_SDK/libjpeg/temp/jpeg-6b/* $SIMAGE_SDK/libjpeg/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/include
    mkdir $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/lib
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/libjpeg/temp
  
  echo "[SIMAGE]         Modifying libjpeg configuration and make files"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cp $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/jconfig.vc $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/jconfig.h
    cp $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/*.h $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/include
    set_simage_param_crt
    cat $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/makefile.vc | sed -e "s/CFLAGS= /CFLAGS= -$SIMAGE_PARAMCRT /g" | sed -e 's/RM= del/RM= rm -f/g' > $SIMAGE_SDK/libjpeg/$SIMAGE_CRT/makefile.vc_$SIMAGE_CRT
  done
fi

if ! test -e $SIMAGE_SDK/libjpeg/mt/lib/jpeg.lib; then
  echo "[SIMAGE]         Making libjpeg"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libjpeg/$SIMAGE_CRT

    if test "X$SIMAGE_CRT" = "Xmt"; then SIMAGE_PARAMDEBUG=""; fi
    if test "X$SIMAGE_CRT" = "Xmtd"; then SIMAGE_PARAMDEBUG="nodebug=1"; fi
    if test "X$SIMAGE_CRT" = "Xmd"; then SIMAGE_PARAMDEBUG=""; fi
    if test "X$SIMAGE_CRT" = "Xmdd"; then SIMAGE_PARAMDEBUG="nodebug=1"; fi

    nmake -f makefile.vc_$SIMAGE_CRT $SIMAGE_PARAMDEBUG
    cp libjpeg.lib lib/jpeg.lib
  done
fi

if ! test -e $SIMAGE_SDK/libjpeg/mt/lib/jpeg.lib; then
  echo "[SIMAGE]         Failed to make libjpeg. Aborting."
  exit;
fi

echo "[SIMAGE]      libjpeg verified OK"

############# zlib

echo "[SIMAGE]      Verifying zlib..."

if ! test -e $SIMAGE_SDK/zlib-1.1.4.tar; then
  if ! test -e $SIMAGE_SDK/zlib-1.1.4.tar.gz; then
    echo "[SIMAGE]         Downloading zlib"
    wget --directory-prefix=$SIMAGE_SDK ftp://ftp.info-zip.org/pub/infozip/zlib/zlib-1.1.4.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing zlib"
  gunzip $SIMAGE_SDK/zlib-1.1.4.tar.gz
fi

if ! test -d $SIMAGE_SDK/zlib; then
  echo "[SIMAGE]         Extracting zlib"
  mkdir $SIMAGE_SDK/zlib
  mkdir $SIMAGE_SDK/zlib/temp
  cd $SIMAGE_SDK/zlib/temp
  tar xf $SIMAGE_SDK/zlib-1.1.4.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/zlib/$SIMAGE_CRT
    cp -R $SIMAGE_SDK/zlib/temp/zlib-1.1.4/* $SIMAGE_SDK/zlib/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/zlib/$SIMAGE_CRT/include
    mkdir $SIMAGE_SDK/zlib/$SIMAGE_CRT/lib
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/zlib/temp
  
  echo "[SIMAGE]         Modifying zlib configuration and make files"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cp $SIMAGE_SDK/zlib/$SIMAGE_CRT/*.h $SIMAGE_SDK/zlib/$SIMAGE_CRT/include
    set_simage_param_crt
    cat $SIMAGE_SDK/zlib/$SIMAGE_CRT/msdos/Makefile.w32 | sed -e "s/CFLAGS=/CFLAGS= -$SIMAGE_PARAMCRT /g" > $SIMAGE_SDK/zlib/$SIMAGE_CRT/makefile.vc_$SIMAGE_CRT
  done
fi

if ! test -e $SIMAGE_SDK/zlib/mt/lib/zlib.lib; then
  echo "[SIMAGE]         Making zlib"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/zlib/$SIMAGE_CRT
    nmake -f makefile.vc_$SIMAGE_CRT
    cp zlib.lib lib/zlib.lib
  done
fi

if ! test -e $SIMAGE_SDK/zlib/mt/lib/zlib.lib; then
  echo "[SIMAGE]         Failed to make zlib. Aborting."
  exit;
fi

echo "[SIMAGE]      zlib verified OK"

############# libpng
# http://www.libpng.org/pub/png/libpng.html

echo "[SIMAGE]      Verifying libpng..."

if ! test -e $SIMAGE_SDK/libpng-1.2.5.tar; then
  if ! test -e $SIMAGE_SDK/libpng-1.2.5.tar.gz; then
    echo "[SIMAGE]         Downloading libpng"
    wget --directory-prefix=$SIMAGE_SDK ftp://ftp.simplesystems.org/pub/libpng/src/libpng-1.2.5.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libpng"
  gunzip $SIMAGE_SDK/libpng-1.2.5.tar.gz
fi

if ! test -d $SIMAGE_SDK/libpng; then
  echo "[SIMAGE]         Extracting libpng"
  mkdir $SIMAGE_SDK/libpng
  mkdir $SIMAGE_SDK/libpng/temp
  cd $SIMAGE_SDK/libpng/temp
  tar xf $SIMAGE_SDK/libpng-1.2.5.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libpng/$SIMAGE_CRT
    cp -R $SIMAGE_SDK/libpng/temp/libpng-1.2.5/* $SIMAGE_SDK/libpng/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/libpng/$SIMAGE_CRT/include
    mkdir $SIMAGE_SDK/libpng/$SIMAGE_CRT/lib
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/libpng/temp
  
  echo "[SIMAGE]         Modifying libpng configuration and make files"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cp $SIMAGE_SDK/libpng/$SIMAGE_CRT/*.h $SIMAGE_SDK/libpng/$SIMAGE_CRT/include
    set_simage_param_crt
    cat $SIMAGE_SDK/libpng/$SIMAGE_CRT/scripts/makefile.vcwin32 | sed -e "s/CFLAGS= /CFLAGS= -$SIMAGE_PARAMCRT /g" | sed -e "s/-I\\.\\.\\\\zlib/-I\\.\\.\\\\\\.\\.\\\\zlib\\\\$SIMAGE_CRT/g" > $SIMAGE_SDK/libpng/$SIMAGE_CRT/makefile.vc_$SIMAGE_CRT
  done
fi

if ! test -e $SIMAGE_SDK/libpng/mt/lib/png.lib; then
  echo "[SIMAGE]         Making libpng"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libpng/$SIMAGE_CRT
    nmake -f makefile.vc_$SIMAGE_CRT
    cp libpng.lib lib/png.lib
  done
fi

if ! test -e $SIMAGE_SDK/libpng/mt/lib/png.lib; then
  echo "[SIMAGE]         Failed to make libpng. Aborting."
  exit;
fi

echo "[SIMAGE]      libpng verified OK"

############# libtiff
# http://www.libtiff.org/

echo "[SIMAGE]      Verifying libtiff..."

if ! test -e $SIMAGE_SDK/tiff-v3.5.7.tar; then
  if ! test -e $SIMAGE_SDK/tiff-v3.5.7.tar.gz; then
    echo "[SIMAGE]         Downloading libtiff"
    wget --directory-prefix=$SIMAGE_SDK ftp://ftp.remotesensing.org/pub/libtiff/tiff-v3.5.7.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libtiff"
  gunzip $SIMAGE_SDK/tiff-v3.5.7.tar.gz
fi

if ! test -d $SIMAGE_SDK/libtiff; then
  echo "[SIMAGE]         Extracting libtiff"
  mkdir $SIMAGE_SDK/libtiff
  mkdir $SIMAGE_SDK/libtiff/temp
  cd $SIMAGE_SDK/libtiff/temp
  tar xf $SIMAGE_SDK/tiff-v3.5.7.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libtiff/$SIMAGE_CRT
    cp -R $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7./libtiff/* $SIMAGE_SDK/libtiff/$SIMAGE_CRT
    cp $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7/VERSION $SIMAGE_SDK/libtiff
    cp $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7/RELEASE-DATE $SIMAGE_SDK/libtiff
    cp -R $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7/dist $SIMAGE_SDK/libtiff
    mkdir $SIMAGE_SDK/libtiff/$SIMAGE_CRT/include
    mkdir $SIMAGE_SDK/libtiff/$SIMAGE_CRT/lib
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/libtiff/temp
  
  echo "[SIMAGE]         Modifying libtiff configuration and make files"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cp $SIMAGE_SDK/libtiff/$SIMAGE_CRT/*.h $SIMAGE_SDK/libtiff/$SIMAGE_CRT/include
    set_simage_param_crt
    cat $SIMAGE_SDK/libtiff/$SIMAGE_CRT/makefile.vc | sed -e "s/CFLAGS  = /CFLAGS  = -$SIMAGE_PARAMCRT /g" > $SIMAGE_SDK/libtiff/$SIMAGE_CRT/makefile.vc_$SIMAGE_CRT
  done
fi

if ! test -e $SIMAGE_SDK/libtiff/mt/lib/tiff.lib; then
  echo "[SIMAGE]         Making libtiff"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libtiff/$SIMAGE_CRT
    nmake -f makefile.vc_$SIMAGE_CRT
    cp libtiff.lib lib/tiff.lib
  done
fi

if ! test -e $SIMAGE_SDK/libtiff/mt/lib/tiff.lib; then
  echo "[SIMAGE]         Failed to make libtiff. Aborting."
  exit;
fi

echo "[SIMAGE]      libtiff verified OK"

############# ogg
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGE]      Verifying libogg..."

if ! test -e $SIMAGE_SDK/libogg-1.0.tar; then
  if ! test -e $SIMAGE_SDK/libogg-1.0.tar.gz; then
    echo "[SIMAGE]         Downloading libogg"
    wget --directory-prefix=$SIMAGE_SDK http://www.xiph.org/ogg/vorbis/download/libogg-1.0.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libogg"
  gunzip $SIMAGE_SDK/libogg-1.0.tar.gz
fi

if ! test -d $SIMAGE_SDK/libogg; then
  echo "[SIMAGE]         Extracting libogg"
  mkdir $SIMAGE_SDK/libogg
  mkdir $SIMAGE_SDK/libogg/temp
  cd $SIMAGE_SDK/libogg/temp
  tar xf $SIMAGE_SDK/libogg-1.0.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libogg/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/libogg/$SIMAGE_CRT/lib
    cp -R $SIMAGE_SDK/libogg/temp/libogg-1.0/* $SIMAGE_SDK/libogg/$SIMAGE_CRT
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/libogg/temp
  
  echo "[SIMAGE]         Modifying libogg configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGE_SDK/libogg/md/win32/ogg_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static \- Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/md/win32/ogg_static_md.dsp
  cat $SIMAGE_SDK/libogg/mdd/win32/ogg_static.dsp | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg\.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mdd/win32/ogg_static_mdd.dsp

  cat $SIMAGE_SDK/libogg/mt/win32/ogg_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static - Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mt/win32/ogg_static_mt.dsp
  cat $SIMAGE_SDK/libogg/mtd/win32/ogg_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mtd/win32/ogg_static_mtd.dsp

fi

if ! test -e $SIMAGE_SDK/libogg/mt/win32/ogg.lib; then
  echo "[SIMAGE]         Making libogg"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libogg/$SIMAGE_CRT/win32
    SIMAGE_OLDINCLUDE=$INCLUDE
    SIMAGE_TEMP1=`cygpath -w $SIMAGE_SDK/libogg/$SIMAGE_CRT/include`
    INCLUDE=$INCLUDE\;$SIMAGE_TEMP1
    cmd /C msdev ogg_static_$SIMAGE_CRT.dsp /useenv /make "ogg_static - Win32 Simage" /rebuild
    INCLUDE=$SIMAGE_OLDINCLUDE
  done
fi

if ! test -e $SIMAGE_SDK/libogg/mt/win32/ogg.lib; then
  echo "[SIMAGE]         Failed to make libogg. Aborting."
  exit;
fi

echo "[SIMAGE]      ogg verified OK"

############# vorbis
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGE]      Verifying libvorbis..."

if ! test -e $SIMAGE_SDK/libvorbis-1.0.tar; then
  if ! test -e $SIMAGE_SDK/libvorbis-1.0.tar.gz; then
    echo "[SIMAGE]         Downloading libvorbis"
    wget --directory-prefix=$SIMAGE_SDK http://www.xiph.org/ogg/vorbis/download/libvorbis-1.0.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libvorbis"
  gunzip $SIMAGE_SDK/libvorbis-1.0.tar.gz
fi

if ! test -d $SIMAGE_SDK/libvorbis; then
  echo "[SIMAGE]         Extracting libvorbis"
  mkdir $SIMAGE_SDK/libvorbis
  mkdir $SIMAGE_SDK/libvorbis/temp
  cd $SIMAGE_SDK/libvorbis/temp
  tar xf $SIMAGE_SDK/libvorbis-1.0.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libvorbis/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/lib
    cp -R $SIMAGE_SDK/libvorbis/temp/libvorbis-1.0/* $SIMAGE_SDK/libvorbis/$SIMAGE_CRT
  done

  # rm -R $SIMAGE_SDK/libvorbis/temp
  
  echo "[SIMAGE]         Modifying libvorbis configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGE_SDK/libvorbis/md/win32/vorbis_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbis.lib/g" | sed -e "s/vorbis_static \- Win32 Release/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/md/win32/vorbis_static_md.dsp
  cat $SIMAGE_SDK/libvorbis/mdd/win32/vorbis_static.dsp | sed -e "s/Vorbis_Static_Debug\\\\vorbis_static_d\.lib/vorbis\.lib/g" | sed -e "s/vorbis_static - Win32 Debug/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/mdd/win32/vorbis_static_mdd.dsp

  cat $SIMAGE_SDK/libvorbis/mt/win32/vorbis_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbis.lib/g" | sed -e "s/vorbis_static - Win32 Release/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/mt/win32/vorbis_static_mt.dsp
  cat $SIMAGE_SDK/libvorbis/mtd/win32/vorbis_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/Vorbis_Static_Debug\\\\vorbis_static_d\.lib/vorbis.lib/g" | sed -e "s/vorbis_static - Win32 Debug/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/mtd/win32/vorbis_static_mtd.dsp

fi

if ! test -e $SIMAGE_SDK/libvorbis/mt/win32/vorbis.lib; then
  echo "[SIMAGE]         Making libvorbis"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/win32
    SIMAGE_OLDINCLUDE=$INCLUDE
    SIMAGE_TEMP1=`cygpath -w $SIMAGE_SDK/libogg/$SIMAGE_CRT/include`
    SIMAGE_TEMP2=`cygpath -w $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/include`
    INCLUDE=$INCLUDE\;$SIMAGE_TEMP1\;$SIMAGE_TEMP2
    cmd /C echo %INCLUDE%
    cmd /C msdev vorbis_static_$SIMAGE_CRT.dsp /useenv /make "vorbis_static - Win32 Simage" /rebuild
    INCLUDE=$SIMAGE_OLDINCLUDE
  done
fi

if ! test -e $SIMAGE_SDK/libvorbis/mt/win32/vorbis.lib; then
  echo "[SIMAGE]         Failed to make libvorbis. Aborting."
  exit;
fi

echo "[SIMAGE]      libvorbis verified OK"

############# vorbisfile
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGE]      Verifying libvorbisfile..."

# libvorbisfile is part of libvorbis

if ! test -e $SIMAGE_SDK/libvorbis/mt/win32/vorbisfile_static_mt.dsp; then
  echo "[SIMAGE]         Modifying libvorbisfile configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGE_SDK/libvorbis/md/win32/vorbisfile_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbisfile.lib/g" | sed -e "s/vorbisfile_static \- Win32 Release/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/md/win32/vorbisfile_static_md.dsp
  cat $SIMAGE_SDK/libvorbis/mdd/win32/vorbisfile_static.dsp | sed -e "s/VorbisFile_Static_Debug\\\\vorbisfile_static_d\.lib/vorbisfile\.lib/g" | sed -e "s/vorbisfile_static - Win32 Debug/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/mdd/win32/vorbisfile_static_mdd.dsp

  cat $SIMAGE_SDK/libvorbis/mt/win32/vorbisfile_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbisfile.lib/g" | sed -e "s/vorbisfile_static - Win32 Release/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/mt/win32/vorbisfile_static_mt.dsp
  cat $SIMAGE_SDK/libvorbis/mtd/win32/vorbisfile_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/VorbisFile_Static_Debug\\\\vorbisfile_static_d\.lib/vorbisfile.lib/g" | sed -e "s/vorbisfile_static - Win32 Debug/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libvorbis/mtd/win32/vorbisfile_static_mtd.dsp

fi

if ! test -e $SIMAGE_SDK/libvorbis/mt/win32/vorbisfile.lib; then
  echo "[SIMAGE]         Making libvorbisfile"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/win32
    SIMAGE_OLDINCLUDE=$INCLUDE
    SIMAGE_TEMP1=`cygpath -w $SIMAGE_SDK/libogg/$SIMAGE_CRT/include`
    SIMAGE_TEMP2=`cygpath -w $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/include`
    INCLUDE=$INCLUDE\;$SIMAGE_TEMP1\;$SIMAGE_TEMP2
    cmd /C echo %INCLUDE%
    cmd /C msdev vorbisfile_static_$SIMAGE_CRT.dsp /useenv /make "vorbisfile_static - Win32 Simage" /rebuild
    INCLUDE=$SIMAGE_OLDINCLUDE
  done
fi

if ! test -e $SIMAGE_SDK/libvorbis/mt/win32/vorbisfile.lib; then
  echo "[SIMAGE]         Failed to make libvorbisfile. Aborting."
  exit;
fi

echo "[SIMAGE]      libvorbisfile verified OK"


########### oggvorbis = ogg + vorbis + vorbisfile

if ! test -d $SIMAGE_SDK/oggvorbis; then
  echo "[SIMAGE]      Creating and populating oggvorbis directory structure"
  mkdir $SIMAGE_SDK/oggvorbis
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT
    mkdir $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/include
    mkdir $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/lib
    cp -R $SIMAGE_SDK/libogg/$SIMAGE_CRT/include/ogg $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/include
    cp -R $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/include/vorbis $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/include
    cp -R $SIMAGE_SDK/libogg/$SIMAGE_CRT/win32/ogg.lib $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/lib
    cp -R $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/win32/vorbis.lib $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/lib
    cp -R $SIMAGE_SDK/libvorbis/$SIMAGE_CRT/win32/vorbisfile.lib $SIMAGE_SDK/oggvorbis/$SIMAGE_CRT/lib
  done
fi

echo "[SIMAGE]      oggvorbis verified OK"

############# libsndfile
# http://www.zip.com.au/~erikd/libsndfile/

echo "[SIMAGE]      Verifying libsndfile..."

if ! test -e $SIMAGE_SDK/libsndfile-1.0.5.tar; then
  if ! test -e $SIMAGE_SDK/libsndfile-1.0.5.tar.gz; then
    echo "[SIMAGE]         Downloading libsndfile"
    wget --directory-prefix=$SIMAGE_SDK http://www.zip.com.au/~erikd/libsndfile/libsndfile-1.0.5.tar.gz
  fi
  echo "[SIMAGE]         gunzip'ing libsndfile"
  gunzip $SIMAGE_SDK/libsndfile-1.0.5.tar.gz
fi

if ! test -d $SIMAGE_SDK/libsndfile; then
  echo "[SIMAGE]         Extracting libsndfile"
  mkdir $SIMAGE_SDK/libsndfile
  mkdir $SIMAGE_SDK/libsndfile/temp
  cd $SIMAGE_SDK/libsndfile/temp
  tar xf $SIMAGE_SDK/libsndfile-1.0.5.tar

  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    mkdir $SIMAGE_SDK/libsndfile/$SIMAGE_CRT
    cp -R $SIMAGE_SDK/libsndfile/temp/libsndfile-1.0.5/* $SIMAGE_SDK/libsndfile/$SIMAGE_CRT
  done

  cd $SIMAGE_SDK

  rm -R $SIMAGE_SDK/libsndfile/temp
  
  echo "[SIMAGE]         Modifying libsndfile configuration and make files"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cp $SIMAGE_SDK/libsndfile/$SIMAGE_CRT/Win32/sndfile.h $SIMAGE_SDK/libsndfile/$SIMAGE_CRT/src
    set_simage_param_crt
    SIMAGE_TEMP1=`which cl | sed -e "s/\/bin\/cl$//g" | sed "s/ /\\\\\\ /g"`
    SIMAGE_MSVCDIR=`cygpath -w "$SIMAGE_TEMP1"`
    
    # Note 2003-05-21 thammer: The line below is to be used if
    # libsndfile is built as a dll.
    # cat $SIMAGE_SDK/libsndfile/$SIMAGE_CRT/Win32/Makefile.msvc | sed -e "s/MSVCDir=.*/MSVCDir=$SIMAGE_MSVCDIR/g" | sed -e "s/\/MD/\/$SIMAGE_PARAMCRT/g" > $SIMAGE_SDK/libsndfile/$SIMAGE_CRT/Win32/Makefile.msvc_$SIMAGE_CRT
    
    cat $SIMAGE_SDK/libsndfile/$SIMAGE_CRT/Win32/Makefile.msvc | sed -e "s/MSVCDir=.*/MSVCDir=$SIMAGE_MSVCDIR/g" | sed -e "s/\/MD/\/$SIMAGE_PARAMCRT/g" | sed -e "s/DLL_LINK_FLAGS=\/nologo.*$/DLL_LINK_FLAGS=-lib \/nologo/" | sed -e "s/_USRDLL/_LIB/" > $SIMAGE_SDK/libsndfile/$SIMAGE_CRT/Win32/Makefile.msvc_$SIMAGE_CRT
  done
fi

if ! test -e $SIMAGE_SDK/libsndfile/mt/libsndfile.lib; then
  echo "[SIMAGE]         Making libsndfile"
  for SIMAGE_CRT in $SIMAGE_CRT_LIST; do
    cd $SIMAGE_SDK/libsndfile/$SIMAGE_CRT
    nmake -f Win32/Makefile.msvc_$SIMAGE_CRT
  done
fi

if ! test -e $SIMAGE_SDK/libsndfile/mt/libsndfile.lib; then
  echo "[SIMAGE]         Failed to make libsndfile. Aborting."
  exit;
fi

# Note 2003-05-21 thammer: The line below is to be used if
# libsndfile is built as a dll. Libsndfile is under the LGPL.
# cp $SIMAGE_SDK/libsndfile/mt/libsndfile.dll $SIMAGE_INSTALL/bin

echo "[SIMAGE]      libsndfile verified OK"



# cvs update simage

if ! test -d $SIMAGE_CHECKOUT/simage; then
  echo "[SIMAGE]      Checking out simage from scratch"
  cd $SIMAGE_CHECKOUT
  cvs -z3 -d:pserver:cvs@cvs.coin3d.org:/export/cvsroot co simage
else
  echo "[SIMAGE]      Updating simage"
  cd $SIMAGE_CHECKOUT/simage
  cvs -z3 update -dP
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


