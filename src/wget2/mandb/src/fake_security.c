/*
 * fake_security.c: fake routines for non setuid programs
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Mon Aug  8 20:35:30 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <sys/types.h>

#include "manconfig.h"

uid_t ruid, euid, uid;

void drop_effective_privs (void)
{
	/* do nothing */
}

void regain_effective_privs (void)
{
	/* do nothing */
}

int remove_with_dropped_privs (const char *filename)
{
	return remove (filename);
}

int do_system_drop_privs (const char *command)
{
	return do_system (command);
}
