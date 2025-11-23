/*
 * whatis.c: search the index or whatis database(s) for words.
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 * Copyright (c) 2001 Colin Watson.
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * routines for whatis and apropos programs. Whatis looks up the 
 * keyword for the description, apropos searches the entire database 
 * for word matches.
 *
 * Mon Aug  8 20:35:30 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 *
 * CJW: Add more safety in the face of corrupted databases.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#ifndef STDC_HEADERS
extern int errno;
#endif

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#if defined(STDC_HEADERS)
#  include <stdlib.h>
#  include <string.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else
extern char *strrchr();
#endif /* no string(s) header */

#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)

#ifdef HAVE_REGEX_H
#  include <sys/types.h>
#  include <regex.h>
#endif /* HAVE_REGEX_H */

#ifdef HAVE_FNMATCH_H
#  include <fnmatch.h>
#else /* !HAVE_FNMATCH_H */
#  include "lib/fnmatch.h"
#endif /* HAVE_FNMATCH_H */

#ifdef HAVE_GETOPT_H
#  include <getopt.h>
#else /* !HAVE_GETOPT_H */
#  include "lib/getopt.h"
#endif /* HAVE_GETOPT_H */

#include "manconfig.h"
#include "libdb/mydbm.h"
#include "libdb/db_storage.h"
#include "lib/error.h"
#include "manp.h"

extern char *manpathlist[];
extern char *optarg;
extern int optind, opterr, optopt;

int debug = 0;
char *program_name;
char *database;
MYDBM_FILE dbf;
int quiet = 1;

#if defined(POSIX_REGEX) || defined(BSD_REGEX)
#  define REGEX
#endif

#ifdef REGEX
#  if defined(POSIX_REGEX)
static regex_t preg;  
#  endif
static int regex;
static int exact;
#  ifndef HAVE_REGEX_H
extern char *re_comp();
extern void regfree();
#  endif
#endif

static int wildcard;
static int status = OK;

#if !defined(APROPOS) && !defined(WHATIS)
#  error #define WHATIS or APROPOS, so I know who I am
#endif

static const char args[] = "drewhVm:M:fkL:";

static const struct option long_options[] =
{
	{"debug",	no_argument,		0, 'd'},
	{"regex",	no_argument,		0, 'r'},
	{"exact",	no_argument,		0, 'e'},
	{"wildcard",	no_argument,		0, 'w'},
	{"help",	no_argument,		0, 'h'},
	{"version",	no_argument,		0, 'V'},
	{"systems",	required_argument,	0, 'm'},
	{"manpath",	required_argument,	0, 'M'},
	{"whatis",	no_argument,		0, 'f'},
	{"apropos",	no_argument,		0, 'k'},
	{"locale",	required_argument,	0, 'L'},
	{"exact",	no_argument,		0, 'e'},
	{0, 0, 0, 0}
};

#ifdef APROPOS
static void usage (int status)
{
	printf (_("usage: %s [-d] [-r|-w|-e] [-m systems] [-M manpath] | [-h] | [-V] keyword ...\n"), program_name);
	printf (_(
		"-d --debug		produce debugging info.\n"
		"-r --regex 		interpret each keyword as a regex (default).\n"
		"-e --exact 		search each keyword for exact match.\n"
		"-w --wildcard		the keyword(s) contain wildcards.\n"
		"-m --systems system	include alternate systems man pages.\n"
		"-M --manpath path	set search path for manual pages to `path'.\n"
		"-V --version		show version.\n"
		"-h --help		show this usage message.\n"));

	exit (status);
}
#else	
static void usage (int status)
{
	printf (_("usage: %s [-d] [-r|-w] [-m systems] [-M manpath] | [-h] | [-V] keyword ...\n"), program_name);
	printf(_(
	       "-d --debug		produce debugging info.\n"
	       "-r --regex 		interpret each keyword as a regex.\n"
	       "-w --wildcard		the keyword(s) contain wildcards.\n"
	       "-m --systems system	include alternate systems man pages.\n"
	       "-M --manpath path	set search path for manual pages to `path'.\n"
	       "-V --version		show version.\n"
	       "-h --help		show this usage message.\n"));

	exit (status);
}
#endif

/* do the old thing, if we cannot find the relevant database */
static __inline__ int use_grep (char *page, char *manpath)
{
	char *whatis_file = strappend (NULL, manpath, "/whatis", NULL);
	int status;

	if (access (whatis_file, R_OK) == 0) {
		char *esc_page = escape_shell (page);
		char *esc_file = escape_shell (whatis_file);
		char *flags, *anchor, *command;
#if defined(WHATIS)
		flags = get_def ("whatis_grep_flags", WHATIS_GREP_FLAGS);
		anchor = "^";
#elif defined(APROPOS)
#ifdef REGEX
		if (regex)
			flags = get_def ("apropos_regex_grep_flags",
					 APROPOS_REGEX_GREP_FLAGS);
		else
#endif
			flags = get_def ("apropos_grep_flags",
					 APROPOS_GREP_FLAGS);
		anchor = "";
#endif 	

		command = strappend (NULL, get_def ("grep", GREP), flags, " ",
				     anchor, esc_page, " ", esc_file, NULL);
		status = (system (command) == 0);
		free (command);
		free (esc_file);
		free (esc_page);
	} else {
		if (debug) {
			error (0, 0, _("warning: can't read the fallback whatis text database."));
			error (0, errno, "%s/whatis", manpath);
		}
		status = 0;
	}

	free (whatis_file);
	return status;
}

/* fill_in_whatis() is really a ../libdb/db_lookup.c routine but whatis.c
   is the only file that actually requires access to the whatis text... */
   
/* Take mandata struct (earlier returned from a dblookup()) and return 
   the relative whatis */
static char *get_whatis (struct mandata *info, char *page)
{
	int rounds;

	/* See if we need to fill in the whatis here. */
	if (*(info->pointer) == '-' || STREQ (info->pointer, page)) {
		if (info->whatis != NULL && *(info->whatis))
			return xstrdup (info->whatis);
		if (*(info->pointer) != '-')
			error (0, 0, _("warning: %s contains a pointer loop"),
			       page);
		return xstrdup (_("(unknown)"));
	}

	/* Now we have to work through pointers. The limit of 10 is fairly
	 * arbitrary: it's just there to avoid an infinite loop.
	 */
	info = dblookup_exact (info->pointer, info->ext);
	for (rounds = 0; rounds < 10; rounds++) {
		struct mandata *newinfo;
		char *return_whatis = NULL;

		/* If the pointer lookup fails, do nothing. */
		if (!info)
			return xstrdup (_("(unknown)"));

		/* See if we need to fill in the whatis here. */
		if (*(info->pointer) == '-') {
			if (info->whatis != NULL && *(info->whatis))
				return_whatis = xstrdup (info->whatis);
			else
				return_whatis = xstrdup (_("(unknown)"));
		}

		if (return_whatis) {
			free_mandata_struct (info);
			return return_whatis;
		}

		newinfo = dblookup_exact (info->pointer, info->ext);
		free_mandata_struct (info);
		info = newinfo;
	}

	error (0, 0, _("warning: %s contains a pointer loop"), page);
	return xstrdup (_("(unknown)"));
}

/* print out any matches found */
static void display (struct mandata *info, char *page)
{
	char *string, *whatis;
	
	whatis = get_whatis (info, page);
	
	if (debug)
		dbprintf (info);

	if (*(info->pointer) == '-' || STREQ (info->pointer, page))
		string = strappend (NULL, page, " (", info->ext, ")", NULL);
	else
		string = strappend (NULL, page, " (", info->ext, ") [",
				    info->pointer, "]", NULL);

	if (strlen (string) < (size_t) 20)
		printf ("%-20s - %s\n", string, whatis);
	else
		printf ("%s - %s\n", string, whatis);

	free (whatis);
	free (string);
}

/* return lowered version of s */
static char *lower (char *s)
{
	char *low, *p;

	p = low = (char *) xmalloc (strlen (s) +1);

	while (*s) {
		if (isupper (*s))
			*p++ = tolower (*s++);
		else
			*p++ = *s++;
	}

	*p = *s;
	return low;
}

#ifdef WHATIS
/* lookup the page and display the results */
static __inline__ int whatis (char *page)
{
	struct mandata *info;
	int count = 0;

	info = dblookup_all (page, NULL);
	while (info) {
		struct mandata *pinfo;
			
		display (info, page);
		count++;
		pinfo = info->next;	/* go on to next structure */
	 	free (info->addr);	/* free info's `content' */
	 	free (info);		/* free info */
		info = pinfo;
	}
	return count;
}
#endif /* WHATIS */

/* return 1 if page matches name, else 0 */
static int parse_name (char *page, char *dbname)
{ 
#ifdef REGEX
	if (regex)
#  if defined(POSIX_REGEX)
		return (regexec (&preg, dbname, 0, (regmatch_t *) 0, 0) == 0);
#  elif defined(BSD_REGEX)
		return re_exec (dbname);
#  endif
#endif /* REGEX */

#ifdef APROPOS
	if (!wildcard) {
		char *lowdbname = lower (dbname);
		return STREQ (lowdbname, page);
		free (lowdbname);
	}
#endif

	return (fnmatch (page, dbname, 0) == 0);
}

#ifdef APROPOS
/* return 1 on word match */
static int match (char *lowpage, char *whatis)
{
	char *lowwhatis = lower (whatis);
	size_t len = strlen (lowpage);
	char *p, *begin;

	begin = lowwhatis;
	
	/* check for string match, then see if it is a _word_ */
	while (lowwhatis && (p = strstr (lowwhatis, lowpage))) {
		char *left = p - 1; 
		char *right = p + len;

		if ((p == begin || (!islower(*left) && *left != '_')) &&
		    (!*right || (!islower(*right) && *right != '_')) ) {
		    	free (begin);
		    	return 1;
		}
		lowwhatis = p + 1;
	}

	free(begin);
	return 0;
}

/* return 1 if page matches whatis, else 0 */
static int parse_whatis (char *page, char *lowpage, char *whatis)
{ 
#ifdef REGEX
	if (regex) 
#  if defined(POSIX_REGEX)
		return (regexec (&preg, whatis, 0, (regmatch_t *) 0, 0) == 0);
#  elif defined(BSD_REGEX)
		return re_exec (whatis);
#  endif
#endif /* REGEX */

	if (wildcard)
		return (fnmatch (page, whatis, 0) == 0);

	return match (lowpage, whatis);
}
#endif /* APROPOS */

/* cjwatson: Optimized functions don't seem to be correct in some
 * circumstances; disabled for now.
 */
#undef BTREE

/* scan for the page, print any matches */
static int apropos (char *page, char *lowpage)
{
	datum key, cont;
	int found = 0;

#ifndef BTREE
	datum nextkey;

	key = MYDBM_FIRSTKEY (dbf);
	while (key.dptr) {
		cont = MYDBM_FETCH (dbf, key);
#else /* BTREE */
	int end;

	end = btree_nextkeydata (dbf, &key, &cont);
	while (!end) {
#endif /* !BTREE */
		/* bug#4372, NULL pointer dereference in cont.dptr, fix
		 * by dassen@wi.leidenuniv.nl (J.H.M.Dassen), thanx Ray.
		 * cjwatson: In that case, complain and exit, otherwise we
		 * might loop (bug #95052).
		 */
		if (!cont.dptr)
		{
			if (debug)
				fprintf (stderr, "key was %s\n", key.dptr);
			error (FATAL, 0,
			       _("Database %s corrupted; rebuild with "
				 "mandb --create"),
			       database);
		}

		if (*key.dptr != '$') {
			if (*cont.dptr != '\t')		/* a real page */
			{
				char *tab;
				int match;
#ifdef APROPOS
				char *whatis;
#endif

				tab = strrchr(key.dptr, '\t');
				if (tab) 
					 *tab = '\0';

#ifdef APROPOS
				match = parse_name (lowpage, key.dptr);
				whatis = strrchr (cont.dptr, '\t');
				if (!(whatis && *++whatis))
					whatis = NULL;
					
				if (!match && whatis)
					match = parse_whatis (page, lowpage,
							      whatis);
#else /* WHATIS */
				match = parse_name (page, key.dptr);
#endif /* APROPOS */
				if (match) {
					struct mandata info;
					split_content (cont.dptr, &info);
					display (&info, key.dptr);
					found++;
					cont.dptr = info.addr;
				}

				found += match;
				if (tab)
					*tab = '\t';
			}
		}
#ifndef BTREE
		nextkey = MYDBM_NEXTKEY (dbf, key);
		MYDBM_FREE (cont.dptr);
		MYDBM_FREE (key.dptr);
		key = nextkey; 
#else /* BTREE */
		MYDBM_FREE (cont.dptr);
		MYDBM_FREE (key.dptr);
		end = btree_nextkeydata (dbf, &key, &cont);
#endif /* !BTREE */
	}

	return found;
}

/* loop through the man paths, searching for a match */
static void search (char *page)
{
	int found = 0;
	char *lowpage = lower (page);
	char *catpath, **mp;

	if (debug)
		fprintf (stderr, "lower(%s) = \"%s\"\n", page, lowpage);

	for (mp = manpathlist; *mp; mp++) {
		catpath = get_catpath (*mp, SYSTEM_CAT | USER_CAT);
		
		if (catpath) {
			database = mkdbname (catpath);
			free (catpath);
		} else
			database = mkdbname (*mp);

		if (debug)
			fprintf (stderr, "path=%s\n", *mp);

		dbf = MYDBM_RDOPEN (database);
		if (dbf && dbver_rd (dbf)) {
			MYDBM_CLOSE (dbf);
			dbf = NULL;
		}
		if (!dbf) {
			found += use_grep (page, *mp);			
			continue;
		}

#ifdef WHATIS
# ifdef REGEX
		if (regex || wildcard) {
# else /* !REGEX */
		if (wildcard) {
# endif /* REGEX */
			found += apropos (page, lowpage);
		} else
			found += whatis (page);
#else /* APROPOS */
		found += apropos (page, lowpage);
#endif /* WHATIS */
		MYDBM_CLOSE (dbf);
	}

	chkr_garbage_detector();

	if (!found) {
		printf (_("%s: nothing appropriate.\n"), page);
		status = NOT_FOUND;
	}

	free (lowpage);
}

int main (int argc, char *argv[])
{
	int c;
	char *manp = NULL, *alt_systems = "";
	char *llocale = 0, *locale;
	int option_index;

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
			case 'L':
				llocale = optarg;
				break;
			case 'm':
				alt_systems = optarg;
				break;
			case 'M':
				manp = optarg;
				break;
			case 'e':
#ifdef REGEX
				regex = 0;
				exact = 1;
#endif
				break;
			case 'r':
#ifdef REGEX
				regex = 1;
#endif
				break;
			case 'w':
#ifdef REGEX
				regex = 0;
#endif
				wildcard = 1;
				break;
			case 'f': /* fall through */
			case 'k': /* ignore, may be passed by man */
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

	/* close this locale and reinitialise in case a new locale was 
	   issued as an argument or in $MANOPT */
	if (llocale) {
		setlocale (LC_ALL, llocale);
		locale = xstrdup (llocale);
		if (debug)
			fprintf(stderr,
				"main(): locale = %s, internal_locale = %s\n",
				llocale, locale);
		if (locale) {
			extern int _nl_msg_cat_cntr;
			if (locale[2] == '_' )
				locale[2] = '\0';
			setenv ("LANGUAGE", locale, 1);
			++_nl_msg_cat_cntr;
		}
	}

#if defined(REGEX) && defined(APROPOS)
	/* Become it even if it's null - GNU standards */
	/* if (getenv("POSIXLY_CORRECT")) */
	if (!exact && !wildcard)
		regex = 1;
#endif

	/* Make sure that we have a keyword! */
	if (argc == optind) {
		printf (_("%s what?\n"), program_name);
		return 0;
	}

	/* sort out the internal manpath */
	if (manp == NULL)
		manp = add_nls_manpath (manpath (alt_systems), locale);
	else
		free (manpath (NULL));

	create_pathlist (xstrdup (manp), manpathlist);

	while (optind < argc) {
#if defined(POSIX_REGEX)		
		if (regex) {
			int errcode = regcomp (&preg, argv[optind], 
					       REG_EXTENDED |
					       REG_NOSUB |
					       REG_ICASE);
						   
			if (errcode) {
				char error_string[64];
				regerror (errcode, &preg, error_string, 64);
				error (FAIL, 0, _("fatal: regex `%s': %s"),
				       argv[optind], error_string);
			}
		}
#elif defined(BSD_REGEX)
		if (regex) {
			/* How to set type of regex ...? */
			char *error_string = re_comp (argv[optind]);
			if (error_string)
				error (FAIL, 0, _("fatal: regex `%s': %s"),
				       argv[optind], error_string);
		}
#endif /* REGEX */
		search(argv[optind++]);
	}

	exit (status);
}
