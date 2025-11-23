/*
 * compression.c: code to find decompressor / compression extension
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Sat Aug 20 15:01:02 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <errno.h>
#include <signal.h>

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
#endif /* STDC_HEADERS */

#ifndef STDC_HEADERS
extern int errno;
#endif

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#ifdef COMP_SRC /* must come after manconfig.h */

#include "lib/error.h"
#include "security.h"
#include "comp_src.h"

static char *file;	/* pointer to temp file name */
static int file_fd = -1;

/* initialise temp filename */
static __inline__ void create_ztemp (void)
{
	int oldmask = umask (022);
	drop_effective_privs ();
	file_fd = create_tempfile ("zman", &file);

	if (file_fd < 0)
		error (FATAL, errno, _("can't create a temporary filename"));
	regain_effective_privs ();
	umask (oldmask);
	atexit (remove_ztemp);
}

/* Take filename as arg, return structure containing decompressor 
   and extension, or NULL if no comp extension found. 
   As an added bonus, return address of comp extension in comp->file
   as this is otherwise unused.

   eg.
   	filename = /usr/man/man1/foo.1.gz 

	comp->prog = "/usr/bin/gzip -dc";
   	comp->ext = "gz";
   	comp->file = filename + 19;				
 */
struct compression *comp_info (char *filename)
{
	char *ext;
	static char buff[10];
	static struct compression hpux_comp = {GUNZIP " -S \"\"", "", buff};

	ext = strrchr (filename, '.');

	if (ext) {
		struct compression *comp;
		ext++;
		for (comp = comp_list; comp->ext; comp++) {
			if (strcmp (comp->ext, ext) == 0) {
				comp->file = --ext;
				return comp;
			}
		}
	}
	ext = strstr (filename, ".Z/");
	if (ext)
		return &hpux_comp;
	return NULL;
}

/* take filename w/o comp ext. as arg, return comp->file as a relative
   compressed file or NULL if none found */
struct compression *comp_file (char *filename)
{
	size_t len;
	char *compfile;
	struct compression *comp;

	compfile = strappend (NULL, filename, ".", NULL);
	len = strlen (compfile);
	
	for (comp = comp_list; comp->ext; comp++) {
		struct stat buf;
		
		compfile = strappend (compfile, comp->ext, NULL);

		if (stat (compfile, &buf) == 0) {
			comp->file = compfile;
			return comp;
		}

		*(compfile + len) = '\0';
	}
	return NULL;
}

/* set up a pointer to a unique temp filename on first call */
char *decompress (const char *filename, const struct compression *comp)
{
	char *command;
	int status;
	int save_debug = debug;
	char *esc_filename, *esc_file;

	if (!file)
		create_ztemp();

	esc_filename = escape_shell (filename);
	esc_file = escape_shell (file);
	/* temporarily drop the debug flag, so that we can continue */
	command = strappend (NULL, comp->prog, " ", esc_filename,
			     " > ", esc_file, NULL);

	if (debug) {
#ifdef SECURE_MAN_UID
		fputs ("The following command done with dropped privs\n",
		       stderr);
#endif /* SECURE_MAN_UID */
		fprintf (stderr, "%s\n", command);
	}

	debug = 0;
	status = do_system_drop_privs (command);
	debug = save_debug;

	free (command);

	if (status) {
		remove_ztemp ();
		exit (CHILD_FAIL);
	}
	return file;
}

/* remove temporary file, drop privs if necessary */
void remove_ztemp (void)
{
	if (file) {
		if (file_fd >= 0)
			close (file_fd);
		(void) remove_with_dropped_privs (file);
		free (file);
		file = NULL;
	}
}

/* return temporary filename */
char *get_ztemp (void)
{
	return file;
}
#endif /* COMP_SRC */
