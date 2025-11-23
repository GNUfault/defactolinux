/*
 * catman.c: create and/or update cat files
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Thu Dec  8 00:03:12 GMT 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

/* MAX_ARGS must be >= 6, 4 for options, 1 for page and 1 for NULL */
#define MAX_ARGS	1024	/* *argv[MAX_ARG] */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <errno.h>

#if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif /* HAVE_SYS_WAIT_H */

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
extern char *getenv();
extern int errno;
#endif

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#if defined(HAVE_LIMITS_H) && defined(_POSIX_VERSION)
#  include <limits.h>  
#else /* !(HAVE_LIMITS_H && _POSIX_VERSION) */
#  include <sys/param.h> 
#endif /* HAVE_LIMITS_H */

#ifndef NAME_MAX
#  if defined(_POSIX_VERSION) && defined(_POSIX_NAME_MAX)
#    define NAME_MAX _POSIX_NAME_MAX
#  else /* !_POSIX_VERSION */
#    ifdef MAXNAMLEN
#      define NAME_MAX MAXNAMLEN
#    else /* !MAXNAMLEN */
#      define NAME_MAX 255 		/* default to max */
#    endif /* MAXNAMLEN */
#  endif /* _POSIX_VERSION */
#endif /* !NAME_MAX */

#ifndef ARG_MAX
#  if defined(_POSIX_VERSION) && defined(_POSIX_ARG_MAX)
#    define ARG_MAX _POSIX_ARG_MAX
#  else /* !_POSIX_VERSION */
#    define ARG_MAX 4096 		/* default to min */
#  endif /* _POSIX_VERSION */
#endif /* !ARG_MAX */

#ifdef HAVE_GETOPT_H
#  include <getopt.h>
#else /* !HAVE_GETOPT_H */
#  include "lib/getopt.h"
#endif /* HAVE_GETOPT_H */

#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "libdb/mydbm.h"
#include "libdb/db_storage.h"
#include "lib/error.h"
#include "manp.h"

/* globals */
int debug = 0;
char *program_name;
int quiet = 1;
MYDBM_FILE dbf;
char *manp;
char *database;

static const struct option long_options[] =
{
    {"debug",		no_argument, 	0, 'd'},
    {"manpath",      required_argument, 0, 'M'},
    {"help",		no_argument,	0, 'h'},
    {"version",		no_argument, 	0, 'V'},
    {0, 0, 0, 0}
};

static const char args[] = "dM:hV";

#ifdef HAVE_SETLOCALE
static char *locale;
#endif /* HAVE_SETLOCALE */

extern char *optarg;
extern int optind, opterr, optopt;
extern char *manpathlist[];

static void usage (int status)
{
	printf (_("usage: %s [-dhV] [-M manpath] [section] ...\n"),
		program_name);
	printf (_(
		"-d --debug                  produce debugging info.\n"
		"-M --manpath path           set search path for manual pages "
					    "to `path'.\n"
		"-V --version                show version.\n"
		"-h --help                   show this usage message.\n"));

	exit (status);
}

/* open db for reading, return 0 for success, errcode for failure */
static int rdopen_db (void)
{ 
	dbf = MYDBM_RDOPEN (database);
	if (dbf == NULL) {
		error (0, errno, _("cannot read database %s"), database);
		return 1;
	}
	return 0;
}

/* fork() and execve() man with the appropriate catman args. 
   If we __inline__ this function, gcc v2.6.2 gives us `clobber' warnings ?? */
static void catman (char *argp[], int arg_no)
{
	pid_t child;
	int status;
	int res;

	if (debug) { 	/* show us what the command is going to be :) */
		char **p;

		fputs ("man command =", stderr);
		for (p = argp; *p; p++)
			fprintf (stderr, " %s", *p);
		putc ('\n', stderr);
		return;
	}
		
	child = vfork ();
	if (child < 0)
		error (FATAL, errno, _("fork failed"));
	else if (child == 0) {
		char *const envp[] = { NULL };

		execve (MAN, argp, envp);
		_exit (127);
	} 

	do {			/* cope with non-restarting system calls */
		res = waitpid (child, &status, 0);
	} while ((res == -1) && (errno == EINTR));
	if (res == -1)
		error (FATAL, 0, _("can't get man command's exit status"));
	else if (status)
		error (CHILD_FAIL, 0,
		       _("man command failed with exit status %d"), status);
}

/* accept key and a pointer to the array address that needs to be filled in,
   fill in address and return 1 if key.dptr can be freed otherwise 0 */ 
static __inline__ int add_arg (datum key, char **argument)
{
    	char *tab;

	tab = strrchr (key.dptr, '\t');

	if (tab && tab != key.dptr) {
		*tab = '\0';
		*argument = xstrdup (key.dptr);
		*tab = '\t';
		return 1;
	} 

	*argument = key.dptr;
	return 0;
}

/* simply close db, tidy up, call catman() and then free() the array */
static void do_catman (char *argp[], int arg_no, int first_arg)
{
	MYDBM_CLOSE (dbf);

	/* The last argument must be NULL */
	argp[arg_no] = NULL;
	
	catman (argp, arg_no);

	/* don't free the last entry, it's NULL */
	/* don't free the last but one entry, it's our nextkey */
	arg_no -= 2;

	while (arg_no >= first_arg)
		/* all db methods now free() */	
		MYDBM_FREE (argp[arg_no--]);
}

#ifdef BTREE
/* we need to reset the cursor position after a reopen */
static __inline__ void reset_cursor (datum key)
{
	int status;
	DBT content; /* dummy */
	
	status = (dbf->seq) (dbf, (DBT *) &key, &content, R_CURSOR);
	if (status == 1)
		status = (dbf->seq) (dbf, (DBT *) &key, &content, R_LAST);
	if (status == -1)
		error (FATAL, errno,
		       _("unable to reset cursor position in %s"), database);
}
#else /* !BTREE */
#  define reset_cursor(key) 		/* nothing */
#endif /* BTREE */

/* find all pages that are in the supplied manpath and section and that are
   ultimate source files. */
static __inline__ int parse_for_sec (char *manpath, char *section)
{
	char *argp[MAX_ARGS];
	datum key;
	size_t arg_size, initial_bit;
	int arg_no = 0, message = 1, first_arg;
	
	if (rdopen_db () || dbver_rd (dbf))
		return 1;
		
	argp[arg_no++] = "man"; 	/* Name of program */

#ifdef HAVE_SETLOCALE
	/* As we supply a NULL environment to save precious execve() space,
	   we must also supply a locale if necessary */
	if (locale) {
		argp[arg_no++] = "-L";		/* locale option */
		argp[arg_no++] = locale;	/* The locale */
		initial_bit += sizeof "-L" + strlen (locale) + 1;
	} else
		initial_bit = 0;

#endif /* HAVE_SETLOCALE */

	argp[arg_no++] = "-caM";	/* options */
	argp[arg_no++] = manpath;	/* particular manpath */
	argp[arg_no++] = section;	/* particular section */

	first_arg = arg_no;		/* first pagename argument */
	
	initial_bit = sizeof "man" + sizeof "-caM" + 
		      strlen (manpath) + strlen (section) + 2;

	arg_size = initial_bit;
	key = MYDBM_FIRSTKEY (dbf);

	while (key.dptr != NULL) {
		int free_key = 1;

		/* ignore db identifier keys */
		if (*key.dptr != '$') { 
			datum content;

			content = MYDBM_FETCH (dbf, key);

			if (!content.dptr)
				error (FATAL, 0,
				       _( "NULL content for key: %s"),
				       key.dptr);

			/* ignore overflow entries */
			if (*content.dptr != '\t') { 
				struct mandata entry;

				split_content (content.dptr, &entry);
				content.dptr = entry.addr;

				/* Accept if the entry is an ultimate manual
				   page and the section matches the one we're
				   currently dealing with */
				if (entry.id == ULT_MAN && 
				    strcmp (entry.sec, section) == 0) {
					if (message) {
						printf (_("\nUpdating cat files for section %s of man hierarchy %s\n"),
							section, manpath);
						message = 0;
					}

					free_key = add_arg (key,
							    &(argp[arg_no]));
					arg_size += strlen (argp[arg_no++]) + 1;

					if (debug)
						fprintf (stderr, "arg space free: %d bytes\n",
							 ARG_MAX - arg_size);

					/* Check to see if we have enough room 
					   to add another max sized filename 
					   and that we haven't run out of array 
					   space too */ 
				    	if (arg_size >= ARG_MAX - NAME_MAX ||
				    	    arg_no == MAX_ARGS - 1) {
				    		do_catman (argp, arg_no,
							   first_arg);

				    		/* reopen db and tidy up */
				    		if (rdopen_db ())
				    			return 1;
						reset_cursor (key);
						free_key = 1;
				    		arg_no = first_arg;
				    		arg_size = initial_bit;
				    	}
				}
			}
			
			/* we don't need the content ever again */
			assert (content.dptr); /* just to be sure */
			MYDBM_FREE (content.dptr);
		}

		/* If we are not using the key, free it now */
		if (free_key) {
			datum nextkey;

			nextkey = MYDBM_NEXTKEY (dbf, key);
			MYDBM_FREE (key.dptr);
			key = nextkey;
		} else 
			key = MYDBM_NEXTKEY (dbf, key);
	}

	if (arg_no > first_arg)
		do_catman (argp, arg_no, first_arg);

	return 0;
}

static __inline__ int check_access (const char *directory)
{
	if (access (directory, W_OK)) {
		error (0, errno, _("cannot write within %s"), directory);
		return 1;
	}

	return 0;
}
		
int main (int argc, char *argv[])
{
	int c;
	char *sys_manp;
	char **mp, **sections;

	int option_index; /* not used, but required by getopt_long() */

	program_name = xstrdup (basename (argv[0]));

	/* initialise the locale */
	locale = setlocale (LC_ALL, "");
	if (locale)
		locale = xstrdup (locale);
	else {
		/* Obviously can't translate this. */
		error (0, 0, "can't set the locale; make sure $LC_* and $LANG "
			     "are correct");
		locale = "C";
	}
	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	while ((c = getopt_long (argc, argv, args,
				 long_options, &option_index)) != EOF) {
		switch (c) {

			case 'd':
				debug = 1;
				break;
			case 'M':
				manp = optarg;
				break;
			case 'V':
				ver ();
				break;
			case 'h':
				usage (OK);
				break;
			default:
				usage (FAIL);
				break;
		}
	}

	/* Deal with the sections */

	/* If we were supplied sections: sort them out */
	if (optind != argc) {
		char **sp;
		
		sections = sp = (char **) xmalloc ((argc - optind + 1) * 
						    sizeof (char *));
		while (optind != argc)
			*sp++ = argv[optind++];
		*sp = NULL;
	} else {
		char *mansect;

		mansect = getenv ("MANSECT");
		if (mansect && *mansect) {
			/* MANSECT contains sections */
			char *sec;
			int i = 0;

			mansect = xstrdup (mansect);
			sections = NULL;
			for (sec = strtok (mansect, ":"); sec; 
			     sec = strtok (NULL, ":")) {
			     	sections = (char **) xrealloc (sections, 
							       (i + 2) *
							       sizeof (char *));
				sections[i++] = sec;
			}
			sections[i] = NULL;
		} else {
			/* use default sections */ 
			static char *std_sections[] = STD_SECTIONS;
			sections = std_sections;
		}
	}

	if (debug) {
		char **sp;

		for (sp = sections; *sp; sp++)
			fprintf (stderr, "sections: %s\n", *sp);
	}

	/* Deal with the MANPATH */

	/* This is required for get_catpath(), regardless */
	sys_manp = manpath (NULL);

	/* pick up the system manpath or use the supplied one */
	if (!manp) {
		manp = get_mandb_manpath ();
		if (!manp)
			manp = sys_manp;
	}

	if (debug)
		fprintf (stderr, "manpath=%s\n", manp);

	/* get the manpath as an array of pointers */
	create_pathlist (xstrdup (manp), manpathlist); 
	
	for (mp = manpathlist; *mp; mp++) {
		char *catpath, **sp;
		size_t len;

		catpath = get_catpath (*mp, SYSTEM_CAT | USER_CAT);

		if (catpath) { 
			if (is_directory (catpath) != 1) {
				free (catpath);
				continue;
			}
			database = mkdbname (catpath);
		} else {
			if (is_directory (*mp) != 1)
				continue;
			database = mkdbname (*mp);
			catpath = xstrdup (*mp);
		}

		len = strlen (catpath);
		
		for (sp = sections; *sp; sp++) {
			*(catpath + len) = '\0';
			catpath = strappend (catpath, "/cat", *sp, NULL);
			if (is_directory (catpath) != 1)
				continue;
			if (check_access (catpath))
				continue;
			if (parse_for_sec (*mp, *sp)) {
				error (0, 0, _("unable to update %s"), *mp);
				break;
			}
		}
			
		free (catpath);
	}

	exit (OK);
}
