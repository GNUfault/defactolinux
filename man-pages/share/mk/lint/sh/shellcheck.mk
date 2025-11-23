# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_SH_SHELLCHECK_INCLUDED
MAKEFILE_LINT_SH_SHELLCHECK_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/shellcheck/shellcheck.mk
include $(MAKEFILEDIR)/src/sh.mk


ext := .lint-sh.shellcheck.touch
tgts := $(patsubst $(SRCBINDIR)/%, $(builddir)/%$(ext), $(BIN_sh))


$(tgts): $(builddir)/%$(ext): $(SRCBINDIR)/% $(SHELLCHECK_CONF) $(MK) | $$(@D)/
	$(info	$(INFO_)SHELLCHECK	$@)
	$(SHELLCHECK) $(SHELLCHECKFLAGS_) $<
	$(TOUCH) $@


.PHONY: lint-sh-shellcheck
lint-sh-shellcheck: $(tgts);


undefine ext
undefine tgts


endif  # include guard
