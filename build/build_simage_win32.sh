#!/bin/bash

# usage: build_simage_win32.sh <u|nu> <r|nr>
#  u (default) is for doing a cvs update of simage
# nu is for no update
#  r (default) is for rebuilding (rechecking) simage
# nr is for no rechecking

# This script will download, (cvs) checkout and build simage
# from scratch, including all dependent libraries.

# Before running this script, please
# - install cygwin, including wget
# - make sure you've done cvs login to cvs.coin3d.org
# - set the environment variables described below

# Beginning of environment variables the user should set

# SIMAGE_SDK is the name of the directory where you want this script to
# install all sdks (image and audio libs)

export SIMAGE_SDK=/cygdrive/c/temp/simage/sdk

# SIMAGE_* are the directories where you want this script to checkout,
# configure and install simage

export SIMAGE_CHECKOUT=/cygdrive/c/temp/simage/checkout
export SIMAGE_CONFIGURE=/cygdrive/c/temp/simage/configure
export SIMAGE_INSTALL=/cygdrive/c/temp/simage/install

# end of the environment variables the user should set

# Verify SDKs

echo "[SIMAGE]   Verifying SDKs..."

if ! test -d $SIMAGE_SDK; then
  echo "[SIMAGE]      Creating directory for SDKs - $SIMAGE_SDK"
  mkdir $SIMAGE_SDK
fi

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

  for SIMAGE_PREFIX in md mdd mt mtd; do
    mkdir $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX
    cp $SIMAGE_SDK/libjpeg/temp/jpeg-6b/* $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX
    mkdir $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/include
    mkdir $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/lib
  done

  rm -R $SIMAGE_SDK/libjpeg/temp
  
  echo "[SIMAGE]         Modifying libjpeg configuration and make files"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cp $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/jconfig.vc $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/jconfig.h
    cp $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/*.h $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/include
    export SIMAGE_UPPERPREFIX=`echo $SIMAGE_PREFIX | tr [:lower:] [:upper:]` 
    cat $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/makefile.vc | sed -e "s/CFLAGS= /CFLAGS= -$SIMAGE_UPPERPREFIX /g" | sed -e 's/RM= del/RM= rm -f/g' > $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX/makefile.vc_$SIMAGE_PREFIX
  done
fi

if ! test -e $SIMAGE_SDK/libjpeg/mt/lib/jpeg.lib; then
  echo "[SIMAGE]         Making libjpeg"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cd $SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX
    nmake -f makefile.vc_$SIMAGE_PREFIX
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

  for SIMAGE_PREFIX in md mdd mt mtd; do
    mkdir $SIMAGE_SDK/zlib/$SIMAGE_PREFIX
    cp -R $SIMAGE_SDK/zlib/temp/zlib-1.1.4/* $SIMAGE_SDK/zlib/$SIMAGE_PREFIX
    mkdir $SIMAGE_SDK/zlib/$SIMAGE_PREFIX/include
    mkdir $SIMAGE_SDK/zlib/$SIMAGE_PREFIX/lib
  done

  rm -R $SIMAGE_SDK/zlib/temp
  
  echo "[SIMAGE]         Modifying zlib configuration and make files"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cp $SIMAGE_SDK/zlib/$SIMAGE_PREFIX/*.h $SIMAGE_SDK/zlib/$SIMAGE_PREFIX/include
    export SIMAGE_UPPERPREFIX=`echo $SIMAGE_PREFIX | tr [:lower:] [:upper:]` 
    cat $SIMAGE_SDK/zlib/$SIMAGE_PREFIX/msdos/Makefile.w32 | sed -e "s/CFLAGS=/CFLAGS= -$SIMAGE_UPPERPREFIX /g" > $SIMAGE_SDK/zlib/$SIMAGE_PREFIX/makefile.vc_$SIMAGE_PREFIX
  done
fi

if ! test -e $SIMAGE_SDK/zlib/mt/lib/zlib.lib; then
  echo "[SIMAGE]         Making zlib"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cd $SIMAGE_SDK/zlib/$SIMAGE_PREFIX
    nmake -f makefile.vc_$SIMAGE_PREFIX
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

  for SIMAGE_PREFIX in md mdd mt mtd; do
    mkdir $SIMAGE_SDK/libpng/$SIMAGE_PREFIX
    cp -R $SIMAGE_SDK/libpng/temp/libpng-1.2.5/* $SIMAGE_SDK/libpng/$SIMAGE_PREFIX
    mkdir $SIMAGE_SDK/libpng/$SIMAGE_PREFIX/include
    mkdir $SIMAGE_SDK/libpng/$SIMAGE_PREFIX/lib
  done

  rm -R $SIMAGE_SDK/libpng/temp
  
  echo "[SIMAGE]         Modifying libpng configuration and make files"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cp $SIMAGE_SDK/libpng/$SIMAGE_PREFIX/*.h $SIMAGE_SDK/libpng/$SIMAGE_PREFIX/include
    export SIMAGE_UPPERPREFIX=`echo $SIMAGE_PREFIX | tr [:lower:] [:upper:]` 
    cat $SIMAGE_SDK/libpng/$SIMAGE_PREFIX/scripts/makefile.vcwin32 | sed -e "s/CFLAGS= /CFLAGS= -$SIMAGE_UPPERPREFIX /g" | sed -e "s/-I\\.\\.\\\\zlib/-I\\.\\.\\\\\\.\\.\\\\zlib\\\\$SIMAGE_PREFIX/g" > $SIMAGE_SDK/libpng/$SIMAGE_PREFIX/makefile.vc_$SIMAGE_PREFIX
  done
fi

if ! test -e $SIMAGE_SDK/libpng/mt/lib/png.lib; then
  echo "[SIMAGE]         Making libpng"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cd $SIMAGE_SDK/libpng/$SIMAGE_PREFIX
    nmake -f makefile.vc_$SIMAGE_PREFIX
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

  for SIMAGE_PREFIX in md mdd mt mtd; do
    mkdir $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX
    cp -R $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7./libtiff/* $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX
    cp $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7/VERSION $SIMAGE_SDK/libtiff
    cp $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7/RELEASE-DATE $SIMAGE_SDK/libtiff
    cp -R $SIMAGE_SDK/libtiff/temp/tiff-v3.5.7/dist $SIMAGE_SDK/libtiff
    mkdir $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX/include
    mkdir $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX/lib
  done

  rm -R $SIMAGE_SDK/libtiff/temp
  
  echo "[SIMAGE]         Modifying libtiff configuration and make files"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cp $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX/*.h $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX/include
    export SIMAGE_UPPERPREFIX=`echo $SIMAGE_PREFIX | tr [:lower:] [:upper:]` 
    cat $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX/makefile.vc | sed -e "s/CFLAGS  = /CFLAGS  = -$SIMAGE_UPPERPREFIX /g" > $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX/makefile.vc_$SIMAGE_PREFIX
  done
fi

if ! test -e $SIMAGE_SDK/libtiff/mt/lib/tiff.lib; then
  echo "[SIMAGE]         Making libtiff"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cd $SIMAGE_SDK/libtiff/$SIMAGE_PREFIX
    nmake -f makefile.vc_$SIMAGE_PREFIX
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

  for SIMAGE_PREFIX in md mdd mt mtd; do
    mkdir $SIMAGE_SDK/libogg/$SIMAGE_PREFIX
    mkdir $SIMAGE_SDK/libogg/$SIMAGE_PREFIX/lib
    cp -R $SIMAGE_SDK/libogg/temp/libogg-1.0/* $SIMAGE_SDK/libogg/$SIMAGE_PREFIX
  done

  rm -R $SIMAGE_SDK/libogg/temp
  
  echo "[SIMAGE]         Modifying libogg configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGE_SDK/libogg/md/win32/ogg_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static \- Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/md/win32/ogg_static_md.dsp
  cat $SIMAGE_SDK/libogg/mdd/win32/ogg_static.dsp | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg\.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mdd/win32/ogg_static_mdd.dsp

  cat $SIMAGE_SDK/libogg/mt/win32/ogg_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static - Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mt/win32/ogg_static_mt.dsp
  cat $SIMAGE_SDK/libogg/mtd/win32/ogg_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mtd/win32/ogg_static_mtd.dsp

fi

if ! test -e $SIMAGE_SDK/libogg/mt/lib/ogg.lib; then
  echo "[SIMAGE]         Making libogg"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cd $SIMAGE_SDK/libogg/$SIMAGE_PREFIX/win32
    SIMAGE_OLDINCLUDE=$INCLUDE
    export INCLUDE=$INCLUDE\;$SIMAGE_SDK/libogg/$SIMAGE_PREFIX/include
    cmd /C msdev ogg_static_$SIMAGE_PREFIX.dsp /useenv /make "ogg_static - Win32 Simage" /rebuild
    cp ogg.lib $SIMAGE_SDK/libogg/$SIMAGE_PREFIX/lib
    INCLUDE=$SIMAGE_OLDINCLUDE
  done
fi

if ! test -e $SIMAGE_SDK/libogg/mt/lib/ogg.lib; then
  echo "[SIMAGE]         Failed to make libogg. Aborting."
  exit;
fi

echo "[SIMAGE]      ogg verified OK"

############# vorbis
# http://www.xiph.org/ogg/vorbis/

echo "[SIMAGE]      Verifying libvorbis..."

*****

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

  for SIMAGE_PREFIX in md mdd mt mtd; do
    mkdir $SIMAGE_SDK/libogg/$SIMAGE_PREFIX
    mkdir $SIMAGE_SDK/libogg/$SIMAGE_PREFIX/lib
    cp -R $SIMAGE_SDK/libogg/temp/libogg-1.0/* $SIMAGE_SDK/libogg/$SIMAGE_PREFIX
  done

  rm -R $SIMAGE_SDK/libogg/temp
  
  echo "[SIMAGE]         Modifying libogg configuration and make files"
  # unix2dos conversion (\n -> \r\n)
  echo -e "s/\$/\r/;\np;" >unix2dos.sed

  cat $SIMAGE_SDK/libogg/md/win32/ogg_static.dsp | sed -e "s/\/MT/\/MD/g" | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static \- Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/md/win32/ogg_static_md.dsp
  cat $SIMAGE_SDK/libogg/mdd/win32/ogg_static.dsp | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg\.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mdd/win32/ogg_static_mdd.dsp

  cat $SIMAGE_SDK/libogg/mt/win32/ogg_static.dsp | sed -e "s/ADD LIB32 \/nologo\$/ADD LIB32 \/nologo \/out:ogg.lib/g" | sed -e "s/ogg_static - Win32 Release/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mt/win32/ogg_static_mt.dsp
  cat $SIMAGE_SDK/libogg/mtd/win32/ogg_static.dsp | sed -e "s/\/MDd/MTD/g" | sed -e "s/Static_Debug\\\\ogg_static_d\.lib/ogg.lib/g" | sed -e "s/ogg_static - Win32 Debug/ogg_static \- Win32 Simage/g" | sed -n -f unix2dos.sed > $SIMAGE_SDK/libogg/mtd/win32/ogg_static_mtd.dsp

fi

if ! test -e $SIMAGE_SDK/libogg/mt/lib/ogg.lib; then
  echo "[SIMAGE]         Making libogg"
  for SIMAGE_PREFIX in md mdd mt mtd; do
    cd $SIMAGE_SDK/libogg/$SIMAGE_PREFIX/win32
    SIMAGE_OLDINCLUDE=$INCLUDE
    export INCLUDE=$INCLUDE\;$SIMAGE_SDK/libogg/$SIMAGE_PREFIX/include
    cmd /C msdev ogg_static_$SIMAGE_PREFIX.dsp /useenv /make "ogg_static - Win32 Simage" /rebuild
    cp ogg.lib $SIMAGE_SDK/libogg/$SIMAGE_PREFIX/lib
    INCLUDE=$SIMAGE_OLDINCLUDE
  done
fi

if ! test -e $SIMAGE_SDK/libogg/mt/lib/ogg.lib; then
  echo "[SIMAGE]         Failed to make libogg. Aborting."
  exit;
fi

echo "[SIMAGE]      ogg verified OK"


# cvs update simage

if ! test -d $SIMAGE_CHECKOUT; then
  echo "[SIMAGE]      Creating directory $SIMAGE_CHECKOUT"
  mkdir $SIMAGE_CHECKOUT
fi

if ! test -d $SIMAGE_CHECKOUT; then
  echo "[SIMAGE]      Couldn\'t create directory $SIMAGE_CHECKOUT. Aborting."
  exit
fi

if ! test -d $SIMAGE_CONFIGURE; then
  echo "[SIMAGE]      Creating directory $SIMAGE_CONFIGURE"
  mkdir $SIMAGE_CONFIGURE
fi

if ! test -d $SIMAGE_CONFIGURE; then
  echo "[SIMAGE]      Couldn\'t create directory $SIMAGE_CONFIGURE. Aborting."
  exit
fi

if ! test -d $SIMAGE_INSTALL; then
  echo "[SIMAGE]      Creating directory $SIMAGE_INSTALL"
  mkdir $SIMAGE_INSTALL
fi

if ! test -d $SIMAGE_INSTALL; then
  echo "[SIMAGE]      Couldn\'t create directory $SIMAGE_INSTALL. Aborting."
  exit
fi

if ! test "X$1" = "Xnu"; then

  if ! test -d $SIMAGE_CHECKOUT/simage; then
    echo "[SIMAGE]      Checking out simage from scratch"
    cd $SIMAGE_CHECKOUT
    cvs -z3 -d:pserver:cvs@cvs.coin3d.org:/export/cvsroot co simage
  else
    echo "[SIMAGE]      Updating simage"
    cd $SIMAGE_CHECKOUT/simage
    cvs -z3 update -dP
  fi

fi # update simage

# configure and make simage

if ! test "X$2" = "Xnr"; then

echo "[SIMAGE]      Configuring simage"

for SIMAGE_PREFIX in md mdd mt mtd; do

  if ! test -d $SIMAGE_CONFIGURE/simage; then
    echo "[SIMAGE]         Creating directory $SIMAGE_CONFIGURE/simage"
    mkdir $SIMAGE_CONFIGURE/simage
  fi

  if ! test -d $SIMAGE_CONFIGURE/simage/$SIMAGE_PREFIX; then
    echo "[SIMAGE]         Creating directory $SIMAGE_CONFIGURE/simage/$SIMAGE_PREFIX"
    mkdir $SIMAGE_CONFIGURE/simage/$SIMAGE_PREFIX
  fi

  cd $SIMAGE_CONFIGURE/simage/$SIMAGE_PREFIX

  if test -e "config.status"; then
    echo "[SIMAGE]         Rechecking existing configuration in simage/$SIMAGE_PREFIX"
    ./config.status --recheck && ./config.status
  else
    echo "[SIMAGE]         Configuring from scratch in simage/$SIMAGE_PREFIX"

    SIMAGE_EXTRAFLAGS=""
    if test "X$SIMAGE_PREFIX" = "Xmd"; then
      SIMAGE_EXTRAFLAGS="--disable-debug --disable-symbols"
    elif test "X$SIMAGE_PREFIX" = "Xmt"; then
      SIMAGE_EXTRAFLAGS="--disable-debug --disable-symbols"
    fi

    $SIMAGE_CHECKOUT/simage/configure --with-jpeg=$SIMAGE_SDK/libjpeg/$SIMAGE_PREFIX --with-zlib=$SIMAGE_SDK/zlib/$SIMAGE_PREFIX --with-png=$SIMAGE_SDK/libpng/$SIMAGE_PREFIX --with-tiff=$SIMAGE_SDK/libtiff/$SIMAGE_PREFIX --with-pic --with-rgb --with-targa --with-avienc --enable-dependency-tracking --with-msvcrt=$SIMAGE_PREFIX --with-suffix=_$SIMAGE_PREFIX --with-alternate=$SIMAGE_PREFIX --prefix=$SIMAGE_INSTALL --enable-static $SIMAGE_EXTRAFLAGS

  fi

  echo "[SIMAGE]         Making install in simage/$SIMAGE_PREFIX"
  make install

done

fi # make simage

echo "[SIMAGE]   All systems ready to go"


