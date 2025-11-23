# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_MANDOC_INCLUDED
MAKEFILE_LINT_MAN_MANDOC_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/true.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/mandoc/mandoc.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .lint-man.mandoc.touch
xfail := $(MAKEFILEDIR)/lint/man/mandoc.xfail
regexf := $(MAKEFILEDIR)/lint/man/mandoc.ignore.grep

tgts := $(patsubst %, %$(ext), $(_NONSO))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


$(tgts): %$(ext): % $(regexf) $(MK) | $$(@D)/
	$(info	$(INFO_)MANDOC		$@)
	! ($(MANDOC) $(MANDOCFLAGS_) $< 2>&1 \
	   | $(GREP) -v -f '$(filter %.grep, $^)' \
	   || $(TRUE); \
	) \
	| $(GREP) ^ >&2
	$(TOUCH) $@


.PHONY: lint-man-mandoc
lint-man-mandoc: $(tgts);


undefine ext
undefine xfail
undefine regexf
undefine tgts


endif  # include guard
