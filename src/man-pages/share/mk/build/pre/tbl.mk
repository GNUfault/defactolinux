# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PRE_TBL_INCLUDED
MAKEFILE_BUILD_PRE_TBL_INCLUDED := 1


include $(MAKEFILEDIR)/build/pre/preconv.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/tbl.mk


ext := .eqn


_MAN_eqn := $(patsubst %.tbl, %$(ext), $(_MAN_tbl))


$(_MAN_eqn): %$(ext): %.tbl $(MK) | $$(@D)/
	$(info	$(INFO_)TBL		$@)
	$(TBL) <$< >$@


.PHONY: build-pre-tbl
build-pre-tbl: $(_MAN_eqn);


undefine ext


endif  # include guard
