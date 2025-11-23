/* tempfile.c: handle temporary file creation.
 *
 * Copyright (c) 2001 Colin Watson <cjwatson@debian.org>.
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#ifdef STDC_HEADERS
#  include <stdlib.h>
#endif /* STDC_HEADERS */
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <sys/types.h>
#include <sys/stat.h>

/* Other library functions used in man_db. */
extern char *strappend (char *str, ...);
extern char *xstrdup (char *string);

static char *path_search ()
{
	char *dir = NULL;

	if (getuid () == geteuid () && getgid () == getegid ()) {
		dir = getenv ("TMPDIR");
		if (!dir || access (dir, W_OK) == -1)
			dir = NULL;
		if (!dir) {
			dir = getenv ("TMP");
			if (!dir || access (dir, W_OK) == -1)
				dir = NULL;
		}
	}
#ifdef P_tmpdir
	if (!dir) {
		dir = P_tmpdir;
		if (!dir || access (dir, W_OK) == -1)
			dir = NULL;
	}
#endif
	if (!dir) {
		dir = "/tmp";
		if (access (dir, W_OK) == -1)
			dir = NULL;
	}

	return dir;
}

/* Get a handle for a sane temporary file, looking in $TMPDIR, P_tmpdir, and
 * finally /tmp.
 */
int create_tempfile (const char *template, char **created_filename)
{
	char *dir = path_search ();
	int fd;
	mode_t old_mode;

	if (!dir)
		return -1;
	dir = xstrdup (dir);
	*created_filename = strappend (dir, "/", template, "XXXXXX", NULL);
	/* -rw------- */
	old_mode = umask (S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	fd = mkstemp (*created_filename);
	umask (old_mode);
	return fd;
}

/* Get a sane temporary directory, looking in $TMPDIR, P_tmpdir, and finally
 * /tmp.
 */
char *create_tempdir (const char *template)
{
	char *dir = path_search ();
	char *created_dirname;

	if (!dir)
		return NULL;
	dir = xstrdup (dir);
	created_dirname = strappend (dir, "/", template, "XXXXXX", NULL);
	mkdtemp (created_dirname);
	return created_dirname;
}
