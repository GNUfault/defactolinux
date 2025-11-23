# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_CHECK_TAR_INCLUDED
MAKEFILE_DIST_CHECK_TAR_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/coreutils/cp.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/rm.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/tar/tar.mk
include $(MAKEFILEDIR)/configure/version.mk
include $(MAKEFILEDIR)/dist/check/_.mk
include $(MAKEFILEDIR)/dist/tar.mk


$(_DISTCHECKDIR)/$(DISTFILE): $(_DISTFILE) $(MK) | $$(@D)/
	$(info	$(INFO_)CP		$@)
	$(CP) -T $< $@

$(_DISTCHECKSRCDIR): %: %.tar $(MK) | $$(@D)/
	$(info	$(INFO_)TAR xf		$<)
	$(RM) -rf $@
	cd $(dir $<) \
	&& $(TAR) xf $(notdir $<)
	$(TOUCH) $@


.PHONY: distcheck-tar
distcheck-tar: $(_DISTCHECKSRCDIR);


endif  # include guard
