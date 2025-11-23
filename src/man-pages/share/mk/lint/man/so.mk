# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_SO_INCLUDED
MAKEFILE_LINT_MAN_SO_INCLUDED := 1


include $(MAKEFILEDIR)/build/_.mk
include $(MAKEFILEDIR)/build/man/so.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/cut.mk
include $(MAKEFILEDIR)/configure/build-depends/findutils/find.mk
include $(MAKEFILEDIR)/configure/build-depends/findutils/xargs.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/xfail.mk


ext := .lint-man.so.touch
xfail := $(MAKEFILEDIR)/lint/man/so.xfail

tgts := $(patsubst %, %$(ext), $(_SO_MAN))
ifeq ($(SKIP_XFAIL),yes)
tgts := $(filter-out $(patsubst %, $(_MANDIR)/%$(ext), $(file < $(xfail))), $(tgts))
endif


$(tgts): %$(ext): % $(MK) | $$(@D)/
	$(info	$(INFO_)FIND		$@)
	$(GREP) '^\.so ' <$< \
	| $(CUT) -f2 -d' ' \
	| $(XARGS) -I {} $(FIND) '$(MANDIR)/{}' \
	>$@


.PHONY: lint-man-so
lint-man-so: $(tgts);


undefine ext
undefine xfail
undefine tgts


endif  # include guard
