# Copyright, the authors of the Linux man-pages project
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception


ifndef MAKEFILE_BUILD_HTML_POST_GROHTML_INCLUDED
MAKEFILE_BUILD_HTML_POST_GROHTML_INCLUDED := 1


include $(MAKEFILEDIR)/build/html/troff.mk
include $(MAKEFILEDIR)/configure/build-depends/groff/post-grohtml.mk


ext := .html


_HTMLMAN := $(patsubst %.html.set, %$(ext), $(_HTMLMAN_set))


$(_HTMLMAN): %$(ext): %.html.set $(MK) | $$(@D)/
	$(info	$(INFO_)POST_GROHTML	$@)
	$(POST_GROHTML) $(POST_GROHTMLFLAGS_) <$< >$@


.PHONY: build-html-post-grohtml
build-html-post-grohtml: $(_HTMLMAN);


undefine ext


endif  # include guard
