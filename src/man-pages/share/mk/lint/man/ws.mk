# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_WS_INCLUDED
MAKEFILE_LINT_MAN_WS_INCLUDED := 1


include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/cat.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/echo.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk


ext := .lint-man.ws.touch
xfail := $(MAKEFILEDIR)/lint/man/ws.xfail
regexf := $(MAKEFILEDIR)/lint/man/ws.egrep

tgts := $(patsubst %, %$(ext), $(_NONSO))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


$(tgts): %$(ext): % $(regexf) $(MK) | $$(@D)/
	$(info	$(INFO_)GREP		$@)
	$(CAT) <$< \
	| if $(GREP) -Ef $(filter %.egrep, $^) >/dev/null; then \
		>&2 $(ECHO) "lint-man-ws: $<: Spurious white space:"; \
		>&2 $(GREP) -ETnf '$(filter %.egrep, $^)' <$<; \
		exit 1; \
	fi;
	$(TOUCH) $@


.PHONY: lint-man-ws
lint-man-ws: $(tgts);


undefine ext
undefine xfail
undefine regexf
undefine tgts


endif  # include guard
