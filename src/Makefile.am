# **************************************************************************

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

if SIMAGE_QIMAGE_SUPPORT
QIMAGESOURCES = simage_qimage.cpp $(top_srcdir)/include/simage_qimage.h
else
QIMAGESOURCES =
endif

JPEGSOURCES = simage_jpeg.c $(top_srcdir)/include/simage_jpeg.h
UNGIFSOURCES = simage_gif.c $(top_srcdir)/include/simage_gif.h
TIFFSOURCES = simage_tiff.c $(top_srcdir)/include/simage_tiff.h
PNGSOURCES = simage_png.c $(top_srcdir)/include/simage_png.h
TGASOURCES = simage_tga.c $(top_srcdir)/include/simage_tga.h
RGBSOURCES = simage_rgb.c $(top_srcdir)/include/simage_rgb.h
PICSOURCES = simage_pic.c $(top_srcdir)/include/simage_pic.h
XWDSOURCES = simage_xwd.c $(top_srcdir)/include/simage_xwd.h
EPSSOURCES = simage_eps.c $(top_srcdir)/include/simage_eps.h

SIMAGESOURCES = \
	$(top_builddir)/include/simage.h \
	simage.c \
	simage_write.c \
	resize.c \
	simage12.c \
	movie.c \
	params.c \
	$(top_srcdir)/include/simage_private.h \
	$(JPEGSOURCES) \
	$(UNGIFSOURCES) \
	$(PNGSOURCES) \
	$(TIFFSOURCES) \
	$(TGASOURCES) \
	$(RGBSOURCES) \
	$(PICSOURCES) \
	$(XWDSOURCES) \
	$(EPSSOURCES) \
	$(QIMAGESOURCES)

if BUILD_WITH_MSVC
# we don't use libtool on Cygwin to generate lib
lib_LIBRARIES = simage@SIMAGE_MAJOR_VERSION@@SUFFIX@.lib
else
lib_LTLIBRARIES = libsimage@SUFFIX@.la
endif

simage@SIMAGE_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = \
	$(SIMAGESOURCES)

libsimage@SUFFIX@_la_SOURCES = \
	$(SIMAGESOURCES)

libsimage@SUFFIX@_la_LDFLAGS = \
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
  $(simage@SUFFIX@_lib_OBJECTS) $(simage@SUFFIX@_lib_DEPENDENCIES)
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
