# **************************************************************************

SUBDIRS = src

bin_SCRIPTS = simage-config

m4datadir = $(datadir)/aclocal
m4data_DATA = cfg/m4/simage.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = simage-@CONFIG@.cfg

simage-@CONFIG@.cfg: $(srcdir)/simage.cfg.in
	@$(SHELL) config.status --file=$@:simage.cfg.in

