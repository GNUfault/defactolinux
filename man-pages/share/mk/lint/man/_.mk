# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_LINT_MAN_INCLUDED
MAKEFILE_LINT_MAN_INCLUDED := 1


.PHONY: lint-man
lint-man: \
	lint-man-blank \
	lint-man-dash \
	lint-man-mandoc \
	lint-man-poems \
	lint-man-quote \
	lint-man-so \
	lint-man-tbl \
	lint-man-ws;


endif  # include guard
