# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PDF_PAGES_GROPDF_INCLUDED
MAKEFILE_BUILD_PDF_PAGES_GROPDF_INCLUDED := 1


include $(MAKEFILEDIR)/build/pdf/pages/troff.mk
include $(MAKEFILEDIR)/configure/build-depends/groff/gropdf.mk


ext := .pdf


_PDFMAN := $(patsubst %.pdf.set, %$(ext), $(_PDFMAN_set))


$(_PDFMAN): %$(ext): %.pdf.set $(MK) | $$(@D)/
	$(info	$(INFO_)GROPDF		$@)
	$(GROPDF) $(GROPDFFLAGS_) <$< >$@


.PHONY: build-pdf-pages-gropdf
build-pdf-pages-gropdf: $(_PDFMAN);


undefine ext


endif  # include guard
