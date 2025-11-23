# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_EX_LD_INCLUDED
MAKEFILE_BUILD_EX_LD_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/examples/cc.mk
include $(MAKEFILEDIR)/configure/build-depends/binutils/ld.mk
include $(MAKEFILEDIR)/configure/xfail.mk


xfail := $(MAKEFILEDIR)/build/examples/ld.xfail

tgts := $(patsubst %.o, %, $(_EX_TU_o))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %.o, $(_MANDIR)/%, $(file < $(xfail))), $(tgts))
endif


_EX_TU_bin := $(tgts)


$(_EX_TU_bin): %: %.o $(MK)
	$(info	$(INFO_)LD		$@)
	$(LD) $(LDFLAGS_) -o $@ $< $(LDLIBS_)


.PHONY: build-ex-ld
build-ex-ld: $(_EX_TU_bin);


undefine xfail
undefine tgts


endif  # include guard
