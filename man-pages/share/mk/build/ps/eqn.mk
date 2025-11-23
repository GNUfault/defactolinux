# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_PS_EQN_INCLUDED
MAKEFILE_BUILD_PS_EQN_INCLUDED := 1


include $(MAKEFILEDIR)/build/pre/tbl.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/eqn.mk


ext := .ps.troff


_PSMAN_troff := $(patsubst %.eqn, %$(ext), $(_MAN_eqn))


$(_PSMAN_troff): %$(ext): %.eqn $(MK) | $$(@D)/
	$(info	$(INFO_)EQN		$@)
	! ($(EQN) -Tps $(EQNFLAGS_) <$< 2>&1 >$@) \
	| $(GREP) ^ >&2


.PHONY: build-ps-eqn
build-ps-eqn: $(_PSMAN_troff);


undefine ext


endif  # include guard
