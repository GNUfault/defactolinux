/*
 * manpath.c: display either the manpath or catpath
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Thu Nov 17 08:29:39 GMT 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <termios.h>

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strchr(), *strcat();
#endif /* STDC_HEADERS */

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#else 
extern uid_t getuid(), geteuid();
extern pid_t vfork();
#endif /* HAVE_UNISTD_H */

#if defined(HAVE_LIMITS_H)
#  include <limits.h>
#elif defined(HAVE_SYS_PARAM_H)
#  include <sys/param.h>
#endif

#ifdef HAVE_GETOPT_H
#  include <getopt.h>
#else /* !HAVE_GETOPT_H */
#  include "lib/getopt.h"
#endif /* HAVE_GETOPT_H */

#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "lib/error.h"
#include "manp.h"

extern char *optarg;
extern int optind, opterr, optopt;

int debug = 0;
char *program_name;
int quiet = 0;

static const struct option long_options[] =
{
    {"catpath", no_argument, 		0, 'c'},
    {"global",  no_argument,		0, 'g'},
    {"debug",   no_argument, 		0, 'd'},
    {"help",    no_argument, 		0, 'h'},
    {"quiet",   no_argument, 		0, 'q'},
    {"version", no_argument, 		0, 'V'},
    {"systems",  required_argument, 	0, 'm'},
    {0, 0, 0, 0}
};

static const char args[] = "cgdhqVm:";

static void usage (int status)
{
	printf (_( 
		"usage: %s [[-gcdq] [-m system]] | [-V] | [-h]\n"),
		program_name);
	printf (_(
		"-c --catpath                show relative catpaths.\n"
		"-g --global                 show the entire global manpath.\n"
	        "-d --debug                  produce debugging info.\n"
	        "-q --quiet                  produce fewer warnings.\n"
	        "-m --systems system         express which `systems' to use.\n"
	        "-V --version                show version.\n"
	        "-h --help                   show this usage message.\n"));

	exit (status);
}

/*
 * Examine user's PATH and print a reasonable MANPATH.
 */
int main (int argc, char *argv[])
{
	int c, global = 0, cat = 0;
	char *alt_system = "";
	char *path_string;
	int option_index; /* not used, but required by getopt_long() */
	char *locale;

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

			case 'c':
				cat = 1;
				break;
			case 'd':
			    	debug = 1;
			    	break;
		    	case 'q':
			    	quiet = 1;
			    	break;
			case 'm':
				alt_system = optarg;
				break;
			case 'g':
				global = 1;
			    	quiet = 1;
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
	
	path_string = manpath (alt_system);

	if (global) {
		path_string = get_mandb_manpath ();
		if (!path_string)
			error (FAIL, 0,
			       _("warning: no global manpaths set in "
				 "config file %s"),
			       CONFIG_FILE);
	}
	if (cat)
		path_string = cat_manpath (path_string);

	printf ("%s\n", path_string);
	exit (OK);
}
