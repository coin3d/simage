## Process this file with automake to generate Makefile.in. 19990822 mortene.

lib_LTLIBRARIES = libsimage.la

INCLUDES = -I$(top_srcdir)/include/ -I$(top_builddir)/include/

## All source files used when building the libraries, including all
## headers.

if SIMAGE_JPEG_SUPPORT
JPEGSOURCES = simage_jpeg.c simage_jpeg.h
else
JPEGSOURCES =
endif

if SIMAGE_UNGIF_SUPPORT
UNGIFSOURCES = simage_gif.c simage_gif.h
else
UNGIFSOURCES =
endif

if SIMAGE_TIFF_SUPPORT
TIFFSOURCES = simage_tiff.c simage_tiff.h
else
TIFFSOURCES =
endif

if SIMAGE_PNG_SUPPORT
PNGSOURCES = simage_png.c simage_png.h
else
PNGSOURCES =
endif

if TGA_SUPPORT
TGASOURCES = simage_tga.c simage_tga.h
else
TGASOURCES =
endif

if RGB_SUPPORT
RGBSOURCES = simage_rgb.c simage_rgb.h
else
RGBSOURCES =
endif

if PIC_SUPPORT
PICSOURCES = simage_pic.c simage_pic.h
else
PICSOURCES =
endif

libsimage_la_SOURCES = \
	simage.c simage.h \
	$(JPEGSOURCES) $(UNGIFSOURCES) $(PNGSOURCES) $(TIFFSOURCES) \
	$(TGASOURCES) $(RGBSOURCES) $(PICSOURCES)

# all the conditionally compiled source files must be added here...
EXTRA_libsimage_la_SOURCES = \
	simage_jpeg.c simage_jpeg.h \
	simage_gif.c simage_gif.h \
	simage_tiff.c simage_tiff.h \
	simage_png.c simage_png.h \
	simage_tga.c simage_tga.h \
	simage_rgb.c simage_rgb.h \
	simage_pic.c simage_pic.h

## Installation information.

libsimageincdir = $(includedir)

libsimage_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libsimageinc_HEADERS = \
	$(top_builddir)/include/simage.h



#### Additional "native" Make rules #########################################


## For linking up a .DLL on MSWindows with MSVC++.
simage.dll: $(libsimage_la_OBJECTS) $(libsimage_la_DEPENDENCIES)
	@rm -f simage.dll && if test x"`$(CC) 2>/dev/null | egrep '^The Microsoft Visual C\+\+ wrapper'`" = x""; then rm -f simage.dll; touch simage.dll; else $(CC) /dll `echo $(libsimage_la_OBJECTS) | sed 's%\.lo%.obj%g'` $(libsimage_la_LIBADD) $(LIBS); fi


## This hook will be run at the end of a ``make install''.
##
## The single action taken is to install the simage.dll and simage.lib
## files (if they have been built properly).
install-exec-hook: simage.dll
	@if test -s simage.dll; then \
	  echo "Installing simage@SIMAGE_MAJOR_VERSION@.dll"; \
	  for i in dll lib; do \
	    $(INSTALL_DATA) simage.$$i \
	      $(DESTDIR)$(libdir)/simage@SIMAGE_MAJOR_VERSION@.$$i; \
	  done \
	fi
