# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_BLANK_INCLUDED
MAKEFILE_LINT_MAN_BLANK_INCLUDED := 1


include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/cat.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/echo.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk


ext := .lint-man.blank.touch
xfail := $(MAKEFILEDIR)/lint/man/blank.xfail

tgts := $(patsubst %, %$(ext), $(_NONSO))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


$(tgts): %$(ext): % $(MK) | $$(@D)/
	$(info	$(INFO_)GREP		$@)
	$(CAT) <$< \
	| if $(GREP) '^$$' >/dev/null; then \
		>&2 $(ECHO) "lint-man-blank: $<: spurious blank lines:"; \
		>&2 $(GREP) -nT '^$$' <$<; \
		exit 1; \
	fi;
	$(TOUCH) $@


.PHONY: lint-man-blank
lint-man-blank: $(tgts);


undefine ext
undefine xfail
undefine tgts


endif  # include guard
