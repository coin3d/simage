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

DLLNAME = simage@SIMAGE_MAJOR_VERSION@@DSUFFIX@

lib_LTLIBRARIES = libsimage.la

libsimage_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libsimage_la_SOURCES = \
	simage.c \
	simage.h \
	$(JPEGSOURCES) \
	$(UNGIFSOURCES) \
	$(PNGSOURCES) \
	$(TIFFSOURCES) \
	$(TGASOURCES) \
	$(RGBSOURCES) \
	$(PICSOURCES)

# all the conditionally compiled source files must be added here...
EXTRA_libsimage_la_SOURCES = \
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

libsimageincdir = $(includedir)
libsimageinc_HEADERS = \
	$(top_builddir)/include/simage.h

# **************************************************************************
# Additional Make rules not supported by Automake

# Rule for linking up a .DLL on MSWindows with MSVC++.
$(DLLNAME).dll: $(libsimage_la_OBJECTS) $(libsimage_la_DEPENDENCIES)
	@rm -f $(DLLNAME).dll; \
	if @BUILD_WITH_MSVC@; then \
	  $(CC) $(LDFLAGS) @DLLFLAGS@ /out:$(DLLNAME).dll \
	    `echo $(libsimage_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	    $(libsimage_la_LIBADD) $(LIBS); \
	else \
	  rm -f $(DLLNAME).dll; \
	  touch $(DLLNAME).dll; \
	fi

## This hook will be run at the end of a ``make install''.
install-exec-hook: $(DLLNAME).dll
	@if test -s $(DLLNAME).dll; then \
	  $(INSTALL_DATA) $(DLLNAME).dll $(DESTDIR)$(bindir)/$(DLLNAME).dll; \
	  $(INSTALL_DATA) $(DLLNAME).lib $(DESTDIR)$(libdir)/$(DLLNAME).lib; \
	  test -s $(DLLNAME).pdb && \
	    $(INSTALL_DATA) $(DLLNAME).pdb $(DESTDIR)$(libdir)/$(DLLNAME).pdb; \
	fi

## should have been uninstall-hook, but that doesn't work
uninstall: uninstall-am
	@file="$(DESTDIR)$(bindir)"/$(DLLNAME).dll; \
	test -f $$file && rm -f $$file && echo rm -f $$file; \
	for ext in lib pdb; do \
	  file="$(DESTDIR)$(libdir)"/$(DLLNAME).$$ext; \
	  test -f $$file && rm -f $$file && echo rm -f $$file; \
	done

# EOF **********************************************************************
