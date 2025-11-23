# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_MAN_INCLUDED
MAKEFILE_BUILD_MAN_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/configure/directory_variables/src.mk
include $(MAKEFILEDIR)/src/man.mk


_MANPAGES := $(patsubst $(MANDIR)/%, $(_MANDIR)/%, $(MANPAGES))


.PHONY: build-man
build-man: build-man-nonso build-man-so;


endif  # include guard
