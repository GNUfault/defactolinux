/*
 * db_lookup.c: low level database interface routines for man.
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
#include <errno.h>

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
extern int errno;
#endif /* not STDC_HEADERS */

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "lib/error.h"
#include "mydbm.h"
#include "db_storage.h"

/* If using ndbm or BTREE, copy the static storage before doing anything 
   interesting with it */
#if defined(NDBM) || defined (BTREE)
datum copy_datum (datum dat)
{
	if (dat.dptr) 
		dat.dptr = memcpy (xmalloc (dat.dsize), dat.dptr, dat.dsize);
	return dat;
}

void gripe_lock(char *filename)
{
	error (0, errno,
	       _( "can't lock index cache %s"),
	       filename);
}
#endif /* NDBM || BTREE */

/* issue fatal message, then exit */
void gripe_corrupt_data(void)
{
	error (FATAL, 0,
	       _( "index cache %s corrupt"),
	       database);
}

/* deal with situation where we cannot replace a key */
void gripe_replace_key(char *data)
{
	error (0, 0,
	       _( "cannot replace key %s"),
	       data);
	gripe_corrupt_data();
}

/* Just print out what would be stored in the db */
void dbprintf(struct mandata *info)
{
	fprintf(stderr,
		"sec. ext:  %s\n"
		"section:   %s\n"
		"comp. ext: %s\n"
		"id:        %c\n"
		"st_mtime   %ld\n"
		"pointer:   %s\n"
		"filter:    %s\n"
		"whatis:    %s\n\n",
		info->ext, info->sec, info->comp,
		info->id, (long)info->_st_mtime, 
		info->pointer, info->filter, info->whatis);
}

/* Form a multi-style key from page and extension info */
datum make_multi_key(char *page, char *ext)
{
	datum key;

	key.dsize = strlen(page) + strlen(ext) + 2;
	key.dptr = (char *) xmalloc (key.dsize);
	sprintf(key.dptr, "%s\t%s", page, ext);
	return key;
}

/* allocate a mandata structure */
struct mandata *infoalloc(void)
{
	return (struct mandata *) xmalloc (sizeof(struct mandata));
}

/* go through the linked list of structures, free()ing the `content' and the
   structs themselves */
void free_mandata_struct(struct mandata *pinfo)
{
	while(pinfo) {
		struct mandata *next;

		next = pinfo->next;
		if (pinfo->addr)
			free(pinfo->addr); 	/* free the `content' */
		free(pinfo);			/* free the structure */
		pinfo = next;
	}
}
 
/* return char ptr array to the data's fields */
char **split_data(char *content, char *start[])
{
        int count;

	/* initialise pointers to first N-1 fields */
	for (count = 0; count < FIELDS - 1 ; count++) {
		start[count] = strsep(&content, "\t");
		if (!start[count]) {
			error (0, 0,
			       _("only %d fields in content"),
			       count);
			gripe_corrupt_data();
		}
	}

	/* initialise pointer to Nth field (whatis) */
	start[FIELDS - 1] = content;
	if (!start[FIELDS - 1]) {
		error (0, 0,
		       _( "only %d fields in content"),
		       FIELDS - 1);
		gripe_corrupt_data();
	}
	
	return start;
}

/* Parse the db-returned data and put it into a mandata format */
void split_content(char *cont_ptr, struct mandata *pinfo)
{
	char *start[FIELDS];
	char **data;

	data = split_data(cont_ptr, start);

	pinfo->ext = *(data++);
	pinfo->sec = *(data++);
	pinfo->_st_mtime = (time_t) atol(*(data++));	/* time_t format */
	pinfo->id = **(data++);				/* single char id */
	pinfo->pointer = *(data++);
	pinfo->filter = *(data++);
	pinfo->comp = *(data++);
	pinfo->whatis = *(data);

	pinfo->addr = cont_ptr;
	pinfo->next = (struct mandata *) NULL;
}
	
/* The complement of split_content */
datum make_content(struct mandata *in)
{
	datum cont;
	static char dash[] = "-";

	if (!in->pointer)
		in->pointer = dash;
	if (!in->filter)
		in->filter = dash;
	if (!in->comp)
		in->comp = dash;
	if (!in->whatis)
		in->whatis = dash + 1;

	cont.dsize = strlen(in->ext) + 
	             strlen(in->sec) + 
	          /* strlen(in->_st_mtime) */ + 11 +
	          /* strlen(in->id) */ + 1 + 
	             strlen(in->pointer) + 
	             strlen(in->filter) + 
	             strlen(in->comp) + 
	             strlen(in->whatis) + 7;  
	cont.dptr = (char *) xmalloc (cont.dsize);
#ifdef ANSI_SPRINTF
	cont.dsize = 1 + sprintf(cont.dptr, "%s\t%s\t%ld\t%c\t%s\t%s\t%s\t%s", 
	        in->ext,
	        in->sec,
	        in->_st_mtime,
	        in->id, 
	        in->pointer, 
	        in->filter, 
	        in->comp, 
	        in->whatis);

	assert(strlen(cont.dptr) + 1 == cont.dsize);
#else /* !ANSI_SPRINTF */
	sprintf(cont.dptr, "%s\t%s\t%ld\t%c\t%s\t%s\t%s\t%s", 
	        in->ext,
	        in->sec,
	        (long)in->_st_mtime,
	        in->id, 
	        in->pointer, 
	        in->filter, 
	        in->comp, 
	        in->whatis);

	cont.dsize = strlen(cont.dptr) + 1;	/* to be sure (st_mtime) */
#endif /* ANSI_SPRINTF */

#ifdef NDBM
	/* limit of 4096 bytes of data using ndbm */
	if (cont.dsize > 4095) {
		cont.dptr[4095] = '\0';
		cont.dsize = 4096;
	}
#endif
	return cont;
}

/* Extract all of the extensions associated with this key */
int list_extensions(char *data, char *ext[])
{  
	int count = 0;

	while ( (ext[count] = strsep(&data, "\t")) )
		count++;

	if (debug)
		fprintf(stderr, "found %d extensions\n", count);
	return count;
}

#define	EXACT	1
#define ALL	0

/*
 There are three possibilities on lookup:

 1) No data exists, lookup will fail, returned structure will be NULL.
 2) One data item exists. Item is returned as first in set of structures.
 3) Many items exist. They are all returned, in a multiple structure set.
 */
#ifndef FAST_BTREE
static struct mandata *dblookup(char *page, char *section, int flags)
{
	struct mandata *info = NULL;
	datum key, cont;

	key.dptr = page;
	key.dsize = strlen(page) + 1;
	cont = MYDBM_FETCH(dbf, key);

	if (cont.dptr == NULL) {		/* No entries at all */
		return info;			/* indicate no entries */
	} else if (*cont.dptr != '\t') {	/* Just one entry */
		info = infoalloc();
		split_content(cont.dptr, info);
		if (section == NULL || 
		      strncmp(section, info->ext, 
		              flags & EXACT ? strlen(info->ext) : 
		                              strlen(section)) == 0) {
		      	return info;
		}
		free_mandata_struct(info);
		return NULL;
	} else {				/* multiple entries */
		char *ext[ENTRIES], **e;
		struct mandata *ret = NULL;

		/* Extract all of the extensions associated with this key */

		(void) list_extensions(cont.dptr + 1, e = ext);

		/* Make the multi keys and look them up */
		
		while (*e) {
			if (section == NULL || 
			    strncmp(section, *e, 
			            flags & EXACT ? strlen(*e)
			                          : strlen(section)) == 0) {
		                datum multi_cont;
			                
			  	key = make_multi_key(page, *e);
			  	if (debug)
			  		fprintf(stderr, 
			  			"multi key lookup (%s)\n", 
			  		        key.dptr);
				multi_cont = MYDBM_FETCH(dbf, key);
				if (multi_cont.dptr == NULL) {
					error (0, 0,
					       _(
						       "bad fetch on multi key %s"),
					       key.dptr);
					gripe_corrupt_data();
				}
				free(key.dptr);
					
				/* allocate info struct, fill it in and
				   point info to the next in the list */
				if (!ret)
					ret = info = infoalloc();
				else
					info = info->next = infoalloc();
				split_content(multi_cont.dptr, info);
			}
			e++;
		}
		MYDBM_FREE(cont.dptr);
		return ret;
	}
}
#endif /* !FAST_BTREE */

struct mandata *dblookup_all(char *page, char *section)
{
	return dblookup(page, section, ALL);
}

struct mandata *dblookup_exact(char *page, char *section)
{
	return dblookup(page, section, EXACT);
}
