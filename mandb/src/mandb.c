/*
 * mandb.c: used to create and initialise global man database.
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 * Copyright (c) 2001 Colin Watson.
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Tue Apr 26 12:56:44 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 *
 * CJW: Security fixes. Make --test work. Purge old database entries.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>	/* for chmod() */

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
extern int errno;
#endif

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#else
#  define W_OK	2
#  ifdef HAVE_GETCWD_H
extern char *getcwd();
#  else
extern char *getwd();
#  endif
#endif /* HAVE_UNISTD_H */

#ifdef __profile__
#  if defined(HAVE_LIMITS_H) && defined(_POSIX_VERSION)
#    include <limits.h>                     /* for PATH_MAX */
#  else /* !(HAVE_LIMITS_H && _POSIX_VERSION) */
#    include <sys/param.h>                  /* for MAXPATHLEN */
#  endif /* HAVE_LIMITS_H */
#  ifndef PATH_MAX
#    ifdef _POSIX_VERSION
#      define PATH_MAX _POSIX_PATH_MAX
#    else /* !_POSIX_VERSION */
#      ifdef MAXPATHLEN
#        define PATH_MAX MAXPATHLEN
#      else /* !MAXPATHLEN */
#        define PATH_MAX 1024
#      endif /* MAXPATHLEN */
#    endif /* _POSIX_VERSION */
#  endif /* !PATH_MAX */
#endif /* __profile__ */

#ifdef SECURE_MAN_UID
#  include <pwd.h>
#endif /* SECURE_MAN_UID */

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
#include "lib/error.h"
#include "lib/cleanup.h"
#include "check_mandirs.h"
#include "manp.h"
#include "security.h"

int debug = 0;
char *program_name;
int quiet = 1;
extern int opt_test;		/* don't update db */
MYDBM_FILE dbf;
char *manp;
char *database;
extern char *extension;		/* for globbing.c */
extern int force_rescan;	/* for check_mandirs.c */

/* default options */
static const struct option long_options[] =
{
    {"create", 		no_argument,	0, 'c'},
    {"debug",		no_argument, 	0, 'd'},
    {"help",		no_argument,	0, 'h'},
    {"no-purge",	no_argument,	0, 'p'},
    {"quiet",		no_argument, 	0, 'q'},
    {"user-db",		no_argument, 	0, 'u'},
    {"no-straycats",    no_argument,	0, 's'},
    {"test",		no_argument, 	0, 't'},
    {"version",		no_argument, 	0, 'V'},
    {0, 0, 0, 0}
};

static const char args[] = "cdhpqstuV";
static int check_for_strays = 1;
static int purge = 1;
static int user;
static int create;

#ifdef NDBM
#  ifdef BERKELEY_DB
static char *dbfile;
static char *tmpdbfile;
#  else /* !BERKELEY_DB NDBM */
static char *dirfile;
static char *pagfile;
static char *tmpdirfile;
static char *tmppagfile;
#  endif /* BERKELEY_DB */
#else /* !NDBM */
static char *xfile;
static char *xtmpfile;
#endif /* NDBM */

#ifdef SECURE_MAN_UID
extern uid_t ruid;
extern uid_t euid;
#ifdef MAN_DB_UPDATES
#  define DO_CHOWN
#endif /* MAN_DB_UPDATES */
#endif /* SECURE_MAN_UID */

extern char *optarg;
extern int optind, opterr, optopt;
extern char *manpathlist[];
extern int pages;

static void usage (int status)
{
	printf (_("usage: %s [-dqspuc|-h|-V] [manpath]\n"), program_name);
	printf (_(
		"-d --debug                  produce debugging info.\n"
		"-q --quiet                  work quietly, except for 'bogus' warning.\n"
		"-s --no-straycats           don't look for or add stray cats to the dbs.\n"
		"-p --no-purge               don't purge obsolete entries from the dbs.\n"
		"-u --user-db                produce user databases only.\n"
		"-c --create                 create dbs from scratch, rather than updating.\n"
		"-V --version                show version.\n"
		"-h --help                   show this usage message.\n")
	);

	exit (status);
}

/* remove() with error checking */
static __inline__ void xremove (const char *path)
{
	if (remove (path) == -1 && errno != ENOENT)
		error (0, errno, _("can't remove %s"), path);
}

/* rename() with error checking */
static __inline__ void xrename (const char *from, const char *to)
{
	if (rename (from, to) == -1 && errno != ENOENT) {
		error (0, errno, _("can't rename %s to %s"), from, to);
		xremove (from);
	}
}

/* CPhipps 2000/02/24 - Copy a file.
 * Still plenty of error handling could be added here. */
static __inline__ void xcopy (const char *from, const char *to)
{
	FILE* ifp = fopen (from, "r");
	FILE* ofp = fopen (to, "w");

	if (!ifp || !ofp) {
		if (ifp) fclose (ifp);
		if (ofp) fclose (ofp);
		perror ("fopen");
		return;
	}

	while (!feof (ifp) && !ferror (ifp)) {
		char buf[1024];
		size_t in = fread (buf, 1, sizeof (buf), ifp);
		if (in > 0)
			fwrite (buf, 1, in, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

/* chmod() with error checking */
static __inline__ void xchmod (const char *path, mode_t mode)
{
	if (chmod (path, mode) == -1) {
		error (0, errno, _("can't chmod %s"), path);
		xremove (path);
	}
}

/* rename and chmod the database */
static __inline__ void finish_up (void)
{
#ifdef NDBM
#  ifdef BERKELEY_DB
	xrename (tmpdbfile, dbfile);
	xchmod (dbfile, DBMODE);
#  else /* not BERKELEY_DB */
	xrename (tmpdirfile, dirfile);
	xchmod (dirfile, DBMODE);
	xrename (tmppagfile, pagfile);
	xchmod (pagfile, DBMODE);
#  endif /* BERKELEY_DB */
#else /* not NDBM */
	xrename (xtmpfile, xfile);
	xchmod (xfile, DBMODE);
#endif /* NDBM */
}

#ifdef DO_CHOWN
/* chown() with error checking */
static __inline__ void xchown (const char *path, uid_t owner, uid_t group)
{
	if (chown (path, owner, group) == -1) {
		error (0, errno, _("can't chown %s"), path);
		xremove (path);
	}
}

/* change the owner of global man databases */
static __inline__ void do_chown (uid_t uid)
{
#  ifdef NDBM
#    ifdef BERKELEY_DB
	xchown (dbfile, uid, -1);
#    else /* not BERKELEY_DB */
	xchown (dirfile, uid, -1);
	xchown (pagfile, uid, -1);
#    endif /* BERKELEY_DB */
#  else /* not NDBM */
	xchown (xfile, uid, -1);
#  endif /* NDBM */
}
#endif /* DO_CHOWN */

/* dont actually create any dbs, just do an update */
static __inline__ short update_db_wrapper (const char *manpath)
{
	short amount;

	amount = update_db (manpath);
	if (amount != EOF)
		return amount;

	return create_db (manpath);
}

/* remove incomplete databases */
static void cleanup (void *dummy)
{
#ifdef NDBM
#  ifdef BERKELEY_DB
	unlink (tmpdbfile);
#  else /* !BERKELEY_DB NDBM */
	unlink (tmpdirfile);
	unlink (tmppagfile);
#  endif /* BERKELEY_DB NDBM */
#else /* !NDBM */
	unlink (xtmpfile);
#endif /* NDBM */
}

/* sort out the database names */
static short mandb (const char *catpath, const char *manpath)
{
	char pid[23];
	short amount;
	char *dbname;

	dbname = mkdbname (catpath);
	sprintf (pid, "%d", getpid ());
	database = strappend (NULL, catpath, "/", pid, NULL);
	
	if (!quiet) 
		printf (_("Processing manual pages under %s...\n"), manpath);
#ifdef NDBM
#  ifdef BERKELEY_DB
	dbfile = strappend (NULL, dbname, ".db", NULL);
	tmpdbfile = strappend (NULL, database, ".db", NULL);
	if (create || opt_test) {
		xremove (tmpdbfile);
		amount = create_db (manpath);
	} else {
		xcopy (dbfile, tmpdbfile);
		amount = update_db_wrapper (manpath);
	}
#  else /* !BERKELEY_DB NDBM */
	dirfile = strappend (NULL, dbname, ".dir", NULL);
	pagfile = strappend (NULL, dbname, ".pag", NULL);
	tmpdirfile = strappend (NULL, database, ".dir", NULL);
	tmppagfile = strappend (NULL, database, ".pag", NULL);
	if (create || opt_test) {
		xremove (tmpdirfile);
		xremove (tmppagfile);
		amount = create_db (manpath);
	} else {
		xcopy (dirfile, tmpdirfile);
		xcopy (pagfile, tmppagfile);
		amount = update_db_wrapper (manpath);
	}
#  endif /* BERKELEY_DB NDBM */
#else /* !NDBM */
	xfile = dbname;
	xtmpfile = database;
	if (create || opt_test) {
		xremove (xtmpfile);
		amount = create_db (manpath);
	} else {
		xcopy (xfile, xtmpfile);
		amount = update_db_wrapper (manpath);
	}
#endif /* NDBM */

	return amount;
}

int main(int argc, char *argv[])
{
	int c;
	char *sys_manp;
	short amount = 0;
	int strays = 0;
	int purged = 0;
	int quiet_temp = 0;
	char **mp;
	char *locale;

	int option_index; /* not used, but required by getopt_long() */

#ifdef __profile__
	char wd[PATH_MAX];
	char *cwd = wd;
#endif /* __profile__ */

#ifdef DO_CHOWN
	struct passwd *man_owner;
#endif

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
			case 'q':
				quiet_temp++;
				break;
			case 'u':
				user = 1;
				break;
			case 'c':
				create = 1;
				purge = 0;
				break;
			case 'p':
				purge = 0;
				break;
			case 's':
				check_for_strays = 0;
				break;
			case 't':
				opt_test = 1;
				break;
			case 'V':
				ver();
				break;
			case 'h':
				usage(OK);
				break;
			default:
				usage(FAIL);
				break;
		}
	}


#ifdef __profile__
#  ifdef HAVE_GETCWD
	if (!getcwd (cwd, PATH_MAX - 1))
#  else /* not HAVE_GETCWD */
	if (!getwd (cwd))
#  endif
		cwd[0] = '\0';
#endif /* __profile__ */


#ifdef SECURE_MAN_UID
	/* record who we are and drop effective privs for later use */
	init_security ();
#endif /* SECURE_MAN_UID */

#ifdef DO_CHOWN
	man_owner = getpwnam (MAN_OWNER);
	if (man_owner == NULL)
		error (FAIL, 0,
		       _("the setuid man user \"%s\" does not exist"),
		       MAN_OWNER);
	if (!user && euid != 0 && euid != man_owner->pw_uid)
		user = 1;
#endif /* DO_CHOWN */


	/* This is required for get_catpath(), regardless */
	manp = manpath (NULL);	/* also calls read_config_file() */

	if (opt_test && !debug)
		quiet = 1;
	else if (quiet_temp == 1)
		quiet = 2;
	else
		quiet = quiet_temp;

	/* pick up the system manpath or use the supplied one */
	if (argc != optind) {
		free (manp);
		manp = argv[optind];
	} else if (!user) {
		sys_manp = get_mandb_manpath ();
		if (sys_manp) {
			free (manp);
			manp = sys_manp;
		} else
			error (0, 0,
			       _("warning: no MANDB_MAP directives in %s, "
				 "using your manpath"),
			       CONFIG_FILE);
	}

	if (debug)
		fprintf (stderr, "manpath=%s\n", manp);

	/* get the manpath as an array of pointers */
	create_pathlist (xstrdup (manp), manpathlist); 

	/* finished manpath processing, regain privs */
	regain_effective_privs ();

	for (mp = manpathlist; *mp; mp++) {
		int global_manpath = is_global_mandir (*mp);
		char *catpath;

		if (global_manpath) { 	/* system db */
		/*	if (access (catpath, W_OK) == 0 && !user) */
			if (user)
				continue;
			catpath = get_catpath (*mp, SYSTEM_CAT);
			assert (catpath);
		} else {		/* user db */
			catpath = get_catpath (*mp, USER_CAT);
			if (!catpath)
				catpath = *mp;
			drop_effective_privs ();
		}

		force_rescan = 0;
		if (purge) {
			database = mkdbname (catpath);
			purged += purge_missing (*mp);
		}

		push_cleanup (cleanup, NULL);
		amount += mandb (catpath, *mp);

		if (!opt_test) {
			finish_up ();
#ifdef DO_CHOWN
			if (global_manpath && euid == 0)
				do_chown (man_owner->pw_uid);
#endif
		} else
			cleanup (NULL);
		pop_cleanup ();
		free (database);
		database = NULL;

		if (check_for_strays) {
			database = mkdbname (catpath);
			strays += straycats (*mp);
		}

		if (!global_manpath)
			regain_effective_privs ();

		if (catpath != *mp)
			free (catpath);

		chkr_garbage_detector ();
	}

	if (!quiet) {
		printf (_(
		        "%d man subdirectories contained newer manual pages.\n"
		        "%d manual pages were added.\n"), 
		        amount, pages);
		if (check_for_strays)
			printf (_("%d stray cats were added.\n"), strays);
		if (purge)
			printf (_("%d old database entries were purged.\n"),
				purged);
	}

#ifdef __profile__
	/* For profiling */
	if (cwd[0])
		chdir (cwd);
#endif /* __profile__ */

	exit (OK);
}
