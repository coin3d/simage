# **************************************************************************

if SIMAGE_MPEG2ENC_SUPPORT
MPEG2ENCDIR = mpeg2enc
else
MPEG2ENCDIR =
endif

SUBDIRS = $(MPEG2ENCDIR) src guile 

dist_bin_SCRIPTS = simage-config

m4datadir = $(datadir)/aclocal
dist_m4data_DATA = cfg/m4/simage.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = simage-@CONFIG@.cfg

noinst_SCRIPTS = \
	cfg/m4/msvccc

EXTRA_DIST = \
	simage.cfg.in \
	cfg/depcomp \
	cfg/ltconfig \
	cfg/m4/msvccc \
	build/simage.spec.in \
	README.WIN32

simage-@CONFIG@.cfg: config.status $(srcdir)/simage.cfg.in
	@$(SHELL) config.status --file=$@:simage.cfg.in

