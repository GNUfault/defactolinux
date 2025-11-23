AUTOMAKE_OPTIONS = foreign

SUBDIRS = po4a

EXTRA_DIST = CONTRIBUTING

bin_PROGRAMS = run-parts tempfile ischroot
run_parts_SOURCES = run-parts.c
tempfile_SOURCES = tempfile.c
ischroot_SOURCES = ischroot.c

dist_bin_SCRIPTS = which.debianutils savelog

dist_sbin_SCRIPTS = installkernel add-shell remove-shell update-shells

dist_man_MANS = run-parts.8 \
	   installkernel.8 savelog.8 \
	   tempfile.1 which.debianutils.1 add-shell.8 \
	   remove-shell.8 update-shells.8 ischroot.1
