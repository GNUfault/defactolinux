/*
 * util.c
 *
 * Copyright (c) 1990, 1991, John W. Eaton.
 * Copyright (C) 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * John W. Eaton
 * jwe@che.utexas.edu
 * Department of Chemical Engineering
 * The University of Texas at Austin
 * Austin, Texas  78712
 *
 * Wed May  4 15:44:47 BST 1994 Wilf. (G.Wilford@ee.surrey.ac.uk): slight
 * changes to all routines, mainly cosmetic.
 *
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strrchr();
extern char *strcat();
extern char *strcpy();
#endif /* STDC_HEADERS */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include "manconfig.h"
#include "libdb/mydbm.h" /* for full definition of MAN_DB */

#undef MAX
#define MAX(a,b)	((a)>(b)?a:b)

/* take path, add db name and return */
char *mkdbname (const char *path)
{
	static char *name;
	size_t len = strlen (path);

	name = (char *) xrealloc (name, sizeof MAN_DB + len);
	(void) strcpy (name, path);
	(void) strcpy (name + len, MAN_DB);

	return name;
}

/*
 * Does file a have a different timestamp to file b?
 *
 * case:
 *
 *   a is man_page, b is cat_page
 *
 *   a and b have different times  returns  1/3  (ret & 1) == 1
 *   a and b have same times       returns  0/2  !(ret & 1) == 1
 *   a is zero in length           returns  + 2 (for Wilf. and his stray cats)
 *   b is zero in length           returns  + 4
 *   stat on a fails               returns   -1
 *   stat on b fails               returns   -2
 *   stat on a and b fails         returns   -3
 */
int is_changed (char *fa, char *fb)
{
	struct stat fa_sb;
	struct stat fb_sb;
	int fa_stat;
	int fb_stat;
	int status = 0;

	if (debug)
		fprintf (stderr, "is_changed: a=%s, b=%s", fa, fb);

	fa_stat = stat (fa, &fa_sb);
	if (fa_stat != 0)
		status = 1;

	fb_stat = stat (fb, &fb_sb);
	if (fb_stat != 0)
		status |= 2;

	if (status != 0) {
		if (debug)
			fprintf (stderr, " (%d)\n", -status);
		return -status;
	}

	if (fa_sb.st_size == 0)
		status |= 2;

	if (fb_sb.st_size == 0)
		status |= 4;

	status |= (fa_sb.st_mtime != fb_sb.st_mtime);

	if (debug)
		fprintf (stderr, " (%d)\n", status);
	return status;
}

/*
 * Is path a directory?
 */
int is_directory (char *path)
{
	struct stat sb;
	int status;

	status = stat (path, &sb);

	if (status != 0)
		return status;

	return ((sb.st_mode & S_IFDIR) != 0);
}

/* wrapper for system() */
int do_system (const char *command)
{
  	/*
  	 * If we're debugging, don't really execute the command -- you never
  	 * know what might be in that mangled string :-O.
  	 */
  	 
	if (debug) {
		fprintf (stderr, "\ntrying command: %s\n", command);
		return 0;
	} else {
		return system (command);
	}
}

/* Escape dangerous metacharacters before dumping into a shell command. */
char *escape_shell (const char *unesc)
{
	char *esc, *escp;
	const char *unescp;

	if (!unesc)
		return NULL;

	escp = esc = (char *) xmalloc (strlen (unesc) * 2 + 1);
	for (unescp = unesc; *unescp; unescp++)
		if ((*unescp >= '0' && *unescp <= '9') ||
		    (*unescp >= 'A' && *unescp <= 'Z') ||
		    (*unescp >= 'a' && *unescp <= 'z') ||
		    strchr (",-./:=@_", *unescp))
			*escp++ = *unescp;
		else {
			*escp++ = '\\';
			*escp++ = *unescp;
		}
	*escp = 0;
	return esc;
}

/* Remove a directory and all files in it. Does not recurse beyond that. */
int remove_directory (const char *directory)
{
	DIR *handle = opendir (directory);
	struct dirent *entry;

	if (!handle)
		return -1;
	entry = readdir (handle);
	while (entry) {
		struct stat st;
		char *path = xstrdup (directory);
		path = strappend (path, "/", entry->d_name, NULL);
		if (stat (path, &st) == -1) {
			free (path);
			return -1;
		}
		if (S_ISREG (st.st_mode)) {
			if (unlink (path) == -1) {
				free (path);
				return -1;
			}
		}
		free (path);
		entry = readdir (handle);
	}

	if (rmdir (directory) == -1)
		return -1;
	return 0;
}
