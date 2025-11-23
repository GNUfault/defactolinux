# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_CHECK_DIST_INCLUDED
MAKEFILE_DIST_CHECK_DIST_INCLUDED := 1


include $(MAKEFILEDIR)/configure/version.mk
include $(MAKEFILEDIR)/dist/check/_.mk
include $(MAKEFILEDIR)/dist/check/tar.mk


REDIST := $(_DISTCHECKBUILDDIR)/$(DISTNAME).tar


$(REDIST): $(_DISTCHECKSRCDIR) $(MK) | $$(@D)/
	$(info	$(INFO_)MAKE		dist-tar)
	$(MAKE) $(_MAKE_OPTS) dist-tar \
		'INFO_= dist-tar:	'


.PHONY: distcheck-dist-tar
distcheck-dist-tar: $(REDIST);


endif  # include guard
