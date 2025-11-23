# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_CHECK_INCLUDED
MAKEFILE_DIST_CHECK_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/coreutils/realpath.mk
include $(MAKEFILEDIR)/configure/directory_variables/build.mk
include $(MAKEFILEDIR)/configure/version.mk


_DISTCHECKDIR      := $(shell $(REALPATH) -m $(builddir)/distcheck)
_DISTCHECKSRCDIR   := $(_DISTCHECKDIR)/$(DISTNAME)
_DISTCHECKBUILDDIR := $(_DISTCHECKDIR)/$(DISTNAME)_builddir
_DISTCHECKDESTDIR  := $(_DISTCHECKDIR)/$(DISTNAME)_destdir
_DISTCHECK_MANDIR  := $(_DISTCHECKBUILDDIR)/man


_MAKE_OPTS = \
	-C $< \
	'builddir=$(_DISTCHECKBUILDDIR)' \
	'DESTDIR=$(_DISTCHECKDESTDIR)'


distcheck-%: $(_DISTCHECKSRCDIR) $(MK) | $$(@D)/
	$(info	$(INFO_)MAKE		$@)
	$(MAKE) $(_MAKE_OPTS) $* \
		'INFO_= $*:	'


.PHONY: distcheck
distcheck: distcheck-diffoscope
distcheck: $(_DISTCHECKSRCDIR) $(MK)
	$(info	$(INFO_)MAKE		lint build-all install-all dist)
	$(MAKE) $(_MAKE_OPTS) lint build-all install-all dist \
		'INFO_= distcheck:	'
	$(info	$(INFO_)MAKE		check)
	$(MAKE) $(_MAKE_OPTS) check \
		'INFO_= distcheck:	'


endif  # include guard
