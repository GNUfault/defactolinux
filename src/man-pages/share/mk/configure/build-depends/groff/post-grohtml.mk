# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_CONFIGURE_BUILD_DEPENDS_GROFF_POST_GROHTML_INCLUDED
MAKEFILE_CONFIGURE_BUILD_DEPENDS_GROFF_POST_GROHTML_INCLUDED := 1


DEFAULT_POST_GROHTMLFLAGS :=
ifndef POST_GROHTMLFLAGS
POST_GROHTMLFLAGS         :=
endif
ifndef POST_GROHTMLFLAGS_
POST_GROHTMLFLAGS_        := $(DEFAULT_POST_GROHTMLFLAGS) $(POST_GROHTMLFLAGS)
endif
ifndef POST_GROHTML
POST_GROHTML              := post-grohtml
endif


endif  # include guard
