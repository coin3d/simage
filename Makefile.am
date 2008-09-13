# **************************************************************************

if SIMAGE_MPEG2ENC_SUPPORT
MPEG2ENCDIR = mpeg2enc
else
MPEG2ENCDIR =
endif

SUBDIRS = $(MPEG2ENCDIR) src guile 

dist_bin_SCRIPTS = simage-config

m4datadir = $(datadir)/aclocal
dist_m4data_DATA = cfg/simage.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = simage-@CONFIG@.cfg

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA= simage.pc

EXTRA_DIST = \
        simage.cfg.in \
        simage.pc.in \
        cfg/depcomp \
        cfg/ltconfig \
        cfg/wrapmsvc.exe \
        cfg/gendsp.sh \
        cfg/errors.txt \
        build/simage.spec.in \
        build/README \
        build/msvc6/config-debug.h \
        build/msvc6/simage1_install.dsp \
        build/msvc6/simage1.dsw \
        build/msvc6/include/simage.h \
        build/msvc6/simage1_uninstall.dsp \
        build/msvc6/config-release.h \
        build/msvc6/config.h \
        build/msvc6/simage1.dsp \
        build/msvc7/config-debug.h \
        build/msvc7/simage1.sln \
        build/msvc7/simage1_uninstall.vcproj \
        build/msvc7/include/simage.h \
        build/msvc7/simage1.vcproj \
        build/msvc7/config-release.h \
        build/msvc7/config.h \
        build/msvc7/simage1_install.vcproj \
        build/msvc8/config-debug.h \
        build/msvc8/simage1.sln \
        build/msvc8/simage1_uninstall.vcproj \
        build/msvc8/include/simage.h \
        build/msvc8/simage1.vcproj \
        build/msvc8/config-release.h \
        build/msvc8/config.h \
        build/msvc8/simage1_install.vcproj \
        build/msvc9/config-debug.h \
        build/msvc9/simage1.sln \
        build/msvc9/simage1_uninstall.vcproj \
        build/msvc9/include/simage.h \
        build/msvc9/simage1.vcproj \
        build/msvc9/config-release.h \
        build/msvc9/config.h \
        build/msvc9/simage1_install.vcproj \
        build/misc/create-directories.bat \
        build/misc/install-headers.bat \
        build/misc/uninstall-headers.bat \
        build/misc/README.GDI+ \
        build/misc/install-sdk.bat \
        build/misc/uninstall-sdk.bat \
        docs/announcement-1_6_0.txt \
        docs/announcement-1_6_1.txt \
        src/simage_jpeg_reader.icc \
        src/simage_jpeg_writer.icc \
        README.WIN32

DISTCLEANFILES = \
	libtool \
	simage-@CONFIG@.cfg

simage-@CONFIG@.cfg: config.status $(srcdir)/simage.cfg.in
	@$(SHELL) config.status --file=$@:simage.cfg.in

install-exec-local: $(srcdir)/cfg/wrapmsvc.exe
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@if @BUILD_WITH_MSVC@; then \
	  p=wrapmsvc.exe; \
	  echo " $(INSTALL_PROGRAM) $(srcdir)/cfg/$$p $(DESTDIR)$(bindir)/$$p"; \
	  $(INSTALL_PROGRAM) $(srcdir)/cfg/$$p $(DESTDIR)$(bindir)/$$p; \
	fi

# **************************************************************************

simage1_$(VERSION):
	$(MAKE) VERSION=dist distdir
	mv simage-dist simage1_$(VERSION)

simage1-dev_$(VERSION): simage1_$(VERSION)
	cp -p -R simage1_$(VERSION) simage1-dev_$(VERSION)

debian-setup: simage1_$(VERSION) simage1-dev_$(VERSION)
	debdir=simage1_$(VERSION)/debian; \
	conf=./config.status; \
	test -d $$debdir || mkdir $$debdir; \
	for f in packaging/debian/*.in; do \
	  file=`echo $$f | sed 's/.*\///g; s/\.in$$//g'`; \
	  $$conf --file=$$debdir/$$file:$$f; \
	done; \
	for f in rules preinst prerm postinst postrm; do \
	  if test -f $$debdir/$$f; then \
	    echo " chmod +x $$debdir/$$f"; \
	    chmod +x $$debdir/$$f; \
	  fi; \
	done;
	debdir=simage1-dev_$(VERSION)/debian; \
	conf=./config.status; \
	test -d $$debdir || mkdir $$debdir; \
	for f in packaging/debian/*.in; do \
	  file=`echo $$f | sed 's/.*\///g; s/\.in$$//g'`; \
	  $$conf --file=$$debdir/$$file:$$f; \
	done; \
	for f in rules preinst prerm postinst postrm; do \
	  if test -f $$debdir/$$f; then \
	    echo " chmod +x $$debdir/$$f"; \
	    chmod +x $$debdir/$$f; \
	  fi; \
	done;
#	perl -pi -e 's/^simage$(SIMAGE_MAJOR_VERSION)\b/simage$(SIMAGE_MAJOR_VERSION)-dev/g;' simage1-dev_$(VERSION)/debian/changelog
#	echo "pkgversion=$(VERSION)" >$$debdir/substvars

debian-packages: debian-setup
	cd simage1_$(VERSION); \
	dpkg-buildpackage -S -uc -us;
	cd simage1_$(VERSION); \
	dpkg-buildpackage -b -uc;
	cd simage1-dev_$(VERSION); \
	dpkg-buildpackage -b -uc;

# **************************************************************************
# misc rules for automatic Microsoft Windows packaging.
# main: `windows-packages'

windows-files:
	@if test -d windows; then :; else \
	  echo " mkdir windows"; \
	  mkdir windows; \
	fi
	@for file in $(top_srcdir)/packaging/windows/*.in; do \
	  basename=`echo $$file | sed 's%^.*/%%g; s%\.in$$%%'`; \
	  echo " config.status: generating windows/$$basename"; \
	  ./config.status --file=-:- >"windows/$$basename" <"$$file"; \
	done

windows-packages: windows-files
	@( cd windows; $(MAKE) VERSION=$(VERSION) )

# EOF ######################################################################
