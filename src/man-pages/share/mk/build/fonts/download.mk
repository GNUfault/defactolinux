# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_FONTS_DOWNLOAD_INCLUDED
MAKEFILE_BUILD_FONTS_DOWNLOAD_INCLUDED := 1


include $(MAKEFILEDIR)/build/fonts/_.mk


_DOWNLOAD := $(_FONTSDIR)/devpdf/download


$(_DOWNLOAD): $(_FONTSDIR)/%: $(MAKEFILEDIR)/build/fonts/% $(MK) | $$(@D)/
	$(info	$(INFO_)CP		$@)
	$(CP) -T $< $@


.PHONY: build-fonts-download
build-fonts-download: $(_DOWNLOAD)


endif  # include guard
