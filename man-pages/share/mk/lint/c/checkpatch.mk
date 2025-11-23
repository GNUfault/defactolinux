# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_C_CHECKPATCH_INCLUDED
MAKEFILE_LINT_C_CHECKPATCH_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/examples/src.mk
include $(MAKEFILEDIR)/configure/build-depends/checkpatch/checkpatch.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .lint-c.checkpatch.touch
xfail := $(MAKEFILEDIR)/lint/c/iwyu.xfail

tgts_EX := $(patsubst %, %$(ext), $(_EX_TU_src))
ifeq ($(SKIP_XFAIL),yes)
tgts_EX := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts_EX))
endif

tgts := $(tgts_EX)


$(tgts_EX): %$(ext): %
$(tgts): $(CHECKPATCH_CONF) $(MK) | $$(@D)/


$(tgts):
	$(info	$(INFO_)CHECKPATCH	$@)
	$(CHECKPATCH) $(CHECKPATCHFLAGS_) -f $< >&2
	$(TOUCH) $@


.PHONY: lint-c-checkpatch
lint-c-checkpatch: $(tgts);


undefine ext
undefine xfail
undefine tgts_EX
undefine tgts


endif  # include guard
