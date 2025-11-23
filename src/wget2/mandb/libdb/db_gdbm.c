/*
 * db_gdbm.c: low level gdbm interface routines for man.
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

#if defined(GDBM) && !defined(HAVE_GDBM_EXISTS)

#ifdef STDC_HEADERS
#  include <stdlib.h>
#endif /* STDC_HEADERS */

#include "manconfig.h"
#include "mydbm.h"

int gdbm_exists(GDBM_FILE dbf, datum key)
{
	char *memory;

	memory = gdbm_fetch(dbf, key).dptr;
	if (memory) {
		free(memory);
		return 1;
	}

	return 0;
}
#endif /* GDBM && !HAVE_GDBM_EXISTS*/
