# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_INCLUDED
MAKEFILE_DIST_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/coreutils/install.mk
include $(MAKEFILEDIR)/configure/directory_variables/build.mk


_DISTDIR := $(builddir)/dist


$(builddir)/dist/%/:
	+$(info	$(INFO_)MKDIR		$@)
	+$(INSTALL_DIR) $@


.PHONY: dist
dist: dist-tar dist-z;


endif  # include guard
