# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PDF_PAGES_EQN_INCLUDED
MAKEFILE_BUILD_PDF_PAGES_EQN_INCLUDED := 1


include $(MAKEFILEDIR)/build/pre/tbl.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/eqn.mk


ext := .pdf.troff


_PDFMAN_troff := $(patsubst %.eqn, %$(ext), $(_MAN_eqn))


$(_PDFMAN_troff): %$(ext): %.eqn $(MK) | $$(@D)/
	$(info	$(INFO_)EQN		$@)
	! ($(EQN) -Tpdf $(EQNFLAGS_) <$< 2>&1 >$@) \
	| $(GREP) ^ >&2


.PHONY: build-pdf-pages-eqn
build-pdf-pages-eqn: $(_PDFMAN_troff);


undefine ext


endif  # include guard
