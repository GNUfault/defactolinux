# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_DIST_Z_INCLUDED
MAKEFILE_DIST_Z_INCLUDED := 1


include $(MAKEFILEDIR)/configure/build-depends/bzip2/bzip2.mk
include $(MAKEFILEDIR)/configure/build-depends/coreutils/touch.mk
include $(MAKEFILEDIR)/configure/build-depends/gzip/gzip.mk
include $(MAKEFILEDIR)/configure/build-depends/lzip/lzip.mk
include $(MAKEFILEDIR)/configure/build-depends/xz-utils/xz.mk
include $(MAKEFILEDIR)/dist/tar.mk


compression := bz2 gz lz xz


define _DISTFILE_z_rule
$(_DISTFILE).$(2): %.$(2): % $(MK) | $$$$(@D)/
	$$(info	$(INFO_)$(1)		$$@)
	$($(1)) $($(1)FLAGS_) -kf $$<
	$(TOUCH) $$@
endef


$(eval $(call _DISTFILE_z_rule,BZIP2,bz2))
$(eval $(call _DISTFILE_z_rule,GZIP,gz))
$(eval $(call _DISTFILE_z_rule,LZIP,lz))
$(eval $(call _DISTFILE_z_rule,XZ,xz))


$(foreach z, $(compression),                                                  \
	$(eval .PHONY: dist-z-$(z)))
$(foreach z, $(compression),                                                  \
	$(eval dist-z-$(z): $(_DISTFILE).$(z);))


.PHONY: dist-z
dist-z: $(foreach z, $(compression), dist-z-$(z));


endif  # include guard
