# **************************************************************************

SUBDIRS = src

bin_SCRIPTS = simage-config

EXTRA_DIST = $(m4data_DATA)

m4datadir = $(datadir)/aclocal
m4data_DATA = cfg/m4/simage.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = simage-@CONFIG@.cfg

simage-@CONFIG@.cfg: $(top_srcdir)/simage.cfg.in
	@./config.status --file=$@:simage.cfg.in

