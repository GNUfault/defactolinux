# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_FONTS_INCLUDED
MAKEFILE_BUILD_FONTS_INCLUDED := 1


include $(MAKEFILEDIR)/configure/directory_variables/build.mk


_FONTSDIR := $(builddir)/fonts


.PHONY: build-fonts
build-fonts: build-fonts-download build-fonts-tinos


endif  # include guard
