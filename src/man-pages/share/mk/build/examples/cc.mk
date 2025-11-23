# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_EX_CC_INCLUDED
MAKEFILE_BUILD_EX_CC_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/examples/src.mk
include $(MAKEFILEDIR)/configure/build-depends/gcc/cc.mk
include $(MAKEFILEDIR)/configure/build-depends/cpp/cpp.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .o
xfail := $(MAKEFILEDIR)/build/examples/cc.xfail

tgts := $(patsubst %.c, %$(ext), $(_EX_TU_c))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %.c, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


_EX_TU_o := $(tgts)


$(_EX_TU_o): %$(ext): %.c $(MK)
	$(info	$(INFO_)CC		$@)
	$(CC) -c $(CFLAGS_) $(CPPFLAGS_) -o $@ $<


.PHONY: build-ex-cc
build-ex-cc:  $(_EX_TU_o);


undefine ext
undefine xfail
undefine tgts


endif  # include guard
