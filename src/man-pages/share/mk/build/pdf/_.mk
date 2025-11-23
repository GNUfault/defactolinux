# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PDF_INCLUDED
MAKEFILE_BUILD_PDF_INCLUDED := 1


.PHONY: build-pdf
build-pdf: build-pdf-book build-pdf-pages;


endif  # include guard
