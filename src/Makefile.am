# **************************************************************************

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

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

if SIMAGE_TGA_SUPPORT
TGASOURCES = simage_tga.c simage_tga.h
else
TGASOURCES =
endif

if SIMAGE_RGB_SUPPORT
RGBSOURCES = simage_rgb.c simage_rgb.h
else
RGBSOURCES =
endif

if SIMAGE_PIC_SUPPORT
PICSOURCES = simage_pic.c simage_pic.h
else
PICSOURCES =
endif

lib_LTLIBRARIES = libsimage@SUFFIX@.la

libsimage@SUFFIX@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libsimage@SUFFIX@_la_SOURCES = \
	simage.c \
	simage_write.c \
	simage.h \
	resize.c \
	$(JPEGSOURCES) \
	$(UNGIFSOURCES) \
	$(PNGSOURCES) \
	$(TIFFSOURCES) \
	$(TGASOURCES) \
	$(RGBSOURCES) \
	$(PICSOURCES)

# all the conditionally compiled source files must be added here...
EXTRA_libsimage@SUFFIX@_la_SOURCES = \
	simage_jpeg.h \
	simage_jpeg.c \
	simage_gif.h \
	simage_gif.c \
	simage_tiff.h \
	simage_tiff.c \
	simage_png.h \
	simage_png.c \
	simage_tga.h \
	simage_tga.c \
	simage_rgb.h \
	simage_rgb.c \
	simage_pic.h \
	simage_pic.c

libsimage@SUFFIX@incdir = $(includedir)
libsimage@SUFFIX@inc_HEADERS = \
	$(top_builddir)/include/simage.h

# **************************************************************************

simage@SIMAGE_MAJOR_VERSION@@SUFFIX@.lib: Makefile \
  $(libsimage@SUFFIX@_la_OBJECTS) $(libsimage@SUFFIX@_la_DEPENDENCIES)
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	rm -f "$$basename.dll" "$$basename.lib"; \
	if @BUILD_WITH_MSVC@; then \
	  echo "Linking $$basename.lib..."; \
	  echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	    `echo $(libsimage@SUFFIX@_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	    $(libsimage@SUFFIX@_la_LIBADD) $(LIBS); \
	  $(CC) $(LDFLAGS) $(LIBFLAGS) \
	    `echo $(libsimage@SUFFIX@_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	    $(libsimage@SUFFIX@_la_LIBADD) $(LIBS); \
	else \
	  touch "$$basename.dll" "$$basename.lib"; \
	fi

all-local: simage@SIMAGE_MAJOR_VERSION@@SUFFIX@.lib

# **************************************************************************

install-exec-local:
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	if test -s "$$basename.dll"; then \
	  $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	  echo " $(INSTALL_PROGRAM)" "$$basename.dll" "$(DESTDIR)$(bindir)/$$basename.dll"; \
	  $(INSTALL_PROGRAM) "$$basename.dll" "$(DESTDIR)$(bindir)/$$basename.dll"; \
	fi

install-data-local:
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	for ext in lib pdb; do \
	  file="$$basename.$$ext"; \
	  if test -s "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo " $(INSTALL_DATA)" "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	  fi \
	done

uninstall-local:
	@basename="simage@SIMAGE_MAJOR_VERSION@@SUFFIX@"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(libdir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done

# EOF **********************************************************************
