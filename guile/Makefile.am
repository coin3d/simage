# **************************************************************************
# guile/Makefile.am

if HAVE_GUILE
guilesimagemoduledir = $(datadir)/guile/coin
dist_guilesimagemodule_DATA = \
	simage.scm

lib_LTLIBRARIES = libguilesimage.la

libguilesimage_la_SOURCES = \
	guilesimage.c \
	guilesimage.h

libguilesimage_la_LIBS = \
	-lguile

libguilesimageincdir = $(includedir)/
libguilesimageinc_HEADERS = \
	guilesimage.h

bin_SCRIPTS = \
	simage-convert.scm

EXTRA_DIST = \
	simage-convert.scm
endif

# EOF **********************************************************************
