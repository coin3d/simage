# **************************************************************************

if BUILD_WITH_MSVC
noinst_LIBRARIES = mpeg2enc.lst
else
noinst_LTLIBRARIES = libmpeg2enc.la
endif

# Remove any linklibraries detected by configure, as they are not
# needed in the submodules -- only in the toplevel libsimage build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

MPEG2ENCSOURCES = \
	api.c api.h \
	conform.c \
	fdctref.c \
	global.h \
	idct.c \
	motion.c \
	mpeg2enc.h \
	predict.c \
	putbits.c \
	puthdr.c \
	putmpg.c \
	putpic.c \
	putseq.c putseq.h \
	putvlc.c \
	quantize.c \
	ratectl.c \
	readpic.c \
	stats.c \
	transfrm.c \
	vlc.h \
	writepic.c

libmpeg2enc_la_SOURCES = \
	$(MPEG2ENCSOURCES)

# conditional to avoid a make warning when it's not in use
if BUILD_WITH_MSVC
mpeg2enc_lst_SOURCES = \
	$(MPEG2ENCSOURCES)

mpeg2enc.lst: Makefile $(mpeg2enc_lst_OBJECTS)
	@echo "Linking mpeg2enc.lst..."; \
	rm -f $@; \
	for i in $(mpeg2enc_lst_OBJECTS); do echo $$i >>$@; done
endif

# EOF **********************************************************************
