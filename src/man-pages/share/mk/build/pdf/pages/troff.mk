# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PDF_PAGES_TROFF_INCLUDED
MAKEFILE_BUILD_PDF_PAGES_TROFF_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/troff.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .pdf.set
xfail := $(MAKEFILEDIR)/build/pdf/pages/troff.xfail

tgts := $(patsubst %, %$(ext), $(_NONSO))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(shell cat $(xfail))), $(tgts))
endif


_PDFMAN_set  := $(tgts)


$(_PDFMAN_set): %$(ext): %.pdf.troff $(MK) | $$(@D)/
	$(info	$(INFO_)TROFF		$@)
	! ($(TROFF) -mandoc -Tpdf $(TROFFFLAGS_) <$< 2>&1 >$@) \
	| $(GREP) ^ >&2


.PHONY: build-pdf-pages-troff
build-pdf-pages-troff: $(_PDFMAN_set);


undefine ext
undefine xfail
undefine tgts


endif  # include guard
