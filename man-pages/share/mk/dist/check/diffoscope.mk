# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_CHECK_DIFFOSCOPE_INCLUDED
MAKEFILE_DIST_CHECK_DIFFOSCOPE_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/diffoscope-minimal/diffoscope.mk
include $(MAKEFILEDIR)/configure/directory_variables/build.mk
include $(MAKEFILEDIR)/dist/check/dist.mk
include $(MAKEFILEDIR)/dist/tar.mk


$(builddir)/distcheck.diffoscope.touch: $(_DISTFILE) $(REDIST) | $$(@D)/
	$(info	$(INFO_)DIFFOSCOPE	$^)
	$(DIFFOSCOPE) $^
	$(TOUCH) $@


.PHONY: distcheck-diffoscope
distcheck-diffoscope: $(builddir)/distcheck.diffoscope.touch;


endif  # include guard
