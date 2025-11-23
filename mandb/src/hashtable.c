/*
 * hashtable.c: in core hash table routines.
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * All of these routines except free_hashtable() can be found in K&R II
 *
 * Sat Aug 20 15:01:02 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

/* which hash function do we want ? */
/* #define PROLOGUE */
#define KRII 

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>	/* only for printf() */

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
#endif /* STDC_HEADERS */

#include "manconfig.h"
#include "hashtable.h"

#if defined(PROLOGUE)
#define HASHSIZE 2048
#elif defined(KRII)
#define HASHSIZE 2001
#else
#error hash function not defined
#endif

static struct nlist *hashtab[HASHSIZE];		/* The storage array */
static int unique;				/* unique hash values */
static int identical;				/* identical hash values */

/* return hash value for string */
static unsigned hash (char *s)
{
	unsigned hashval = 0;

	while (*s)
#if defined(KRII)
		hashval = *s++ + 31 * hashval;
	return hashval % HASHSIZE;
#elif defined(PROLOGUE)
		hashval = (hashval << 1) + *s++;
	return hashval & (HASHSIZE - 1);
#endif
}

/* return pointer to structure containing defn, else NULL if it 
   doesn't exist */
struct nlist *lookup (char *s)
{
	struct nlist *np;
	
	for (np = hashtab[hash(s)]; np; np = np->next) {
		if (strcmp (s, np->name) == 0)
			return np;
	}
	return NULL;
}

/* free the defn ptr's contents (if not NULL) */
static __inline__ void free_defn (struct nlist *np)
{
	if (np->defn) {
		if (np->is_text)
			free (np->defn);
	 	else
			free_mandata_struct (np->defn);
	}
}
	
/* return struct containing defn or NULL if unable to store */
static struct nlist *install (char *name, void *defn, int flag)
{
	struct nlist *np;

	np = lookup (name);
	if (np == NULL) {
		unsigned hashval;

		np = (struct nlist *) xmalloc (sizeof (struct nlist));
		np->name = xstrdup (name);
		hashval = hash (name);

		/* record uniqueness if debugging */
		if (debug) {
			if (hashtab[hashval])
				identical++;
			else
				unique++;
		}
		
		/* point to last w/ this hash */
		np->next = hashtab[hashval];

		/* attach to hashtab array */
		hashtab[hashval] = np;
	} else
		free_defn (np);

	np->defn = defn;
	np->is_text = flag;

	return np;
}

/* special wrapper to insert a string or NULL */
struct nlist *install_text (char *name, char *text)
{
	return install (name, text ? xstrdup(text) : text, 1);
}

/* special wrapper to insert a ptr to in core mandata struct linked list */
struct nlist *install_db_ptr (char *name, struct mandata *db_ptr)
{
	return install (name, db_ptr, 0);
}

/* free up the hash tree (garbage collect), also free up any xstrdup()'d text
   or mandata structures */
void free_hashtab (void)
{
	int i;

	if (debug) {
		fprintf (stderr,
			 "free_hashtab: %d entries, %d (%d%%) unique\n", 
		         unique + identical,
		         unique,
			 unique ? (unique * 100) / (unique + identical) : 0);
		unique = identical = 0;
	}

	for (i = 0; i < HASHSIZE; i++) {
		struct nlist *np;
		
		np = hashtab[i];
		while (np) {
			struct nlist *next;

			free_defn (np);
			free (np->name);
			next = np->next;
			free (np);
			np = next;
		}
		hashtab[i] = NULL;
	}
}
