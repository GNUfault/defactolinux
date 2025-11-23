# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_CONFIGURE_BUILD_DEPENDS_FONTFORGE_FONTFORGE_INCLUDED
MAKEFILE_CONFIGURE_BUILD_DEPENDS_FONTFORGE_FONTFORGE_INCLUDED := 1


DEFAULT_FONTFORGEFLAGS :=
ifndef FONTFORGEFLAGS
FONTFORGEFLAGS         :=
endif
ifndef FONTFORGEFLAGS_
FONTFORGEFLAGS_        := $(DEFAULT_FONTFORGEFLAGS) $(FONTFORGEFLAGS)
endif
ifndef FONTFORGE
FONTFORGE              := fontforge
endif


endif  # include guard
