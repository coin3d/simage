# **************************************************************************

SUBDIRS = src

bin_SCRIPTS = simage-config
BUILT_SOURCES = simage.cfg

EXTRA_DIST = $(m4data_DATA)

m4datadir = $(datadir)/aclocal
m4data_DATA = cfg/m4/simage.m4

install-exec-hook: simage.cfg
	@target=simage.cfg; \
	@SIMAGE_DEBUG_BUILD@ && target=simage-debug.cfg; \
	echo "Installing $$target"; \
	$(mkinstalldirs) $(DESTDIR)$(datadir)/Coin/conf && \
	$(INSTALL_DATA) simage.cfg $(DESTDIR)$(datadir)/Coin/conf/$$target;

