# **************************************************************************

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

if SIMAGE_GDIPLUS_SUPPORT
GDIPLUSSOURCES = simage_gdiplus.cpp $(top_srcdir)/include/simage_gdiplus.h
else
GDIPLUSSOURCES =
endif

if SIMAGE_QIMAGE_SUPPORT
QIMAGESOURCES = simage_qimage.cpp $(top_srcdir)/include/simage_qimage.h
else
QIMAGESOURCES =
endif

if SIMAGE_CGIMAGE_SUPPORT
CGIMAGESOURCES = simage_cgimage.c $(top_srcdir)/include/simage_cgimage.h
else
CGIMAGESOURCES = 
endif

if SIMAGE_QUICKTIME_SUPPORT
QUICKTIMESOURCES = simage_quicktime.c $(top_srcdir)/include/simage_quicktime.h
GIFSOURCES = 
JPEGSOURCES = 
TIFFSOURCES = 
PNGSOURCES = 
TGASOURCES = 
else
QUICKTIMESOURCES =
GIFSOURCES = simage_gif.c $(top_srcdir)/include/simage_gif.h
JPEGSOURCES = simage_jpeg.c $(top_srcdir)/include/simage_jpeg.h
TIFFSOURCES = simage_tiff.c $(top_srcdir)/include/simage_tiff.h
PNGSOURCES = simage_png.c $(top_srcdir)/include/simage_png.h
TGASOURCES = simage_tga.c $(top_srcdir)/include/simage_tga.h
endif

JASPERSOURCES = simage_jasper.c $(top_srcdir)/include/simage_jasper.h
RGBSOURCES = simage_rgb.c $(top_srcdir)/include/simage_rgb.h
PICSOURCES = simage_pic.c $(top_srcdir)/include/simage_pic.h
XWDSOURCES = simage_xwd.c $(top_srcdir)/include/simage_xwd.h
EPSSOURCES = simage_eps.c $(top_srcdir)/include/simage_eps.h
AVISOURCES = simage_avi.c $(top_srcdir)/include/simage_avi.h \
             avi_encode.c $(top_srcdir)/include/avi_encode.h
OGGVORBISSOURCES = simage_oggvorbis_reader.c \
                   $(top_srcdir)/include/simage_oggvorbis.h
LIBSNDFILESOURCES = simage_libsndfile.c \
                   $(top_srcdir)/include/simage_libsndfile.h

SIMAGESOURCES = \
	$(top_builddir)/include/simage.h \
	simage.c \
	simage_write.c \
	resize.c \
	simage12.c \
	simage13.c \
	movie.c \
	stream.c \
	params.c \
	$(top_srcdir)/include/simage_private.h \
	$(GDIPLUSSOURCES) \
	$(JPEGSOURCES) \
	$(JASPERSOURCES) \
	$(GIFSOURCES) \
	$(PNGSOURCES) \
	$(TIFFSOURCES) \
	$(TGASOURCES) \
	$(RGBSOURCES) \
	$(PICSOURCES) \
	$(XWDSOURCES) \
	$(EPSSOURCES) \
	$(QIMAGESOURCES) \
	$(QUICKTIMESOURCES) \
	$(CGIMAGESOURCES) \
        $(AVISOURCES) \
        $(OGGVORBISSOURCES) \
        $(LIBSNDFILESOURCES)

if BUILD_WITH_MSVC
# we don't use libtool on Cygwin to generate lib
lib_LIBRARIES = simage@SIMAGE_MAJOR_VERSION@@SUFFIX@.lib
else
lib_LTLIBRARIES = libsimage@SUFFIX@.la
endif

# On Mac OS 10.2 or later, create two-level namespace binaries. Also
# pad the install_name to make it possible to change it (e.g. to
# include the library in an app bundle). 
if MACOSX_10_2FF
LibtoolLinkArgs = -no-undefined -Wl,-twolevel_namespace -Wl,-undefined,error -Wl,-headerpad_max_install_names 
else
LibtoolLinkArgs = -no-undefined 
endif

simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = \
	$(SIMAGESOURCES)

libsimage@SUFFIX@_la_SOURCES = \
	$(SIMAGESOURCES)

libsimage@SUFFIX@_la_LDFLAGS = \
  $(LibtoolLinkArgs) \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

if SIMAGE_MPEG2ENC_SUPPORT
if BUILD_WITH_MSVC
simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_LIBADD = ../mpeg2enc/mpeg2enc.lst
else
libsimage@SUFFIX@_la_LIBADD = ../mpeg2enc/libmpeg2enc.la
endif
endif

libsimage@SUFFIX@incdir = $(includedir)
libsimage@SUFFIX@inc_HEADERS = \
	$(top_builddir)/include/simage.h

# **************************************************************************
# for MS Windows (MSVC++) LIB/DLL linkage

# conditional to avoid a make warning when it's not in use
if BUILD_WITH_MSVC
simage@SIMAGE_MAJOR_VERSION@@SUFFIX@.lib: Makefile \
  $(simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
  $(simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_DEPENDENCIES)
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	rm -f "$$basename.dll" "$$basename.lib"; \
	echo "Linking $$basename.lib..."; \
	echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
	  $(simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_LIBADD) $(LIBS); \
	$(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
	  $(simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_LIBADD) $(LIBS);
endif

# **************************************************************************

install-exec-local:
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	for ext in dll pdb; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	    echo " $(INSTALL_PROGRAM)" "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	    $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	  fi \
	done;

install-data-local:
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	for ext in lib; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo " $(INSTALL_DATA)" "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	  fi \
	done;

uninstall-local:
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(bindir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done

# EOF **********************************************************************
