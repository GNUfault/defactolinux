# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_QUOTE_INCLUDED
MAKEFILE_LINT_MAN_QUOTE_INCLUDED := 1


include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/cat.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/echo.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk


ext := .lint-man.quote.touch
xfail := $(MAKEFILEDIR)/lint/man/quote.xfail
regexf := $(MAKEFILEDIR)/lint/man/quote.Pgrep

tgts := $(patsubst %, %$(ext), $(_NONSO))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


$(tgts): %$(ext): % $(regexf) $(MK) | $$(@D)/
	$(info	$(INFO_)GREP		$@)
	$(CAT) <$< \
	| if $(GREP) -Pf $(filter %.Pgrep, $^) >/dev/null; then \
		>&2 $(ECHO) "lint-man-quote: $<: Unmatched quote:"; \
		>&2 $(GREP) -PTnf '$(filter %.Pgrep, $^)' <$<; \
		exit 1; \
	fi;
	$(TOUCH) $@


.PHONY: lint-man-quote
lint-man-quote: $(tgts);


undefine ext
undefine xfail
undefine regexf
undefine tgts


endif  # include guard
