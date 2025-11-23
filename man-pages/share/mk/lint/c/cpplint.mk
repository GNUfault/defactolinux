# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_C_CPPLINT_INCLUDED
MAKEFILE_LINT_C_CPPLINT_INCLUDED := 1


include $(MAKEFILEDIR)/build/examples/src.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/cpplint/cpplint.mk


ext := .lint-c.cpplint.touch
tgts_EX := $(patsubst %, %$(ext), $(_EX_TU_src))
tgts    := $(tgts_EX)


$(tgts_EX): %$(ext): %
$(tgts): $(CPPLINT_CONF) $(MK) | $$(@D)/


$(tgts):
	$(info	$(INFO_)CPPLINT		$@)
	$(CPPLINT) $(CPPLINTFLAGS_) $< >/dev/null
	$(TOUCH) $@


.PHONY: lint-c-cpplint
lint-c-cpplint: $(tgts);


undefine ext
undefine tgts_EX
undefine tgts


endif  # include guard
