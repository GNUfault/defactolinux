# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_TAR_INCLUDED
MAKEFILE_DIST_TAR_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/coreutils/sort.mk
include $(MAKEFILEDIR)/configure/build-depends/findutils/xargs.mk
include $(MAKEFILEDIR)/configure/build-depends/sed/sed.mk
include $(MAKEFILEDIR)/configure/build-depends/tar/tar.mk
include $(MAKEFILEDIR)/configure/version.mk
include $(MAKEFILEDIR)/dist/_.mk
include $(MAKEFILEDIR)/dist/files.mk


DISTFILE  := $(DISTNAME).tar
_DISTFILE := $(builddir)/$(DISTFILE)


$(_DISTFILE): $(_DISTFILES) $(MK) | $$(@D)/
	$(info	$(INFO_)TAR		$@)
	$(TAR) $(TARFLAGS_) -cf $@ -T /dev/null
	$(DISTFILESCMD) \
	| $(SED) 's,^$(srcdir)/,$(_DISTDIR)/,' \
	| $(SORT) \
	| $(XARGS) $(TAR) $(TARFLAGS_) -rf $@ -C $(srcdir) \
		--transform 's,^$(patsubst /%,%,$(_DISTDIR)),$(DISTNAME),'


.PHONY: dist-tar
dist-tar: $(_DISTFILE);


endif  # include guard
