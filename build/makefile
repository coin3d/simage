#****************************************************************
#
# Makefile for simage
#
# Usage:
#	make			Makes libsimage_debug.a
#       make release		Makes libsimage.a
#
#****************************************************************

simage: makefile.simage .simage

release: makefile.release .release

.simage:
	@mkdir -p objects
	@$(MAKE) -f makefile.simage

makefile.simage: simage.pro common.pro
	tmake -o makefile.simage -p common.pro simage.pro

.release:
	@mkdir -p releaseobjects
	@$(MAKE) -f makefile.release

makefile.release: release.pro common.pro
	tmake -o makefile.release -p common.pro release.pro

clean: makefile.simage makefile.release
	@$(MAKE) -f makefile.simage clean
	@$(MAKE) -f makefile.release clean
