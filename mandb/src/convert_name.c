/*
 * convert_name.c
 *
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * code to do appropriate pathname conversion
 *
 * Mon May  2 11:14:28 BST 1994 Wilf. (G.Wilford@ee.surrey.ac.uk)
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
#endif /* STDC_HEADERS */

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <stdio.h>

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "lib/error.h"
#include "manp.h"

static __inline__ void gripe_converting_name (char *name)
{
	error (FATAL, 0, _("Can't convert %s to cat name"), name);
}

/* derive the catpage path for manpage 'name'. If alternate is not NULL, use
   its value as the catpath dir. */
char *convert_name (char *name, char *alternate)
{
	char *to_name, *t1 = NULL;
	char *t2 = NULL;
#ifdef COMP_SRC
	struct compression *comp;
#endif /* COMP_SRC */

	/* we already checked that there is an alternate catdir */
	if (alternate)
		name = alternate;

#ifdef COMP_SRC
	comp = comp_info (name);
	if (comp)
		*comp->file = '\0';
#endif /* COMP_SRC */

#ifdef COMP_CAT
	to_name = strappend (NULL, name, "." COMPRESS_EXT, NULL);
#else /* !COMP_CAT */
	to_name = xstrdup (name);
#endif /* COMP_CAT */

	t1 = strrchr (to_name, '/');
	if (t1) {
		*t1 = '\0';

		t2 = strrchr (to_name, '/');
		if (t2 == NULL)
			gripe_converting_name (name);

		*t1 = '/';
		*(t2 + 1) = 'c';
		*(t2 + 3) = 't';
	} else
		gripe_converting_name (name);

	if (0) /* (debug) */
		fprintf (stderr, "to_name in convert_name () is %s\n", to_name);

#ifdef COMP_SRC
	if (comp)
		*comp->file = '.';
#endif /* COMP_SRC */
	return to_name;
}

