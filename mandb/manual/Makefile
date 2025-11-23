# manual Makefile for the man_db package.
#
# Copyright (C) 1995, Graeme Wilford.
#
# You may distribute under the terms of the GNU General Public
# License as specified in the COPYING file that comes with this
# distribution.
#
# Thu Sep 21 16:53:26 BST 1995  Wilf. (G.Wilford@ee.surrey.ac.uk) 

manual := man_db

SOELIM = soelim

.PHONY: dvi ps tps cat html

all: dvi ps cat

dvi: $(manual).dvi
ps: $(manual).ps
cat: $(manual).cat
tps: $(manual).tps
html: $(manual).html

SRCS := $(wildcard *.me)

postpro: $(SRCS)
	$(SOELIM) $(manual).me | tbl > $@
	
$(manual).dvi: postpro
	groff -me -Tdvi $< > $@

$(manual).ps: postpro
	groff -me -Tps $< > $@

$(manual).tps: postpro
	troff -me -Tdvi $< | grodvi -d > $(manual).tdvi
	dvips -f -t a4 $(manual).tdvi > $@
	rm -f $(manual).tdvi

$(manual).cat: postpro
	nroff -me $< > $@

$(manual).html: postpro
	groff -Thtml -me -P-I$(manual) $< > $@

# The standard targets
install uninstall:

mostlyclean:
	rm -f *~ *.bak

clean: mostlyclean
	rm -f postpro

realclean distclean: clean
	rm -f $(manual).dvi $(manual).ps $(manual).cat $(manual).tps \
	      $(manual).html $(manual)-*.png

TAGS:
	$(MKTAGS) $(SRCS)
