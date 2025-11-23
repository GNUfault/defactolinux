/*
 * check_mandirs.c: used to auto-update the database caches
 *
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 * Copyright (c) 2001 Colin Watson.
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Mon May  2 17:36:33 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk)
 *
 * CJW: Many changes to whatis parsing. Added database purging.
 * See docs/ChangeLog for details.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#else /* not HAVE_DIRENT_H */
#  define dirent direct
#  ifdef HAVE_SYS_NDIR_H
#    include <sys/ndir.h>
#  endif /* HAVE_SYS_NDIR_H */
#  ifdef HAVE_SYS_DIR_H
#    include <sys/dir.h>
#  endif /* HAVE_SYS_DIR_H */
#  ifdef HAVE_NDIR_H
#    include <ndir.h>
#  endif /* HAVE_NDIR_H */
#endif /* HAVE_DIRENT_H  */

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strchr(), *strrchr(), *strstr(), *strpbrk();
#endif /* no string(s) header */

#ifndef STDC_HEADERS
extern time_t time();
extern int errno;
#endif

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "libdb/mydbm.h"
#include "libdb/db_storage.h"
#include "lib/error.h"
#include "globbing.h"
#include "ult_src.h"
#include "hashtable.h"
#include "security.h"
#include "check_mandirs.h"

int opt_test;		/* don't update db */
int pages;
int force_rescan = 0;

static void gripe_bogus_manpage (char *manpage)
{
	if (quiet < 2)
		error (0, 0, _("warning: %s: ignoring bogus filename"),
		       manpage);
}	  	  

static void gripe_multi_extensions (const char *path, const char *sec, 
				    const char *name, const char *ext)
{
	if (quiet < 2)
		error (0, 0,
		       _("warning: %s/man%s/%s.%s*: competing extensions"),
		       path, sec, name, ext);
}

static void gripe_rwopen_failed (char *database)
{
	if (errno == EACCES || errno == EROFS) {
		if (debug)
			fprintf (stderr, "database %s is read-only\n",
				 database);
	} else {
#ifdef MAN_DB_UPDATES
		if (!quiet)
#endif /* MAN_DB_UPDATES */
			error (0, errno, _("can't update index cache %s"),
			       database);
	}
}

char *make_filename (const char *path, const char *name, 
		     struct mandata *in, char *type)
{
	static char *file;
	
	file = (char *) xrealloc (file, sizeof "//." + strlen (path) + 
				  strlen (type) + strlen (in->sec) +
				  strlen (name) + strlen (in->ext));
				   
	(void) sprintf (file, "%s/%s%s/%s.%s",
			path, type, in->sec, name, in->ext);

	if (*in->comp != '-')	/* Is there an extension ? */
		file = strappend (file, ".", in->comp, NULL);

	return file;
}

int splitline (char *raw_whatis, struct mandata *info, char *base_name)
{
	char *pointer_name;
	char *comma;
	int ret;

	info->whatis = NULL;	/* default */
	if (raw_whatis) {
		info->whatis = strstr (raw_whatis, " - ");
		if (info->whatis) {
			char *space = info->whatis;
			while (*space == ' ')
				*space-- = '\0';    /* separate description */
			info->whatis += 3;
			/* Now trim trailing spaces off the description. */
			space = strchr (info->whatis, '\0') - 1;
			while (*space == ' ')
				*space = '\0';
		} else
			raw_whatis = NULL; /* kill entire whatis line */
	}

	/* Here we store the direct reference */
	if (debug)
		fprintf (stderr, "base_name = `%s', id = %c\n",
			 base_name, info->id);

	pointer_name = xstrdup (base_name);
	comma = strchr (pointer_name, ',');
	if (comma) {
		*comma = '\0';
		if (debug)
			fprintf (stderr, "pointer_name = `%s'\n",
				 pointer_name);
	}

	ret = dbstore (info, pointer_name);
	if (ret > 0) {
		free (pointer_name);
		return ret;
	}

	/* if there are no indirect references, just go on to the 
	   next file */

	if (!raw_whatis || strchr (raw_whatis, ',') == NULL) {
		free (pointer_name);
		return 0;
	}

	/* If there are...  */
		
	if (info->id < STRAY_CAT)
		info->id = WHATIS_MAN;
	else
		info->id = WHATIS_CAT;

	/* don't waste space storing the whatis in the db */
	info->whatis = NULL;
	info->pointer = pointer_name; 
	
	while ((comma = strrchr (raw_whatis, ',')) != NULL) {
		*comma = '\0';
		comma += 2;

		/* If we've already dealt with it, ignore */
		
		if (strcmp (comma, pointer_name) != 0) {
			if (debug)
				fprintf (stderr, "comma = `%s'\n", comma);
			ret = dbstore (info, comma);
			if (ret > 0) {
				free (pointer_name);
				return ret;
			}
		}
	}

	/* If we've already dealt with it, ignore */
		
	if (strcmp (raw_whatis, pointer_name) == 0) {
		free (pointer_name);
		return 0;
	}
		
	if (debug)
		fprintf (stderr, "raw_w = `%s'\n", raw_whatis);
	ret = dbstore (info, raw_whatis);
	if (ret > 0) {
		free (pointer_name);
		return ret;
	}

	return 0;
}

/* Fill in a mandata structure with information about a file name.
 * file is the name to examine. info points to the structure to be filled
 * in.
 * 
 * Returns either a pointer to the buffer which the fields in info point
 * into, to be freed by the caller, or NULL on error. The buffer will
 * contain either three or four null-terminated strings: the directory name,
 * the base of the file name in that directory, the section extension, and
 * optionally the compression extension (if COMP_SRC is defined).
 * 
 * Only the fields ext, sec, and comp are filled in by this function.
 */
char *filename_info (char *file, struct mandata *info)
{
	char *manpage = xstrdup (file);
	char *base_name = basename (manpage);
#ifdef COMP_SRC
	struct compression *comp;
#endif

	/* Bogus files either have (i) no period, ie no extension, (ii)
	   a compression extension, but no sectional extension, (iii)
	   a missmatch between the section they are under and the
	   sectional part of their extension. */

#ifdef COMP_SRC
	comp = comp_info (base_name);
	if (comp) {
		info->comp = comp->ext;
		*(comp->file) = '\0';		/* to strip the comp ext */
	} else
		info->comp = NULL;
#else /* !COMP_SRC */	
	info->comp = NULL;
#endif /* COMP_SRC */

	info->ext = strrchr (base_name, '.');
	if (!info->ext) {
		/* no section extension */
		gripe_bogus_manpage (file);
		free (manpage);
		return NULL;
	}

	*(info->ext++) = '\0';			/* set section ext */
	*(base_name - 1) = '\0';		/* strip '/base_name' */ 
	info->sec = strrchr (manpage, '/') + 4;	/* set section name */

	if (strncmp (info->sec, info->ext, strlen (info->sec)) != 0) {
		/* missmatch in extension */
		gripe_bogus_manpage (file);
		free (manpage);
		return NULL;
	}

	return manpage;
}

/* take absolute filename and path (for ult_src) and do sanity checks on 
   file. Also check that file is non-zero in length and is not already in
   the db. If not, find its ult_src() and see if we have the whatis cached, 
   otherwise cache it in case we trace another manpage back to it. Next,
   store it in the db along with any references found in the whatis. */
void test_manfile (char *file, const char *path)
{
	char *base_name, *ult;
	struct lexgrog lg;
	char *manpage;
	struct mandata info, *exists;
	struct nlist *in_cache;
	struct stat buf;
	size_t len;

	memset (&lg, '\0', sizeof (struct lexgrog));

	manpage = filename_info (file, &info);
	if (!manpage)
		return;
	base_name = manpage + strlen (manpage) + 1;

	len  = strlen (manpage) + 1;		/* skip over directory name */
	len += strlen (manpage + len) + 1;	/* skip over base name */
	len += strlen (manpage + len);		/* skip over section ext */

	/* to get mtime info */
	(void) lstat (file, &buf);
	info._st_mtime = buf.st_mtime;
	
	/* check that our file actually contains some data */
	if (buf.st_size == 0) {
		/* man_db pre 2.3 place holder ? */
		free (manpage);
		return;
	}

	/* see if we already have it, before going any further, this will
	   save both an ult_src() a find_name(), amongst other time wastes */
	exists = dblookup_exact (base_name, info.ext);

	/* Ensure we really have the actual page. Gzip keeps the mtime
	   the same when it compresses, so we have to compare comp 
	   extensions also */

	if (exists) {
		if (strcmp (exists->comp, info.comp ? info.comp : "-") == 0) {
			if (exists->_st_mtime == info._st_mtime 
			    && exists->id < WHATIS_MAN) {
				free_mandata_struct (exists);
				free (manpage);
				return;
			}
		} else {
			struct stat physical;
			char *abs_filename;
			
			/* see if the cached file actually exists. It's 
			   evident at this point that we have multiple 
			   comp extensions */
			abs_filename = make_filename (path, base_name,
						      exists, "man");
			if (debug)
				fprintf (stderr, "test_manfile(): stat %s\n",
					 abs_filename);
			if (stat (abs_filename, &physical) == -1) {
				if (!opt_test)
					dbdelete (base_name, exists);
			} else {
				gripe_multi_extensions (path, exists->sec,
							base_name, exists->ext);
				free_mandata_struct (exists);
				free (manpage);
				return;
			}
		}
		free_mandata_struct (exists);
	}

	/* Check if it happens to be a symlink/hardlink to something already
	   in our cache. This just does some extra checks to avoid scanning
	   links quite so many times. */
	{
		/* Avoid too much noise in debug output */
		int save_debug = debug;
		debug = 0;
		ult = ult_src (file, path, &buf, SOFT_LINK | HARD_LINK);
		debug = save_debug;
	}

	if (!ult) {
		/* already warned about this, don't do so again */
		if (debug)
			fprintf (stderr,
				 "test_manfile(): bad link %s\n", file);
		free (manpage);
		return;
	}

	if (lookup (ult) == NULL) {
		if (debug && strncmp (ult, file, len) != 0)
			fprintf (stderr,
				 "\ntest_manfile(): link not in cache:\n"
				 " source = %s\n"
				 " target = %s\n", file, ult);
		/* Trace the file to its ultimate source, else we'll be
		   looking for whatis info in files containing only
		   '.so manx/foo.x', which will give us an unobtainable
		   whatis for the entry. */
		ult = ult_src (file, path, &buf,
			       SO_LINK | SOFT_LINK | HARD_LINK);
	}

	if (!ult) {
		error (0, 0,
		       _("warning: %s: bad symlink or ROFF `.so' request"),
		       file);
		free (manpage);
		return;
	}

	pages++;			/* pages seen so far */

	if (strncmp (ult, file, len) == 0)
		info.id = ULT_MAN;	/* ultimate source file */
	else
		info.id = SO_MAN;	/* .so, sym or hard linked file */

	/* Ok, here goes: Use a hash tree to store the ult_srcs with
	   their whatis. Anytime after, check the hash tree, if it's there, 
	   use it. This saves us a find_name() which is a real hog */

	/* could use strrchr(ult, '/') + 1 as hash text, but not worth it */

	in_cache = lookup (ult);

	if (in_cache) {		/* cache hit */
		lg.whatis = in_cache->defn ? xstrdup (in_cache->defn) : NULL;
	} else {		/* cache miss */
		/* go get the whatis info in its raw state */
#ifdef COMP_SRC
		/* if the nroff was compressed, an uncompressed version is
		   shown by a call to get_ztemp(), grog this for a whatis
		   rather than ult. This is a bit difficult to follow, sorry:
		   ult_src() will leave the last uncompressed nroff file it
		   has to deal with in get_ztemp() */
		char *ztemp;
#endif /* COMP_SRC */

		lg.type = MANPAGE;
		drop_effective_privs ();
#ifdef COMP_SRC
		ztemp = get_ztemp ();
		if (ztemp) {
			find_name (ztemp, basename (file), &lg);
			remove_ztemp ();  /* get rid of temp file identifier */
		} else
#endif /* COMP_SRC */
			find_name (ult, basename (file), &lg);
		regain_effective_privs ();
			
		install_text (ult, lg.whatis);
	}

	if (debug)
		fprintf (stderr, "\"%s\"\n", lg.whatis);

	/* split up the raw whatis data and store references */
	info.pointer = NULL;	/* direct page, so far */
	info.filter = lg.filters;
	if (lg.whatis) {
		int last_name;
		char save_id;

		last_name = 0;
		save_id = info.id;

		/* It's easier to run through the names in reverse order. */
		while (!last_name) {
			char *sep, *othername, *end_othername;
			/* Get the next name, with leading spaces and the
			 * description removed.
			 */
			sep = strrchr (lg.whatis, 0x11);
			if (sep)
				*(sep++) = '\0';
			else {
				sep = lg.whatis;
				last_name = 1;
			}
			if (!*sep)
				/* Probably a double line break or something */
				continue;
			sep += strspn (sep, " ");
			othername = xstrdup (sep);
			end_othername = strstr (othername, " - ");
			if (end_othername) {
				while (*(end_othername - 1) == ' ')
					--end_othername;
				*end_othername = '\0';
			}
			if (STREQ (base_name, othername))
				info.id = save_id;
			else {
				info.id = WHATIS_MAN;
				info.pointer = base_name;
			}
			if (!opt_test) {
				char *dup_whatis = xstrdup (sep);
				if (splitline (dup_whatis, &info,
					       othername) == 1)
					gripe_multi_extensions (path, info.sec,
								base_name,
								info.ext);
				free (dup_whatis);
			}
			free (othername);
		}

		info.id = save_id;
		info.pointer = NULL;
		if (!opt_test)
			if (splitline (lg.whatis, &info, base_name) == 1)
				gripe_multi_extensions (path, info.sec,
							base_name, info.ext);
	} else {
		(void) stat (ult, &buf);
		if (buf.st_size == 0) {
			if (quiet < 2)
				error (0, 0,
				       _("warning: %s: ignoring empty file"),
				       ult);
			free (manpage);
			return;
		}
		if (quiet < 2)
			error (0, 0,
			       _("warning: %s: whatis parse for %s(%s) failed"),
			       ult, base_name, info.ext);
	}

	free (manpage);
	if (lg.whatis)
		free (lg.whatis);
}

static __inline__ void add_dir_entries (const char *path, char *infile)
{
	char *manpage;
	int len;
	struct dirent *newdir;
	DIR *dir;

	manpage = strappend (NULL, path, "/", infile, "/", NULL);
	len = strlen (manpage);

	/*
	 * All filename entries in this dir should either be valid manpages
	 * or . files (such as current, parent dir).
	 */

	dir = opendir (infile);
	if (!dir) {
		error (0, errno, _("can't search directory %s"), manpage);
		free (manpage);
                return;
        }
        
        /* strlen(newdir->d_name) could be replaced by newdir->d_reclen */
        
	while ( (newdir = readdir (dir)) )
		if (!(*newdir->d_name == '.' && 
		      strlen (newdir->d_name) < (size_t) 3)) {
			manpage = strappend (manpage, newdir->d_name, NULL);
			test_manfile (manpage, path);
			*(manpage + len) = '\0';
		}
		
	free (manpage);
	closedir (dir);
}

/*
 * accepts the raw man dir tree eg. "/usr/man" and the time stored in the db
 * any dirs of the tree that have been modified (ie added to) will then be
 * scanned for new files, which are then added to the db.
 */
static short testmandirs (const char *path, time_t last)
{
	DIR *dir;
	struct dirent *mandir;
	struct stat stbuf;
	short amount = 0;

	if (debug)
		fprintf (stderr, "Testing %s for new files\n", path);

	dir = opendir (path);
	if (!dir) {
		error (0, errno, _("can't search directory %s"), path);
		return 0;
	}

	chdir (path);

	while( (mandir = readdir (dir)) ) {
		if (strncmp (mandir->d_name, "man", 3) != 0)
			continue;

		if (stat (mandir->d_name, &stbuf) != 0)	/* stat failed */
			continue;
		if (!S_ISDIR(stbuf.st_mode))		/* not a directory */
			continue;
		if (!force_rescan && stbuf.st_mtime <= last)
			/* scanned already */
			continue;

		if (debug)
			fprintf (stderr,
			  "\tsubdirectory %s has been 'modified'\n",
			  mandir->d_name);

		dbf = MYDBM_RWOPEN(database);

		if (!dbf) {
			gripe_rwopen_failed (database);
			return 0;
		}

		if (!quiet) {
			fprintf (stderr, "\r");
			fprintf (stderr,
				 _("Updating index cache for path "
				   "`%s'. Wait..."), path);
		}
		add_dir_entries (path, mandir->d_name);
		MYDBM_CLOSE (dbf);
		amount++;
	}
	closedir (dir);

	/* clean out the whatis hashtable for new hierarchy */
	if (amount > 0)
		free_hashtab ();

	return amount;
}

/* update the time key stored within `database' */
void update_db_time (void)
{
	datum key, content;
#ifdef FAST_BTREE
	datum key1, content1;
#endif /* FAST_BTREE */

	key.dptr = KEY;
	key.dsize = sizeof KEY;
	content.dptr = (char *) xmalloc (16); /* 11 is max long with '\0' */
	(void) sprintf (content.dptr, "%ld", (long) time (NULL));
	content.dsize = strlen (content.dptr) + 1;

	/* Open the db in RW to store the $mtime$ ID */
	/* we know that this should succeed because we just updated the db! */
	dbf = MYDBM_RWOPEN (database);
	if (dbf == NULL) {
#ifdef MAN_DB_UPDATES
		if (!quiet)
#endif /* MAN_DB_UPDATES */
			error (0, errno, _("can't update index cache %s"),
			       database);
		free (content.dptr);
		return;
	}
#ifndef FAST_BTREE
	MYDBM_REPLACE (dbf, key, content);
#else /* FAST_BTREE */
	key1.dptr = KEY;
	key1.dsize = sizeof KEY;

	(dbf->seq) (dbf, (DBT *) &key1, (DBT *) &content1, R_CURSOR);
	
	if (strcmp (key1.dptr, key.dptr) == 0)
		(dbf->put) (dbf, (DBT *) &key, (DBT *) &content, R_CURSOR);
	else
		(dbf->put) (dbf, (DBT *) &key, (DBT *) &content, 0);
#endif /* !FAST_BTREE */

	MYDBM_CLOSE (dbf);
	free (content.dptr);
}

/* remove the db's time key - called prior to update_db if we want
   to `force' a full consistency check */
void reset_db_time (void)
{
	datum key;

	key.dptr = KEY;
	key.dsize = sizeof KEY;

	/* we don't really care if we can't open it RW - it's not fatal */
	dbf = MYDBM_RWOPEN (database);
	if (dbf == NULL) {
		if (debug) {
			fprintf (stderr, "reset_db_time(): ");
			perror ("can't open db");
		}
		return;
	}

	MYDBM_DELETE (dbf, key);
	if (debug)
		fprintf (stderr, "reset_db_time()\n");
	MYDBM_CLOSE (dbf);
}

/* routine to prepare/create the db prior to calling testmandirs() */
short create_db (const char *manpath)
{
	short amount;
	
	if (debug)
		fprintf (stderr, "create_db(%s): %s\n", manpath, database);

	/* Open the db in CTRW mode to store the $ver$ ID */

	dbf = MYDBM_CTRWOPEN (database);
	if (dbf == NULL) {
		if (errno == EACCES || errno == EROFS) {
			if (debug)
				fprintf (stderr, "database %s is read-only\n",
					 database);
		} else
			error (0, errno, _("can't create index cache %s"),
			       database);
		return 0;
		/* should really return EOF */
	}

	dbver_wr (dbf);
	MYDBM_CLOSE (dbf);

	amount = testmandirs (manpath, (time_t) 0);

	if (amount) {
		update_db_time ();
		if (!quiet)
			fputs (_("done.\n"), stderr);
	}

	return amount;
}

/* routine to update the db, ensure that it is consistent with the 
   filesystem */
short update_db (const char *manpath)
{
	dbf = MYDBM_RDOPEN (database);
	if (dbf && dbver_rd (dbf)) {
		MYDBM_CLOSE (dbf);
		dbf = NULL;
	}
	if (dbf) {
		datum key, content;
		short new;

		key.dptr = KEY;
		key.dsize = sizeof KEY;
		content = MYDBM_FETCH (dbf, key);
		MYDBM_CLOSE (dbf);

		if (debug)
			fprintf (stderr, "update_db(): %ld\n",
				 content.dptr ? atol (content.dptr) : 0L);
		if (content.dptr) {
			new = testmandirs (manpath,
					   (time_t) atol (content.dptr));
			MYDBM_FREE (content.dptr);
		} else
			new = testmandirs (manpath, (time_t) 0);

		if (new) {
			update_db_time ();
			if (!quiet)
				fputs (_("done.\n"), stderr);
		}
		
		return new;
	}
		
	if (debug)
		fprintf (stderr, "failed to open %s O_RDONLY\n", database);
		
	return EOF;
}

/* Decide whether to purge a reference to a "normal" (ULT_MAN or SO_MAN)
 * page.
 */
static __inline__ short purge_normal (char *name, struct mandata *info,
				      char **found)
{
	if (found)
		return 0;

	if (!opt_test)
		dbdelete (name, info);
	else if (debug)
		fprintf (stderr, "%s(%s): missing page, would delete\n",
			 name, info->ext);

	return 1;
}

/* Decide whether to purge a reference to a WHATIS_MAN page. */
static __inline__ short purge_whatis (const char *manpath, char *name,
				      struct mandata *info, char **found)
{
	if (found) {
		/* If the page exists and didn't beforehand, then presumably
		 * we're about to rescan, which will replace the WHATIS_MAN
		 * entry with something better. However, there have been
		 * bugs that created false WHATIS_MAN entries, so force the
		 * rescan just to be sure; since in the absence of a bug we
		 * would rescan anyway, this isn't a problem.
		 */
		if (debug && !force_rescan)
			fprintf (stderr,
				 "%s(%s): whatis replaced by real page; "
				 "forcing a rescan just in case\n",
				 name, info->ext);
		force_rescan = 1;
		return 0;
	} else if (*info->pointer == '-') {
		/* This is broken; a WHATIS_MAN should never have an empty
		 * pointer field. This might have happened due to the first
		 * name in a page being different from what the file name
		 * says; that's fixed now, so delete and force a rescan.
		 */
		if (!opt_test)
			dbdelete (name, info);
		else if (debug)
			fprintf (stderr,
				 "%s(%s): whatis with empty pointer, "
				 "would delete\n",
				 name, info->ext);

		if (debug && !force_rescan)
			fprintf (stderr,
				 "%s(%s): whatis had empty pointer; "
				 "forcing a rescan just in case\n",
				 name, info->ext);
		force_rescan = 1;
		return 1;
	} else {
		/* Does the real page still exist? */
		char **real_found;
		int save_debug = debug;
		debug = 0;
		real_found = look_for_file (manpath, info->ext,
					    info->pointer, 0);
		debug = save_debug;

		if (real_found)
			return 0;

		if (!opt_test)
			dbdelete (name, info);
		else if (debug)
			fprintf (stderr,
				 "%s(%s): whatis target was deleted, "
				 "would delete\n",
				 name, info->ext);
		return 1;
	}
}

/* Go through the database and purge references to man pages that no longer
 * exist.
 */
short purge_missing (const char *manpath)
{
	datum key;
	short count = 0;

	if (!quiet)
		printf (_("Purging old database entries in %s...\n"), manpath);

	dbf = MYDBM_RWOPEN (database);
	if (!dbf) {
		gripe_rwopen_failed (database);
		return 0;
	}

	key = MYDBM_FIRSTKEY (dbf);

	while (key.dptr != NULL) {
		datum content, nextkey;
		struct mandata entry;
		char *nicekey, *tab;
		int save_debug;
		char **found;

		/* Ignore db identifier keys. */
		if (*key.dptr == '$') {
			nextkey = MYDBM_NEXTKEY (dbf, key);
			MYDBM_FREE (key.dptr);
			key = nextkey;
			continue;
		}

		content = MYDBM_FETCH (dbf, key);
		if (!content.dptr)
			return count;

		/* Ignore overflow entries. */
		if (*content.dptr == '\t') {
			MYDBM_FREE (content.dptr);
			nextkey = MYDBM_NEXTKEY (dbf, key);
			MYDBM_FREE (key.dptr);
			key = nextkey;
			continue;
		}

		split_content (content.dptr, &entry);
		content.dptr = entry.addr;

		/* We only handle ULT_MAN, SO_MAN, and WHATIS_MAN for now. */
		if (entry.id > WHATIS_MAN) {
			MYDBM_FREE (content.dptr);
			nextkey = MYDBM_NEXTKEY (dbf, key);
			MYDBM_FREE (key.dptr);
			key = nextkey;
			continue;
		}

		/* Get just the name. */
		nicekey = xstrdup (key.dptr);
		tab = strchr (nicekey, '\t');
		if (tab)
			*tab = '\0';

		save_debug = debug;
		debug = 0;	/* look_for_file() is quite noisy */
		found = look_for_file (manpath, entry.ext, nicekey, 0);
		debug = save_debug;

		/* Now actually decide whether to purge, depending on the
		 * type of entry.
		 */
		if (entry.id == ULT_MAN || entry.id == SO_MAN)
			count += purge_normal (nicekey, &entry, found);
		else		/* entry.id == WHATIS_MAN */
			count += purge_whatis (manpath, nicekey,
					       &entry, found);

		free (nicekey);

		MYDBM_FREE (content.dptr);
		nextkey = MYDBM_NEXTKEY (dbf, key);
		MYDBM_FREE (key.dptr);
		key = nextkey;
	}

	MYDBM_CLOSE (dbf);
	return count;
}
