# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_CATMAN_EQN_INCLUDED
MAKEFILE_BUILD_CATMAN_EQN_INCLUDED := 1


include $(MAKEFILEDIR)/build/pre/tbl.mk
include $(MAKEFILEDIR)/configure/build-depends/grep/grep.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/eqn.mk
include $(MAKEFILEDIR)/configure/build-depends/groff-base/nroff.mk


ext := .cat.troff


_CATMAN_troff := $(patsubst %.eqn, %$(ext), $(_MAN_eqn))


$(_CATMAN_troff): %$(ext): %.eqn $(MK) | $$(@D)/
	$(info	$(INFO_)EQN		$@)
	! ($(EQN) -T$(NROFF_OUT_DEVICE) $(EQNFLAGS_) <$< 2>&1 >$@) \
	| $(GREP) ^ >&2


.PHONY: build-catman-eqn
build-catman-eqn: $(_CATMAN_troff);


undefine ext


endif  # include guard
