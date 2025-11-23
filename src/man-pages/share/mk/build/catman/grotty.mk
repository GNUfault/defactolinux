# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_CATMAN_GROTTY_INCLUDED
MAKEFILE_BUILD_CATMAN_GROTTY_INCLUDED := 1


include $(MAKEFILEDIR)/build/catman/troff.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/grotty.mk


ext := .cat


_CATMAN := $(patsubst %.cat.set, %$(ext), $(_CATMAN_set))


$(_CATMAN): %$(ext): %.cat.set $(MK) | $$(@D)/
	$(info	$(INFO_)GROTTY		$@)
	$(GROTTY) $(GROTTYFLAGS_) <$< >$@


.PHONY: build-catman-grotty
build-catman-grotty: $(_CATMAN);


undefine ext


endif  # include guard
