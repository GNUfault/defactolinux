# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_EX_DIR_INCLUDED
MAKEFILE_BUILD_EX_DIR_INCLUDED := 1


include $(MAKEFILEDIR)/build/man/nonso.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/mkdir.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk


_PAGEEXDIRS := $(patsubst %, %.d/, $(_NONSO))


$(_PAGEEXDIRS):
	+$(info	$(INFO_)MKDIR		$@)
	+$(MKDIR) -p $@
	+$(TOUCH) $@


.PHONY: build-ex-dir
build-ex-dir: $(_PAGEEXDIRS);


endif  # include guard
