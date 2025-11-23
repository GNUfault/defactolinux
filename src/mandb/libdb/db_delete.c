/*
 * db_delete.c: dbdelete(), database delete routine.
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU Library General Public
 * License as specified in the file COPYING.LIB that comes with this
 * distribution.
 *
 * Mon Aug  8 20:35:30 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <assert.h>

#if defined(STDC_HEADERS)
#include <string.h>
#include <stdlib.h>
#elif defined(HAVE_STRING_H)
#include <string.h>
#elif defined(HAVE_STRINGS_H)
#include <strings.h>
#else /* no string(s) header file */
extern char *strsep();
#endif /* STDC_HEADERS */

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifndef STDC_HEADERS
extern long atol();
extern char *strsep();
#endif /* not STDC_HEADERS */

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "lib/error.h"
#include "mydbm.h"
#include "db_storage.h"

/* Delete an entry for a page.
   Again, 3 possibilities:

   1) page is singular reference, just delete it :)
   2) page has 2+ companions. Delete page and alter multi entry to not 
      point to it anymore.
   3) page has 1 companion. Could do as (2), but we'd waste an entry in 
      the db. Should delete page, extract friend and reinsert as singular,
      overwriting the old multi entry.
*/

#define NO_ENTRY	1;

int dbdelete(char *name, struct mandata *info)
{
	datum key, cont;

	/* get entry for info */

	if (debug)
		fprintf(stderr, "Attempting delete of %s(%s) entry.\n", 
			name, info->ext);

	key.dptr = name;
	key.dsize = strlen(key.dptr) + 1;
	cont = MYDBM_FETCH(dbf, key);

	if (!cont.dptr) {			/* 0 entries */
		return NO_ENTRY;
	} else if (*cont.dptr != '\t') {	/* 1 entry */
		MYDBM_DELETE(dbf, key);
		MYDBM_FREE(cont.dptr);
	} else {				/* 2+ entries */
		char *ext[ENTRIES], **e;
		char *multi_content = NULL;
		datum multi_key;
		int refs;

		/* Extract all of the extensions associated with 
		   this key */

		refs = list_extensions(cont.dptr + 1, e = ext);
		
		while (*e && strcmp(*e, info->ext) != 0)
			e++;

		if (!*e) {
			MYDBM_FREE(cont.dptr);
			return NO_ENTRY;
		}
			
		multi_key = make_multi_key(name, *e);
		if (!MYDBM_EXISTS(dbf, multi_key)) {
			error (0, 0,
			       _( "multi key %s does not exist"),
			       multi_key.dptr);
			gripe_corrupt_data();
		}
		MYDBM_DELETE(dbf, multi_key);
		free(multi_key.dptr);
		**e = '\0';

		/* refs *may* be 1 if all manual pages with this name 
		   have been deleted. In this case, we'll have to remove 
		   the key too */
		   
		if (refs == 1) {
			MYDBM_FREE(cont.dptr);
			MYDBM_DELETE(dbf, key);
			return 0;
		}
			 
		/* create our new multi content */
		for (e = ext; *e; e++) {
			if (**e)
				multi_content = strappend(multi_content,
							  "\t", *e, NULL);
		}

		MYDBM_FREE(cont.dptr);

		/* if refs = 2 do something else. Doesn't really matter as
		   the gdbm db file does not shrink any after a deletion
		   anyway */

		cont.dptr = multi_content;
		cont.dsize = strlen(cont.dptr) + 1;

		if (MYDBM_REPLACE(dbf, key, cont))
			gripe_replace_key(key.dptr);
	}
	return 0;
}
