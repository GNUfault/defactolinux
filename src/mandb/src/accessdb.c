/*
 * accessdb.c: show every key/content pair in the database.
 *
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Tue Apr 26 12:56:44 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <errno.h>

#ifndef STDC_HEADERS
extern int errno;
#endif

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strchr();
#endif /* STDC_HEADERS */

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "libdb/mydbm.h"
#include "lib/error.h"

char *program_name;
char *cat_root;
int debug;

/* for db_storage.c */
char *database;
MYDBM_FILE dbf;

static void usage (int status)
{
	fprintf (stderr,
		 _("\nUsage: accessdb [man_database]\n"
		 "\tman_database defaults to %s" MAN_DB "\n"), cat_root);

	exit (status);
}

int main (int argc, char *argv[])
{
	MYDBM_FILE dbf;
	datum key;

	program_name = basename (argv[0]);
	if (is_directory (FHS_CAT_ROOT) == 1)
		cat_root = FHS_CAT_ROOT;
	else if (is_directory (CAT_ROOT) == 1)
		cat_root = CAT_ROOT;

	if (argc > 2)
		usage (FAIL);
	else if (argc == 2) 
		database = argv[1];
	else
		database = strappend (NULL, cat_root, MAN_DB, NULL);
		
	dbf = MYDBM_RDOPEN (database);
	if (dbf && dbver_rd (dbf)) {
		MYDBM_CLOSE (dbf);
		dbf = NULL;
	}
	if (!dbf) {
		error (0, errno, _("can't open %s for reading"), database);
		usage (FAIL);
	}

	key = MYDBM_FIRSTKEY (dbf);

	while (key.dptr != NULL) {
		datum content;
		char *t, *nicekey;

		content = MYDBM_FETCH (dbf, key);
		if (!content.dptr)
			exit (FATAL);
		nicekey = xstrdup (key.dptr);
		while ( (t = strchr (nicekey, '\t')) )
			*t = '~';
		while ( (t = strchr (content.dptr, '\t')) )
			*t = ' ';
		printf ("%s -> \"%s\"\n", nicekey, content.dptr);
		free (nicekey); 
		MYDBM_FREE (content.dptr);
		key = MYDBM_NEXTKEY (dbf, key);
	}

	MYDBM_CLOSE (dbf);
	exit (OK);
}
