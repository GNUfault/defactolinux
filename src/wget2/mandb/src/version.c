/*
 * version.c: the ver() and atexit()/on_exit() routines.
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 * Copyright (c), 1996, 2000, Fabrizio Polacco
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "lib/error.h"

/* print the version message, then exit */
void ver (void)
{
	printf (_("%s, version %s, %s\n"), program_name, VERSION, DATE);
	exit (OK);
}
