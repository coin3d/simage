## Process this file with automake to generate Makefile.in. 19990822 mortene.

lib_LTLIBRARIES = libsimage.la

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building the libraries, including all
## headers.

if JPEG_SUPPORT
JPEGSOURCES = simage_jpeg.c simage_jpeg.h
else
JPEGSOURCES =
endif

if TIFF_SUPPORT
TIFFSOURCES = simage_tiff.c simage_tiff.h
else
TIFFSOURCES =
endif

if PNG_SUPPORT
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
	$(JPEGSOURCES) $(PNGSOURCES) $(TIFFSOURCES) \
	$(TGASOURCES) $(RGBSOURCES) $(PICSOURCES)


## Installation information.

libsimageincdir = $(includedir)

libsimage_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-release $(LT_RELEASE)

libsimageinc_HEADERS = \
	$(top_srcdir)/include/simage.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990913 mortene.

install-libsimageincHEADERS: $(libsimageinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libsimageincdir)
	@list='$(libsimageinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsimageincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsimageincdir)/$$f; \
	done

uninstall-libsimageincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libsimageinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libsimageincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libsimageincdir)/$$f; \
	done
