# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_CHECK_CATMAN_GREP_INCLUDED
MAKEFILE_CHECK_CATMAN_GREP_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/man/man.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .check-catman.touch
xfail := $(MAKEFILEDIR)/check/catman/grep.xfail

tgts := $(patsubst %.cat.grep, %$(ext), $(_CHECK_catman_grep))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


_CHECK_catman := $(tgts)


$(_CHECK_catman): %.check-catman.touch: %.cat.grep $(MK) | $$(@D)/
	$(info	$(INFO_)GREP		$@)
	! $(GREP) -n '.\{$(MANWIDTH)\}.' $< /dev/null >&2
	$(TOUCH) $@


.PHONY: check-catman-grep
check-catman-grep: $(_CHECK_catman);


undefine ext
undefine xfail
undefine tgts


endif  # include guard
