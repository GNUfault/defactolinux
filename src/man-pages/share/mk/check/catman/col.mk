# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_CHECK_CATMAN_COL_INCLUDED
MAKEFILE_CHECK_CATMAN_COL_INCLUDED := 1


include $(MAKEFILEDIR)/build/catman/grotty.mk
include $(MAKEFILEDIR)/configure/build-depends/bsdextrautils/col.mk


ext := .cat.grep


_CHECK_catman_grep := $(patsubst %.cat, %$(ext), $(_CATMAN))


$(_CHECK_catman_grep): %$(ext): %.cat $(MK) | $$(@D)/
	$(info	$(INFO_)COL		$@)
	$(COL) $(COLFLAGS_) <$< >$@


.PHONY: check-catman-col
check-catman-col: $(_CHECK_catman_grep);


undefine ext


endif  # include guard
