# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_CONFIGURE_BUILD_DEPENDS_GROFF_BASE_GROTTY_INCLUDED
MAKEFILE_CONFIGURE_BUILD_DEPENDS_GROFF_BASE_GROTTY_INCLUDED := 1


DEFAULT_GROTTYFLAGS := -c
ifndef GROTTYFLAGS
GROTTYFLAGS         :=
endif
ifndef GROTTYFLAGS_
GROTTYFLAGS_        := $(DEFAULT_GROTTYFLAGS) $(GROTTYFLAGS)
endif
ifndef GROTTY
GROTTY              := grotty
endif


endif  # include guard
