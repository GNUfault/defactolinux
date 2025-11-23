# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_CONFIGURE_BUILD_DEPENDS_GROFF_BASE_TROFF_INCLUDED
MAKEFILE_CONFIGURE_BUILD_DEPENDS_GROFF_BASE_TROFF_INCLUDED := 1


DEFAULT_TROFFFLAGS := \
	-wbreak \
	-rS12 \
	-rU0
ifndef TROFFFLAGS
TROFFFLAGS         :=
endif
ifndef TROFFFLAGS_
TROFFFLAGS_        := $(DEFAULT_TROFFFLAGS) $(TROFFFLAGS)
endif
ifndef TROFF
TROFF              := troff
endif


endif  # include guard
