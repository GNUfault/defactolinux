# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


SHELL       := bash
.SHELLFLAGS := -Eeuo pipefail -c


ifneq (4.4.999,$(firstword $(sort 4.4.999 $(MAKE_VERSION))))
  ifneq (R,$(findstring R, $(firstword -$(MAKEFLAGS))))
    $(error Please run make(1) with the '-R' option)
  endif
endif


MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables
MAKEFLAGS += --warn-undefined-variables


ifndef srcdir
srcdir      := .
endif
MAKEFILEDIR := $(CURDIR)/share/mk


INFO_ :=


.PHONY: all
all: build;


.SECONDEXPANSION:


MK_ := $(wildcard $(addprefix $(MAKEFILEDIR)/, *.mk */*.mk */*/*.mk */*/*/*.mk))
MK  := $(CURDIR)/GNUmakefile $(MK_)
include $(MK_)
$(MK):: ;


.PHONY: nothing
nothing:;


.PHONY: help
help:
	$(info	$(INFO_)Common targets:)
	$(info	$(INFO_)	all		Synonym of 'build')
	$(info	$(INFO_)	build		Build the usual stuff)
	$(info	$(INFO_)	build-all	Build everything)
	$(info	$(INFO_)	check		Check the results of the build)
	$(info	$(INFO_)	clean		Remove all temporary files)
	$(info	$(INFO_)	dist		Produce the release tarball)
	$(info	$(INFO_)	distcheck	Check the release tarball)
	$(info	$(INFO_)	help		Print this help)
	$(info	$(INFO_)	install		Install the usual stuff)
	$(info	$(INFO_)	install-all	Install everything)
	$(info	$(INFO_)	lint		Lint the source code)
	$(info	$(INFO_)	nothing		Do nothing; useful for debugging)
	$(info	$(INFO_)	uninstall	Uninstall everything (might leave traces))
	$(info	)
	$(info	$(INFO_)To see the full list of targets, run:)
	$(info	$(INFO_)	$$ make -R -p nothing \)
	$(info	$(INFO_)	| grep '^\.PHONY:' \)
	$(info	$(INFO_)	| tr ' ' '\n' \)
	$(info	$(INFO_)	| grep -v '^\.PHONY:' \)
	$(info	$(INFO_)	| sort;)
	$(info	)
	$(info	$(INFO_)To see a list of variables, run:)
	$(info	$(INFO_)	$$ find GNUmakefile share/mk/configure -type f \)
	$(info	$(INFO_)	| sort \)
	$(info	$(INFO_)	| xargs grep '^[^[:space:]].*=' \)
	$(info	$(INFO_)	| sed 's/=.*/=/' \)
	$(info	$(INFO_)	| grep -v -e ':DEFAULT_.*=' -e ':MAKEFILE_.*INCLUDED :=';)
	$(info	)
	$(info	$(INFO_)To see a list of dependencies (package/program), run:)
	$(info	$(INFO_)	$$ find share/mk/configure/build-depends -type f \)
	$(info	$(INFO_)	| sed 's,share/mk/configure/build-depends/,,' \)
	$(info	$(INFO_)	| sed 's,\.mk,,' \)
	$(info	$(INFO_)	| sort;)
	$(info	)


.DELETE_ON_ERROR:
.SILENT:
FORCE:
