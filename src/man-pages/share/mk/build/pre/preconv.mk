# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PRE_PRECONV_INCLUDED
MAKEFILE_BUILD_PRE_PRECONV_INCLUDED := 1


include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/preconv.mk


ext := .tbl


_MAN_tbl := $(patsubst %, %$(ext), $(_NONSO))


$(_MAN_tbl): %$(ext): % $(MK) | $$(@D)/
	$(info	$(INFO_)PRECONV		$@)
	$(PRECONV) $(PRECONVFLAGS_) $< >$@


.PHONY: build-pre-preconv
build-pre-preconv: $(_MAN_tbl);


undefine ext


endif  # include guard
