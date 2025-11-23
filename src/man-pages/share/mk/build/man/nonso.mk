# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_MAN_NONSO_INCLUDED
MAKEFILE_BUILD_MAN_NONSO_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/configure/build-depends/git/git.mk
include $(MAKEFILEDIR)/configure/build-depends/sed/sed.mk
include $(MAKEFILEDIR)/configure/directory_variables/src.mk
include $(MAKEFILEDIR)/configure/verbose.mk
include $(MAKEFILEDIR)/configure/version.mk
include $(MAKEFILEDIR)/src/man.mk


_NONSO := $(patsubst $(MANDIR)/%, $(_MANDIR)/%, $(NONSO))


$(_NONSO): $(_MANDIR)/%: $(MANDIR)/% $(MK) | $$(@D)/
	$(info	$(INFO_)SED		$@)
	<$< \
	$(SED) "/^\.TH /s/(date)/$$($(MANPAGEDATECMD))/" \
	| $(SED) '/^\.TH /s/(unreleased)/$(DISTVERSION)/' \
	| $(SED) '/^\.Dd /s/$$Mdocdate$$'"/$$($(MANPAGEDATECMD))/" \
	| $(SED) '/^\.Os /s/(unreleased)/$(DISTVERSION)/' \
	>$@


.PHONY: build-man-nonso
build-man-nonso: $(_NONSO)


endif  # include guard
