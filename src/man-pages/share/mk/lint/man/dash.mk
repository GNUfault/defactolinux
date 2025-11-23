# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_DASH_INCLUDED
MAKEFILE_LINT_MAN_DASH_INCLUDED := 1


include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/awk/awk.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/cat.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/echo.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/pcre2-utils/pcre2grep.mk
include $(MAKEFILEDIR)/configure/build-depends/sed/sed.mk


ext := .lint-man.dash.touch
regexf := $(MAKEFILEDIR)/lint/man/dash.pcre2grep

tgts := $(patsubst %, %$(ext), $(_NONSO))


$(tgts): %$(ext): % $(regexf) $(MK) | $$(@D)/
	$(info	$(INFO_)PCRE2GREP	$@)
	$(CAT) <$< \
	| if $(PCRE2GREP) -f '$(filter %.pcre2grep, $^)' >/dev/null; then \
		>&2 $(ECHO) "lint-man-dash: $<: Unescaped dash:"; \
		$(PCRE2GREP) -n -f '$(filter %.pcre2grep, $^)' <$< \
		| $(SED) -E 's/([^:]+:)(.*)/\1\n\t\2/' \
		| $(AWK) -F: \
			'/^[^\t]/{ printf("%7d:", $$1); }; \
			 /^\t/{ print($$0); };' \
		>&2; \
		exit 1; \
	fi;
	$(TOUCH) $@


.PHONY: lint-man-dash
lint-man-dash: $(tgts);


undefine ext
undefine regexf
undefine tgts


endif  # include guard
