#!/bin/bash

# This script will download and build all libraries simage depends on
#
# Before running this script, please
# - Install cygwin, including wget
# - Make sure VisualC++ V6's vcvars32.bat has been run - i.e.
#   that the environment variables for this compiler has been set up
#   correctly.
#   This script has been designed to work with VisualC++ v6.0 only,
#   and will require minor changes to work with newer versions of VisualC++.
#
# ToDo:
#
# - Copy all license-related docs for all dependant libs 
#   to the installation dir
#
#
# FIXME 2003-10-10 thammer:
# Verify that everything works if cygwin was built with Unix-style newlines
# instead of dos-style. Check lars' windows-packages stuff for unix2dos

SIMAGELIBS_CRT_LIST="md mdd"

SIMAGELIBS_PARAMS_OK="no"

if test $# -eq 1; then
  SIMAGELIBS_PARAMS_OK="yes"
fi

if test $# -eq 1; then
  if ! test -d $1; then
    echo "[SIMAGELIBS]      Creating directory $1"
    mkdir $1
  fi
  # make sure it's an absolute path
  ABSPATH=`cd $1; pwd`
  SIMAGELIBS_DOWNLOAD=$ABSPATH/download
  SIMAGELIBS_BUILD=$ABSPATH/build
  SIMAGELIBS_DIST=$ABSPATH/dist
fi

if test "X$SIMAGELIBS_PARAMS_OK" = "Xno"; then
  echo "Usage: build_simage_libs_win32.sh <target dir>"
  echo ""
  echo "Ex:    build_simage_win32.sh /cygdrive/c/simagelibs"
  echo ""
  echo "       The script will download and build all libraries simage depends"
  echo "       on. If something fails during this process, try fixing"
  echo "       the problem based on the error messages, then _delete_ the"
  echo "       directories affected by the error. This is important because"
  echo "       the script has not been built to be able to continue after"
  echo "       any error."
  echo ""
  echo "       Note: The files:"
  echo "               * libungif_win32.diff"
  echo "               * libungif_win32.mak"
  echo "       must be in the current working directory."
  echo ""
  exit 1
fi

function set_simage_param_crt()
{
  if test "X$SIMAGELIBS_CRT" = "Xmt"; then SIMAGELIBS_PARAMCRT="MT"; fi
  if test "X$SIMAGELIBS_CRT" = "Xmtd"; then SIMAGELIBS_PARAMCRT="MTd"; fi
  if test "X$SIMAGELIBS_CRT" = "Xmd"; then SIMAGELIBS_PARAMCRT="MD"; fi
  if test "X$SIMAGELIBS_CRT" = "Xmdd"; then SIMAGELIBS_PARAMCRT="MDd"; fi
}

# Verify SDKs

echo "[SIMAGELIBS]   Verifying SDKs..."

if ! test -d $SIMAGELIBS_DOWNLOAD; then
  echo "[SIMAGELIBS]      Creating temp directory for downloaded SDKs - $SIMAGELIBS_DOWNLOAD"
  mkdir $SIMAGELIBS_DOWNLOAD
fi

if ! test -d $SIMAGELIBS_BUILD; then
  echo "[SIMAGELIBS]      Creating temp directory for building SDKs - $SIMAGELIBS_BUILD"
  mkdir $SIMAGELIBS_BUILD
fi

############# libungif
# http://prtr-13.ucsc.edu/~badger/software/libungif/index.shtml
# ftp://prtr-13.ucsc.edu/pub/libungif/libungif-4.1.0.tar.gz
# ftp://ftp.coin3d.org/pub/3rdparty/libungif-4.1.0.tar.gz
#

echo "[SIMAGELIBS]      Verifying libungif..."

if ! test -e $SIMAGELIBS_DOWNLOAD/libungif-4.1.0.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/libungif-4.1.0.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libungif"
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.coin3d.org/pub/3rdparty/libungif-4.1.0.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libungif"
  gunzip $SIMAGELIBS_DOWNLOAD/libungif-4.1.0.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libungif; then
  echo "[SIMAGELIBS]         Extracting libungif"
  mkdir $SIMAGELIBS_BUILD/libungif
  mkdir $SIMAGELIBS_BUILD/libungif/temp

  if ! test -e ./libungif_win32.mak; then
    echo "[SIMAGELIBS]       File ./libungif_win32.mak is missing."
    echo "[SIMAGELIBS]       Please make sure this file is in current working directory."
    echo "[SIMAGELIBS]       Aborting."
    exit 1;
  fi

  cp ./libungif_win32.mak $SIMAGELIBS_BUILD/libungif/temp

  if ! test -e ./libungif_win32.diff; then
    echo "[SIMAGELIBS]       File ./libungif_win32.diff is missing."
    echo "[SIMAGELIBS]       Please make sure this file is in current working directory."
    echo "[SIMAGELIBS]       Aborting."
    exit 1;
  fi

  cp ./libungif_win32.diff $SIMAGELIBS_BUILD/libungif/temp

  cd $SIMAGELIBS_BUILD/libungif/temp
  tar xf $SIMAGELIBS_DOWNLOAD/libungif-4.1.0.tar

  cd libungif-4.1.0/lib
  /usr/bin/patch -p0 < ./../../libungif_win32.diff

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT
    cp -R $SIMAGELIBS_BUILD/libungif/temp/libungif-4.1.0/* $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT
    cp $SIMAGELIBS_BUILD/libungif/temp/libungif_win32.diff $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT
    cp $SIMAGELIBS_BUILD/libungif/temp/libungif_win32.mak $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/include
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/libungif/temp
  
  echo "[SIMAGELIBS]         Modifying libungif configuration and make files"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cp $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/lib/*.h $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/include
    set_simage_param_crt
    cat $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/libungif_win32.mak | sed -e "s/\/MT /\/$SIMAGELIBS_PARAMCRT /g" | sed -e "s/\/MTd /\/$SIMAGELIBS_PARAMCRT /g" > $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/makefile.vc_$SIMAGELIBS_CRT

  done
fi

if ! test -e $SIMAGELIBS_BUILD/libungif/md/lib/ungif.lib; then
  echo "[SIMAGELIBS]         Making libungif"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT

    if test "X$SIMAGELIBS_CRT" = "Xmt"; then 
        SIMAGELIBS_PARAMDEBUG="libungif - Win32 Release"; 
    fi
    if test "X$SIMAGELIBS_CRT" = "Xmtd"; then 
        SIMAGELIBS_PARAMDEBUG="libungif - Win32 Debug"; 
    fi
    if test "X$SIMAGELIBS_CRT" = "Xmd"; then 
        SIMAGELIBS_PARAMDEBUG="libungif - Win32 Release"; 
    fi
    if test "X$SIMAGELIBS_CRT" = "Xmdd"; then 
        SIMAGELIBS_PARAMDEBUG="libungif - Win32 Debug"; 
    fi

    which nmake
    echo nmake -f makefile.vc_$SIMAGELIBS_CRT $SIMAGELIBS_PARAMDEBUG
    nmake -f makefile.vc_$SIMAGELIBS_CRT CFG="$SIMAGELIBS_PARAMDEBUG"
    cp libungif.lib lib/ungif.lib
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libungif/md/lib/ungif.lib; then
  echo "[SIMAGELIBS]         Failed to make libungif. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      libungif verified OK"

############# libjpeg

echo "[SIMAGELIBS]      Verifying libjpeg..."

if ! test -e $SIMAGELIBS_DOWNLOAD/jpegsrc.v6b.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/jpegsrc.v6b.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libjpeg"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD http://www.ijg.org/files/jpegsrc.v6b.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/jpegsrc.v6b.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libjpeg"
  gunzip $SIMAGELIBS_DOWNLOAD/jpegsrc.v6b.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libjpeg; then
  echo "[SIMAGELIBS]         Extracting libjpeg"
  mkdir $SIMAGELIBS_BUILD/libjpeg
  mkdir $SIMAGELIBS_BUILD/libjpeg/temp
  cd $SIMAGELIBS_BUILD/libjpeg/temp
  tar xf $SIMAGELIBS_DOWNLOAD/jpegsrc.v6b.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT
    cp $SIMAGELIBS_BUILD/libjpeg/temp/jpeg-6b/* $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/include
    mkdir $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/lib
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/libjpeg/temp
  
  echo "[SIMAGELIBS]         Modifying libjpeg configuration and make files"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cp $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/jconfig.vc $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/jconfig.h
    cp $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/jpeglib.h $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/include
    cp $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/jconfig.h $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/include
    cp $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/jmorecfg.h $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/include
    cp $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/jerror.h $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/include
    set_simage_param_crt
    cat $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/makefile.vc | sed -e "s/CFLAGS= /CFLAGS= -$SIMAGELIBS_PARAMCRT /g" | sed -e 's/RM= del/RM= rm -f/g' > $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/makefile.vc_$SIMAGELIBS_CRT
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libjpeg/md/lib/jpeg.lib; then
  echo "[SIMAGELIBS]         Making libjpeg"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT

    if test "X$SIMAGELIBS_CRT" = "Xmt"; then SIMAGELIBS_PARAMDEBUG=""; fi
    if test "X$SIMAGELIBS_CRT" = "Xmtd"; then SIMAGELIBS_PARAMDEBUG="nodebug=1"; fi
    if test "X$SIMAGELIBS_CRT" = "Xmd"; then SIMAGELIBS_PARAMDEBUG=""; fi
    if test "X$SIMAGELIBS_CRT" = "Xmdd"; then SIMAGELIBS_PARAMDEBUG="nodebug=1"; fi

    nmake -f makefile.vc_$SIMAGELIBS_CRT $SIMAGELIBS_PARAMDEBUG
    cp libjpeg.lib lib/jpeg.lib
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libjpeg/md/lib/jpeg.lib; then
  echo "[SIMAGELIBS]         Failed to make libjpeg. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      libjpeg verified OK"

############# zlib

echo "[SIMAGELIBS]      Verifying zlib..."

if ! test -e $SIMAGELIBS_DOWNLOAD/zlib-1.1.4.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/zlib-1.1.4.tar.gz; then
    echo "[SIMAGELIBS]         Downloading zlib"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.info-zip.org/pub/infozip/zlib/zlib-1.1.4.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/zlib-1.1.4.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing zlib"
  gunzip $SIMAGELIBS_DOWNLOAD/zlib-1.1.4.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/zlib; then
  echo "[SIMAGELIBS]         Extracting zlib"
  mkdir $SIMAGELIBS_BUILD/zlib
  mkdir $SIMAGELIBS_BUILD/zlib/temp
  cd $SIMAGELIBS_BUILD/zlib/temp
  tar xf $SIMAGELIBS_DOWNLOAD/zlib-1.1.4.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT
    cp -R $SIMAGELIBS_BUILD/zlib/temp/zlib-1.1.4/* $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/include
    mkdir $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/lib
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/zlib/temp
  
  echo "[SIMAGELIBS]         Modifying zlib configuration and make files"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cp $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/zlib.h $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/include
    cp $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/zconf.h $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/include
    set_simage_param_crt
    cat $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/msdos/Makefile.w32 | sed -e "s/CFLAGS=/CFLAGS= -$SIMAGELIBS_PARAMCRT /g" > $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/makefile.vc_$SIMAGELIBS_CRT
  done
fi

if ! test -e $SIMAGELIBS_BUILD/zlib/md/lib/zlib.lib; then
  echo "[SIMAGELIBS]         Making zlib"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT
    nmake -f makefile.vc_$SIMAGELIBS_CRT
    cp zlib.lib lib/zlib.lib
  done
fi

if ! test -e $SIMAGELIBS_BUILD/zlib/md/lib/zlib.lib; then
  echo "[SIMAGELIBS]         Failed to make zlib. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      zlib verified OK"

############# libpng
# http://www.libpng.org/pub/png/libpng.html

echo "[SIMAGELIBS]      Verifying libpng..."

if ! test -e $SIMAGELIBS_DOWNLOAD/libpng-1.2.5.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/libpng-1.2.5.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libpng"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.simplesystems.org/pub/libpng/src/libpng-1.2.5.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/libpng-1.2.5.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libpng"
  gunzip $SIMAGELIBS_DOWNLOAD/libpng-1.2.5.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libpng; then
  echo "[SIMAGELIBS]         Extracting libpng"
  mkdir $SIMAGELIBS_BUILD/libpng
  mkdir $SIMAGELIBS_BUILD/libpng/temp
  cd $SIMAGELIBS_BUILD/libpng/temp
  tar xf $SIMAGELIBS_DOWNLOAD/libpng-1.2.5.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT
    cp -R $SIMAGELIBS_BUILD/libpng/temp/libpng-1.2.5/* $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/include
    mkdir $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/lib
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/libpng/temp
  
  echo "[SIMAGELIBS]         Modifying libpng configuration and make files"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cp $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/*.h $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/include
    set_simage_param_crt
    cat $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/scripts/makefile.vcwin32 | sed -e "s/CFLAGS= /CFLAGS= -$SIMAGELIBS_PARAMCRT /g" | sed -e "s/-I\\.\\.\\\\zlib/-I\\.\\.\\\\\\.\\.\\\\zlib\\\\$SIMAGELIBS_CRT/g" > $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/makefile.vc_$SIMAGELIBS_CRT
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libpng/md/lib/png.lib; then
  echo "[SIMAGELIBS]         Making libpng"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT
    nmake -f makefile.vc_$SIMAGELIBS_CRT
    cp libpng.lib lib/png.lib
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libpng/md/lib/png.lib; then
  echo "[SIMAGELIBS]         Failed to make libpng. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      libpng verified OK"

############# libtiff
# http://www.libtiff.org/

echo "[SIMAGELIBS]      Verifying libtiff..."

if ! test -e $SIMAGELIBS_DOWNLOAD/tiff-v3.5.7.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/tiff-v3.5.7.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libtiff"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.remotesensing.org/pub/libtiff/tiff-v3.5.7.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/tiff-v3.5.7.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libtiff"
  gunzip $SIMAGELIBS_DOWNLOAD/tiff-v3.5.7.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libtiff; then
  echo "[SIMAGELIBS]         Extracting libtiff"
  mkdir $SIMAGELIBS_BUILD/libtiff
  mkdir $SIMAGELIBS_BUILD/libtiff/temp
  cd $SIMAGELIBS_BUILD/libtiff/temp
  tar xf $SIMAGELIBS_DOWNLOAD/tiff-v3.5.7.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT
    cp -R $SIMAGELIBS_BUILD/libtiff/temp/tiff-v3.5.7/libtiff/* $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT
    cp $SIMAGELIBS_BUILD/libtiff/temp/tiff-v3.5.7/VERSION $SIMAGELIBS_BUILD/libtiff
    cp $SIMAGELIBS_BUILD/libtiff/temp/tiff-v3.5.7/RELEASE-DATE $SIMAGELIBS_BUILD/libtiff
    cp $SIMAGELIBS_BUILD/libtiff/temp/tiff-v3.5.7/COPYRIGHT $SIMAGELIBS_BUILD/libtiff
    cp -R $SIMAGELIBS_BUILD/libtiff/temp/tiff-v3.5.7/dist $SIMAGELIBS_BUILD/libtiff
    mkdir $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/include
    mkdir $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/lib
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/libtiff/temp
  
  echo "[SIMAGELIBS]         Modifying libtiff configuration and make files"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cp $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/tiff.h $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/include
    cp $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/tiffio.h $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/include
    cp $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/tiffvers.h $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/include
    set_simage_param_crt
    cat $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/makefile.vc | sed -e "s/CFLAGS  = /CFLAGS  = -$SIMAGELIBS_PARAMCRT /g" > $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/makefile.vc_$SIMAGELIBS_CRT
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libtiff/md/lib/tiff.lib; then
  echo "[SIMAGELIBS]         Making libtiff"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT
    nmake -f makefile.vc_$SIMAGELIBS_CRT
    cp libtiff.lib lib/tiff.lib
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libtiff/md/lib/tiff.lib; then
  echo "[SIMAGELIBS]         Failed to make libtiff. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      libtiff verified OK"

############# ogg
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGELIBS]      Verifying libogg..."

if ! test -e $SIMAGELIBS_DOWNLOAD/libogg-1.0.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/libogg-1.0.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libogg"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD http://www.xiph.org/ogg/vorbis/download/libogg-1.0.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/libogg-1.0.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libogg"
  gunzip $SIMAGELIBS_DOWNLOAD/libogg-1.0.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libogg; then
  echo "[SIMAGELIBS]         Extracting libogg"
  mkdir $SIMAGELIBS_BUILD/libogg
  mkdir $SIMAGELIBS_BUILD/libogg/temp
  cd $SIMAGELIBS_BUILD/libogg/temp
  tar xf $SIMAGELIBS_DOWNLOAD/libogg-1.0.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/lib
    cp -R $SIMAGELIBS_BUILD/libogg/temp/libogg-1.0/* $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/libogg/temp
  
  echo "[SIMAGELIBS]         Modifying libogg configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGELIBS_BUILD/libogg/md/win32/ogg_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static \- Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libogg/md/win32/ogg_static_md.dsp
  cat $SIMAGELIBS_BUILD/libogg/mdd/win32/ogg_static.dsp | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg\.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libogg/mdd/win32/ogg_static_mdd.dsp

  cat $SIMAGELIBS_BUILD/libogg/mt/win32/ogg_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static - Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libogg/mt/win32/ogg_static_mt.dsp
  cat $SIMAGELIBS_BUILD/libogg/mtd/win32/ogg_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libogg/mtd/win32/ogg_static_mtd.dsp

fi

if ! test -e $SIMAGELIBS_BUILD/libogg/md/win32/ogg.lib; then
  echo "[SIMAGELIBS]         Making libogg"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/win32
    SIMAGELIBS_OLDINCLUDE=$INCLUDE
    SIMAGELIBS_TEMP1=`cygpath -w $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/include`
    INCLUDE=$INCLUDE\;$SIMAGELIBS_TEMP1
    cmd /C msdev ogg_static_$SIMAGELIBS_CRT.dsp /useenv /make "ogg_static - Win32 Simage" /rebuild
    INCLUDE=$SIMAGELIBS_OLDINCLUDE
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libogg/md/win32/ogg.lib; then
  echo "[SIMAGELIBS]         Failed to make libogg. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      ogg verified OK"

############# vorbis
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGELIBS]      Verifying libvorbis..."

if ! test -e $SIMAGELIBS_DOWNLOAD/libvorbis-1.0.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/libvorbis-1.0.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libvorbis"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD http://www.xiph.org/ogg/vorbis/download/libvorbis-1.0.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/libvorbis-1.0.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libvorbis"
  gunzip $SIMAGELIBS_DOWNLOAD/libvorbis-1.0.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libvorbis; then
  echo "[SIMAGELIBS]         Extracting libvorbis"
  mkdir $SIMAGELIBS_BUILD/libvorbis
  mkdir $SIMAGELIBS_BUILD/libvorbis/temp
  cd $SIMAGELIBS_BUILD/libvorbis/temp
  tar xf $SIMAGELIBS_DOWNLOAD/libvorbis-1.0.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/lib
    cp -R $SIMAGELIBS_BUILD/libvorbis/temp/libvorbis-1.0/* $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT
  done

  # rm -R $SIMAGELIBS_BUILD/libvorbis/temp
  
  echo "[SIMAGELIBS]         Modifying libvorbis configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbis_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbis.lib/g" | sed -e "s/vorbis_static \- Win32 Release/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbis_static_md.dsp
  cat $SIMAGELIBS_BUILD/libvorbis/mdd/win32/vorbis_static.dsp | sed -e "s/Vorbis_Static_Debug\\\\vorbis_static_d\.lib/vorbis\.lib/g" | sed -e "s/vorbis_static - Win32 Debug/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/mdd/win32/vorbis_static_mdd.dsp

  cat $SIMAGELIBS_BUILD/libvorbis/mt/win32/vorbis_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbis.lib/g" | sed -e "s/vorbis_static - Win32 Release/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/mt/win32/vorbis_static_mt.dsp
  cat $SIMAGELIBS_BUILD/libvorbis/mtd/win32/vorbis_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/Vorbis_Static_Debug\\\\vorbis_static_d\.lib/vorbis.lib/g" | sed -e "s/vorbis_static - Win32 Debug/vorbis_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/mtd/win32/vorbis_static_mtd.dsp

fi

if ! test -e $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbis.lib; then
  echo "[SIMAGELIBS]         Making libvorbis"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/win32
    SIMAGELIBS_OLDINCLUDE=$INCLUDE
    SIMAGELIBS_TEMP1=`cygpath -w $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/include`
    SIMAGELIBS_TEMP2=`cygpath -w $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/include`
    INCLUDE=$INCLUDE\;$SIMAGELIBS_TEMP1\;$SIMAGELIBS_TEMP2
    cmd /C echo %INCLUDE%
    cmd /C msdev vorbis_static_$SIMAGELIBS_CRT.dsp /useenv /make "vorbis_static - Win32 Simage" /rebuild
    INCLUDE=$SIMAGELIBS_OLDINCLUDE
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbis.lib; then
  echo "[SIMAGELIBS]         Failed to make libvorbis. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      libvorbis verified OK"

############# vorbisfile
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGELIBS]      Verifying libvorbisfile..."

# libvorbisfile is part of libvorbis

if ! test -e $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbisfile_static_mt.dsp; then
  echo "[SIMAGELIBS]         Modifying libvorbisfile configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbisfile_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbisfile.lib/g" | sed -e "s/vorbisfile_static \- Win32 Release/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbisfile_static_md.dsp
  cat $SIMAGELIBS_BUILD/libvorbis/mdd/win32/vorbisfile_static.dsp | sed -e "s/VorbisFile_Static_Debug\\\\vorbisfile_static_d\.lib/vorbisfile\.lib/g" | sed -e "s/vorbisfile_static - Win32 Debug/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/mdd/win32/vorbisfile_static_mdd.dsp

  cat $SIMAGELIBS_BUILD/libvorbis/mt/win32/vorbisfile_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:vorbisfile.lib/g" | sed -e "s/vorbisfile_static - Win32 Release/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/mt/win32/vorbisfile_static_mt.dsp
  cat $SIMAGELIBS_BUILD/libvorbis/mtd/win32/vorbisfile_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/VorbisFile_Static_Debug\\\\vorbisfile_static_d\.lib/vorbisfile.lib/g" | sed -e "s/vorbisfile_static - Win32 Debug/vorbisfile_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGELIBS_BUILD/libvorbis/mtd/win32/vorbisfile_static_mtd.dsp

fi

if ! test -e $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbisfile.lib; then
  echo "[SIMAGELIBS]         Making libvorbisfile"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/win32
    SIMAGELIBS_OLDINCLUDE=$INCLUDE
    SIMAGELIBS_TEMP1=`cygpath -w $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/include`
    SIMAGELIBS_TEMP2=`cygpath -w $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/include`
    INCLUDE=$INCLUDE\;$SIMAGELIBS_TEMP1\;$SIMAGELIBS_TEMP2
    cmd /C echo %INCLUDE%
    cmd /C msdev vorbisfile_static_$SIMAGELIBS_CRT.dsp /useenv /make "vorbisfile_static - Win32 Simage" /rebuild
    INCLUDE=$SIMAGELIBS_OLDINCLUDE
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libvorbis/md/win32/vorbisfile.lib; then
  echo "[SIMAGELIBS]         Failed to make libvorbisfile. Aborting."
  exit 1;
fi

echo "[SIMAGELIBS]      libvorbisfile verified OK"


########### oggvorbis = ogg + vorbis + vorbisfile

if ! test -d $SIMAGELIBS_BUILD/oggvorbis; then
  echo "[SIMAGELIBS]      Creating and populating oggvorbis directory structure"
  mkdir $SIMAGELIBS_BUILD/oggvorbis
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT
    mkdir $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/include
    mkdir $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/include/ogg
    mkdir $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/include/vorbis
    mkdir $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/lib
    cp -R $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/include/ogg/*.h $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/include/ogg
    cp -R $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/include/vorbis/*.h $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/include/vorbis
    cp -R $SIMAGELIBS_BUILD/libogg/$SIMAGELIBS_CRT/win32/ogg.lib $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/lib
    cp -R $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/win32/vorbis.lib $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/lib
    cp -R $SIMAGELIBS_BUILD/libvorbis/$SIMAGELIBS_CRT/win32/vorbisfile.lib $SIMAGELIBS_BUILD/oggvorbis/$SIMAGELIBS_CRT/lib
  done
fi

echo "[SIMAGELIBS]      oggvorbis verified OK"

############# libsndfile
# http://www.zip.com.au/~erikd/libsndfile/

echo "[SIMAGELIBS]      Verifying libsndfile..."

if ! test -e $SIMAGELIBS_DOWNLOAD/libsndfile-1.0.5.tar; then
  if ! test -e $SIMAGELIBS_DOWNLOAD/libsndfile-1.0.5.tar.gz; then
    echo "[SIMAGELIBS]         Downloading libsndfile"
    #wget --directory-prefix=$SIMAGELIBS_DOWNLOAD http://www.zip.com.au/~erikd/libsndfile/libsndfile-1.0.5.tar.gz
    wget --directory-prefix=$SIMAGELIBS_DOWNLOAD ftp://ftp.sim.no/pub/3rdparty/libsndfile-1.0.5.tar.gz
  fi
  echo "[SIMAGELIBS]         gunzip'ing libsndfile"
  gunzip $SIMAGELIBS_DOWNLOAD/libsndfile-1.0.5.tar.gz
fi

if ! test -d $SIMAGELIBS_BUILD/libsndfile; then
  echo "[SIMAGELIBS]         Extracting libsndfile"
  mkdir $SIMAGELIBS_BUILD/libsndfile
  mkdir $SIMAGELIBS_BUILD/libsndfile/temp
  cd $SIMAGELIBS_BUILD/libsndfile/temp
  tar xf $SIMAGELIBS_DOWNLOAD/libsndfile-1.0.5.tar

  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    mkdir $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT
    cp -R $SIMAGELIBS_BUILD/libsndfile/temp/libsndfile-1.0.5/* $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT
  done

  cd $SIMAGELIBS_BUILD

  rm -R $SIMAGELIBS_BUILD/libsndfile/temp
  
  echo "[SIMAGELIBS]         Modifying libsndfile configuration and make files"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cp $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT/Win32/sndfile.h $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT/src
    set_simage_param_crt
    SIMAGELIBS_TEMP1=`which cl | sed -e "s/\/bin\/cl$//g" | sed "s/ /\\\\\\ /g"`
    SIMAGELIBS_MSVCDIR=`cygpath -w "$SIMAGELIBS_TEMP1"`
    
    # Note 2003-05-21 thammer: The line below is to be used if
    # libsndfile is built as a dll.
    # cat $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT/Win32/Makefile.msvc | sed -e "s/MSVCDir=.*/MSVCDir=$SIMAGELIBS_MSVCDIR/g" | sed -e "s/\/MD/\/$SIMAGELIBS_PARAMCRT/g" > $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT/Win32/Makefile.msvc_$SIMAGELIBS_CRT
    
    cat $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT/Win32/Makefile.msvc | sed -e "s/MSVCDir=.*/MSVCDir=$SIMAGELIBS_MSVCDIR/g" | sed -e "s/\/MD/\/$SIMAGELIBS_PARAMCRT/g" | sed -e "s/DLL_LINK_FLAGS=\/nologo.*$/DLL_LINK_FLAGS=-lib \/nologo/" | sed -e "s/_USRDLL/_LIB/" > $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT/Win32/Makefile.msvc_$SIMAGELIBS_CRT
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libsndfile/md/libsndfile.lib; then
  echo "[SIMAGELIBS]         Making libsndfile"
  for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
    cd $SIMAGELIBS_BUILD/libsndfile/$SIMAGELIBS_CRT
    nmake -f Win32/Makefile.msvc_$SIMAGELIBS_CRT
  done
fi

if ! test -e $SIMAGELIBS_BUILD/libsndfile/md/libsndfile.lib; then
  echo "[SIMAGELIBS]         Failed to make libsndfile. Aborting."
  exit 1;
fi

# Note 2003-05-21 thammer: The line below is to be used if
# libsndfile is built as a dll. Libsndfile is under the LGPL.
# cp $SIMAGELIBS_BUILD/libsndfile/mt/libsndfile.dll $SIMAGELIBS_INSTALL/bin

echo "[SIMAGELIBS]      libsndfile verified OK"

echo "[SIMAGELIBS]   All SKDs verified OK"

echo "[SIMAGELIBS]   Making distribution"

if ! test -d $SIMAGELIBS_DIST; then
  echo "[SIMAGELIBS]      Creating directory for distribution - $SIMAGELIBS_DIST"
  mkdir $SIMAGELIBS_DIST
fi

# libungif

if ! test -d $SIMAGELIBS_DIST/libungif; then
  mkdir $SIMAGELIBS_DIST/libungif
fi

for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
  if ! test -d $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT; then
    mkdir $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT
  fi
  if ! test -d $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT/lib; then
    mkdir $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT/lib
  fi
  cp -R $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/lib/ungif.lib \
        $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT/lib
  if ! test -d $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT/include; then
    mkdir $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT/include
  fi
  cp -R $SIMAGELIBS_BUILD/libungif/$SIMAGELIBS_CRT/include/gif_lib.h \
        $SIMAGELIBS_DIST/libungif/$SIMAGELIBS_CRT/include
done

cp -R $SIMAGELIBS_BUILD/libungif/md/COPYING \
      $SIMAGELIBS_DIST/libungif/readme.txt

# zlib

if ! test -d $SIMAGELIBS_DIST/zlib; then
  mkdir $SIMAGELIBS_DIST/zlib
fi

for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
  if ! test -d $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT; then
    mkdir $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT
  fi
  if ! test -d $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT/lib; then
    mkdir $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT/lib
  fi
  cp -R $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/lib/zlib.lib \
        $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT/lib
  if ! test -d $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT/include; then
    mkdir $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT/include
  fi
  cp -R $SIMAGELIBS_BUILD/zlib/$SIMAGELIBS_CRT/include/*.h \
        $SIMAGELIBS_DIST/zlib/$SIMAGELIBS_CRT/include
done

cp -R $SIMAGELIBS_BUILD/zlib/md/README \
      $SIMAGELIBS_DIST/zlib/readme.txt

#libjpeg

if ! test -d $SIMAGELIBS_DIST/libjpeg; then
  mkdir $SIMAGELIBS_DIST/libjpeg
fi

for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
  if ! test -d $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT; then
    mkdir $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT
  fi
  if ! test -d $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT/lib; then
    mkdir $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT/lib
  fi
  cp -R $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/lib/jpeg.lib \
        $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT/lib
  if ! test -d $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT/include; then
    mkdir $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT/include
  fi
  cp -R $SIMAGELIBS_BUILD/libjpeg/$SIMAGELIBS_CRT/include/*.h \
        $SIMAGELIBS_DIST/libjpeg/$SIMAGELIBS_CRT/include
done

cp -R $SIMAGELIBS_BUILD/libjpeg/md/README \
      $SIMAGELIBS_DIST/libjpeg/readme.txt

# libtiff

if ! test -d $SIMAGELIBS_DIST/libtiff; then
  mkdir $SIMAGELIBS_DIST/libtiff
fi

for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
  if ! test -d $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT; then
    mkdir $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT
  fi
  if ! test -d $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT/lib; then
    mkdir $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT/lib
  fi
  cp -R $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/lib/tiff.lib \
        $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT/lib
  if ! test -d $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT/include; then
    mkdir $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT/include
  fi
  cp -R $SIMAGELIBS_BUILD/libtiff/$SIMAGELIBS_CRT/include/*.h \
        $SIMAGELIBS_DIST/libtiff/$SIMAGELIBS_CRT/include
done

cp -R $SIMAGELIBS_BUILD/libtiff/COPYRIGHT \
      $SIMAGELIBS_DIST/libtiff/readme.txt

# libpng

if ! test -d $SIMAGELIBS_DIST/libpng; then
  mkdir $SIMAGELIBS_DIST/libpng
fi

for SIMAGELIBS_CRT in $SIMAGELIBS_CRT_LIST; do
  if ! test -d $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT; then
    mkdir $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT
  fi
  if ! test -d $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT/lib; then
    mkdir $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT/lib
  fi
  cp -R $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/lib/png.lib \
        $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT/lib
  if ! test -d $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT/include; then
    mkdir $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT/include
  fi
  cp -R $SIMAGELIBS_BUILD/libpng/$SIMAGELIBS_CRT/include/*.h \
        $SIMAGELIBS_DIST/libpng/$SIMAGELIBS_CRT/include
done

cp -R $SIMAGELIBS_BUILD/libpng/md/LICENSE \
      $SIMAGELIBS_DIST/libpng/readme.txt

echo "[SIMAGELIBS]   Distribution OK"
