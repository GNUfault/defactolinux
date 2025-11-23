/*
 * basename.c: take path as argument, return basename.
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU Library General Public
 * License as specified in the file COPYING.LIB that comes with this
 * distribution.
 *
 * Thu Dec  8 20:43:47 GMT 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#if defined(STDC_HEADERS)
#  include <string.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strrchr();
#endif /* STDC_HEADERS */

/* return basename of given filename */
char *basename(char *filename)
{
	char *base;

	base = strrchr(filename, '/');
	return base ? base + 1 : filename;
}
