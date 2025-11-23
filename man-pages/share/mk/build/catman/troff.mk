# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_CATMAN_TROFF_INCLUDED
MAKEFILE_BUILD_CATMAN_TROFF_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/true.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/nroff.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/troff.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .cat.set
xfail := $(MAKEFILEDIR)/build/catman/troff.xfail
regexf := $(MAKEFILEDIR)/build/catman/troff.ignore.grep

tgts := $(patsubst %, %$(ext), $(_NONSO))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


_CATMAN_set := $(tgts)


$(_CATMAN_set): %$(ext): %.cat.troff $(regexf) $(MK) | $$(@D)/
	$(info	$(INFO_)TROFF		$@)
	! ($(TROFF) -mandoc $(TROFFFLAGS_) $(NROFFFLAGS_) <$< 2>&1 >$@ \
	   | $(GREP) -v -f '$(filter %.grep, $^)' \
	   || $(TRUE); \
	) \
	| $(GREP) ^ >&2


.PHONY: build-catman-troff
build-catman-troff: $(_CATMAN_set);


undefine ext
undefine xfail
undefine regexf
undefine tgts


endif  # include guard
