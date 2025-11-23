/*
 * db_store.c: dbstore(), database storage routine.
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
#  include "config.h"
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

/* deal with situation where we cannot insert an unused key */
static void gripe_insert_unused(char *data)
{
	error (0, 0,
	       _( "cannot insert unused key %s"),
	       data);
	gripe_corrupt_data();
}

/* FAVOUR(a,b) is true if id 'a' is preferred to id 'b', i.e. if 'a' is a
 * more canonical database entry than 'b'. This usually goes in comparison
 * order, but there's a special exception when FAVOUR_STRAYCATS is set.
 */
#ifdef FAVOUR_STRAYCATS
#define FAVOUR(a,b) ((a < b && !(a == WHATIS_MAN && b == STRAY_CAT)) || \
		     (a == STRAY_CAT && b == WHATIS_MAN))
#else /* !FAVOUR_STRAYCATS */
#define FAVOUR(a,b) (a < b)
#endif /* FAVOUR_STRAYCATS */

/* The do_we_replace logic */
static int replace_if_necessary(struct mandata *in, struct mandata *info, 
				datum newkey, datum newcont)
{
	if (FAVOUR(in->id, info->id)) {
		if (MYDBM_REPLACE(dbf, newkey, newcont))
			gripe_replace_key(newkey.dptr);
	} else if (in->id == info->id) 
		if (debug)
			fprintf(stderr,
				"ignoring identical multi key: %s\n",
				newkey.dptr);
			
	if (in->id == info->id &&
	    in->id == ULT_MAN) {
		if (strcmp(in->comp ? in->comp : "-", info->comp) == 0) {
			if (in->_st_mtime != info->_st_mtime) {
				if (debug)
					fprintf(stderr, "replace_if_necessary(): replace\n");
				if (MYDBM_REPLACE(dbf, newkey, newcont))
					gripe_replace_key(newkey.dptr);
			}
			return 0; /* same file */
		} else
			return 1; /* differing ext's */
	}

	return 0; /* not physical file anyway */
}

/*
 Any one of three situations can occur when storing some data.
 
 1) no simple key is found.
 	store as singular reference.
 2) simple key already exists, content starts with a '\t'.
 	Already multiple reference. Add our new item in multiple format
 	and update the simple key content, to point to our new one also.
 3) simple key already exists, content does not start with a '\t'.
 	First we have to reformat the simple key into a multi key for the
 	old item, and insert. Then we have to insert the new data as a 
 	multi key. Lastly we must create the simple key and do a replace 
 	on it.

 Use precedence algorithm on inserts. If we already have a key assigned
 to the new value, check priority of page using id. If new page is higher
 (lower value), replace old with new, otherwise ignore new page.

 If we have two ULT_MAN pages competing for the same key, we must have
 more than one of foo.sec, foo.sec.comp1, foo.sec.comp2. OR we have a 
 replacement page. If the st_mtimes differ, throw out the old struct and
 replace it with the new, if the comp exts differ, oops, this is bad, 
 keep one and return appropriate error code.

 If we have two WHATIS_MAN pages or a WHATIS_MAN and a SO_MAN page
 competing for the same key, don't worry. This will happen a lot and is 
 not a problem.
 
 return errorcode or 0 on success.
*/
#ifndef FAST_BTREE
int dbstore(struct mandata *in, char *basename)
{
	datum oldkey, oldcont;

	/* create a simple key */
 	oldkey.dsize = strlen(basename) + 1;

 	if (oldkey.dsize == 1) {
 		if (debug)
 			dbprintf(in);
 		return 2;
 	}

	oldkey.dptr = basename;

	/* get the content for the simple key */
	
	oldcont = MYDBM_FETCH(dbf, oldkey);

	if (oldcont.dptr == NULL) { 		/* situation (1) */
		oldcont = make_content(in);
		if (MYDBM_INSERT(dbf, oldkey, oldcont))
			gripe_insert_unused(oldkey.dptr);
		free(oldcont.dptr);
	} else if (*oldcont.dptr == '\t') { 	/* situation (2) */
		datum newkey, newcont;

		newkey = make_multi_key(oldkey.dptr, in->ext);
		newcont = make_content(in);

		/* Try to insert the new multi data */
		
		if (MYDBM_INSERT(dbf, newkey, newcont)) {
			datum cont;
			struct mandata info;
			int ret;
			
			MYDBM_FREE(oldcont.dptr);
			cont = MYDBM_FETCH(dbf, newkey);
			split_content(cont.dptr, &info);
			ret = replace_if_necessary(in, &info, newkey, newcont);
			/* MYDBM_FREE(cont.dptr); */
			free(info.addr);
			free(newkey.dptr);
			free(newcont.dptr);

			return ret;
		}

		/* Now lets add some info to the simple key's cont. */

		/* This next bit needs to be done first as we'll wipe out
		   oldcont.dptr otherwise (for NDBM only!) */

		free(newkey.dptr);
		free(newcont.dptr);
		
		newcont.dsize = oldcont.dsize + strlen(in->ext) + 1;
		newcont.dptr = (char *) xmalloc(newcont.dsize);

		sprintf(newcont.dptr, "%s\t%s", oldcont.dptr, in->ext);
		MYDBM_FREE(oldcont.dptr);

		/* Try to replace the old simple data with the new stuff */
		
		if (MYDBM_REPLACE(dbf, oldkey, newcont))
			gripe_replace_key(oldkey.dptr);

		free(newcont.dptr);
	} else { 				/* situation (3) */
		datum newkey, newcont, lastkey, lastcont; 
		struct mandata old;

		/* Extract the old singular reference */

		split_content(oldcont.dptr, &old);

		/* Create multi keys for both 
		   and new items, create new content */

		lastkey = make_multi_key(oldkey.dptr, old.ext);
		newkey = make_multi_key(oldkey.dptr, in->ext);
		newcont = make_content(in);

		/* Check against identical multi keys before inserting
		   into db */

		if (strcmp(lastkey.dptr, newkey.dptr) == 0) {
			int ret;

			ret = replace_if_necessary(in, &old, oldkey, newcont);
			/* MYDBM_FREE(oldcont.dptr); */
			free(old.addr);
			free(newcont.dptr);
			free(newkey.dptr);
			free(lastkey.dptr);

			return ret;
		}

		lastcont = make_content(&old);	

		if (MYDBM_INSERT(dbf, lastkey, lastcont))
			gripe_insert_unused(lastkey.dptr);

		free(lastkey.dptr);
		free(lastcont.dptr);

		if (MYDBM_INSERT(dbf, newkey, newcont))
			gripe_insert_unused(newkey.dptr);

		free(newkey.dptr);
		free(newcont.dptr);

		/* Now build a simple reference to the above two items */

		newcont.dsize = strlen(old.ext) + strlen(in->ext) + 3;
		newcont.dptr = (char *) xmalloc (newcont.dsize);
		sprintf(newcont.dptr, "\t%s\t%s", old.ext, in->ext);

		if (MYDBM_REPLACE(dbf, oldkey, newcont))
			gripe_replace_key(oldkey.dptr);

		/* MYDBM_FREE(oldcont.dptr); */
		free(old.addr);
		free(newcont.dptr);
	}
	return 0;
}
#endif /* !FAST_BTREE */
