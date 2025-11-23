/*
 * security.c: routines to aid secure uid operations. 
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
#include <assert.h>
#include <errno.h>

#ifndef STDC_HEADERS
extern int errno;
#endif

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#endif /* STDC_HEADERS */

#include <sys/types.h>

#if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "lib/error.h"
#include "lib/cleanup.h"
#include "security.h"

#ifdef SECURE_MAN_UID

   /*
    * This is the name of the user that the preformatted man pages belong to.
    * If you are running man as a setuid program, you should make sure
    * that all of the cat pages and the directories that
    * they live in are writeable by this user.
    */

#  ifdef HAVE_UNISTD_H
#    include <unistd.h> 			/* for _POSIX_SAVED_IDS */
#    if defined(_POSIX_SAVED_IDS)
#      if defined(__ultrix__)
         /* Ultrix pretends to have saved uids, but hasn't unless: */
#        if defined(POSIX) || defined(SYSTEM_FIVE)
#          define POSIX_SAVED_IDS
#        endif /* POSIX || SYSTEM_FIVE */
#      else /* !ultrix */
#        define POSIX_SAVED_IDS
#      endif /* ultrix */
#    endif /* _POSIX_SAVED_IDS */
#  endif /* HAVE_UNISTD_H */

/* Sort out the function to use to set the euid.  Used if we have suid */
  
#  ifdef POSIX_SAVED_IDS
#    if defined (HAVE_SETEUID)
#      define SET_EUID(euid)		seteuid(euid)
#    elif defined (HAVE_SETREUID)
#      define SET_EUID(euid)		setreuid(-1, euid)
#    elif defined (HAVE_SETRESUID)
#      define SET_EUID(euid)		setresuid(-1, euid, -1)
#    endif /* HAVE_SETEUID */

/* Sort out the function to use to swap ruid with euid.  Used if no suid. */

#  else /* !POSIX_SAVED_IDS */
#    if defined (HAVE_SETREUID)
#      define SWAP_UIDS(ida, idb)	setreuid(idb, ida)
#    elif defined (HAVE_SETRESUID)
#      define SWAP_UIDS(ida, idb)	setresuid(idb, ida, -1)
#      warning Using setresuid() whithout _POSIX_SAVED_IDS!
#    endif /* HAVE_SETREUID */
#  endif /* POSIX_SAVED_IDS */

#  if defined (POSIX_SAVED_IDS) && !defined (SET_EUID) || \
    !defined (POSIX_SAVED_IDS) && !defined (SWAP_UIDS)
#    error Cannot compile man as a setuid program: insufficient seteuid funcs.
#  endif

uid_t ruid;				/* initial real user id */
uid_t euid;				/* initial effective user id */
uid_t uid;				/* current euid */

static struct passwd *man_owner;

/* Keep a count of how many times we've dropped privileges, and only regain
 * them if regain_effective_privs() is called an equal number of times.
 */
static int priv_drop_count = 0;

static __inline__ void gripe_set_euid ()
{
	error (FATAL, errno, _("can't set effective uid"));
}

void init_security (void)
{
	ruid = getuid ();
	uid = euid = geteuid ();
	if (debug)
		fprintf (stderr, "ruid=%d, euid=%d\n", (int) ruid, (int) euid);
	priv_drop_count = 0;
	drop_effective_privs ();
}

/* Return a pointer to the password entry structure for MAN_OWNER. This
 * structure will be statically stored.
 */
struct passwd *get_man_owner (void)
{
	if (man_owner)
		return man_owner;

	man_owner = getpwnam (MAN_OWNER);
	if (!man_owner)
		error (FAIL, 0, _("the setuid man user \"%s\" does not exist"),
		       MAN_OWNER);
	assert (man_owner);
	return man_owner;
}

#endif /* SECURE_MAN_UID */

/* 
 * function to gain user privs by either (a) dropping effective privs 
 * completely (saved ids) or (b) reversing euid w/ uid.
 * Ignore if superuser.
 */
void drop_effective_privs (void)
{
#ifdef SECURE_MAN_UID
	if (uid != ruid) {
		if (debug)
			fputs ("drop_effective_privs()\n", stderr);
#  ifdef POSIX_SAVED_IDS
		if (SET_EUID (ruid))
#  else
		if (SWAP_UIDS (euid, ruid))
#  endif 
			gripe_set_euid ();

		uid = ruid;
	}
#endif /* SECURE_MAN_UID */

	priv_drop_count++;
	if (debug)
		fprintf (stderr, "++priv_drop_count = %d\n", priv_drop_count);
}

/* 
 * function to (re)gain setuid privs by (a) setting euid from suid or (b)
 * (re)reversing uid w/ euid. Ignore if superuser.
 */
void regain_effective_privs (void)
{
	if (priv_drop_count) {
		priv_drop_count--;
		if (debug)
			fprintf (stderr, "--priv_drop_count = %d\n",
				 priv_drop_count);
		if (priv_drop_count)
			return;
	}

#ifdef SECURE_MAN_UID
	if (uid != euid) {
		if (debug)
			fputs ("regain_effective_privs()\n", stderr);
#  ifdef POSIX_SAVED_IDS
		if (SET_EUID (euid))
#  else
		if (SWAP_UIDS (ruid, euid))
#  endif
			gripe_set_euid ();

		uid = euid;
	}
#endif /* SECURE_MAN_UID */
}

/* remove() a file after dropping privs. If already dropped, just remove and 
   return, don't regain any privs! */
int remove_with_dropped_privs (const char *filename)
{
	int ret;
	
#ifdef SECURE_MAN_UID
	if (uid != ruid) {
		drop_effective_privs ();
		ret = remove (filename);
		if (debug)
			fprintf (stderr, "remove(\"%s\")\n", filename);
		regain_effective_privs ();
	} else
#endif /* SECURE_MAN_UID */
		ret = remove (filename);

	if (ret != 0)
		error (0, errno, _("can't remove %s"), filename);

	return ret;
}

/* 
 * If we want to execute a system command with no effective priveledges
 * we have to either
 * 	(a) Use saved id's (if available) to completely drop effective 
 * 	    priveledges and re-engage them after the call.
 *	(b) fork() and then drop effective privs in the child. Do the 
 * 	    system() command from the child and wait for it to die.
 * (b) does not need saved ids as, once dropped, the effective privs are 
 * not required in the child again. (a) does not require a fork() as the
 * system()'d processes will not have suid=MAN_OWNER and will be unable 
 * to gain any man derived priveledges.
 */
int do_system_drop_privs (const char *command)
{
#ifdef SECURE_MAN_UID
	
#  ifdef POSIX_SAVED_IDS
	if (uid == ruid)
		return do_system (command);
	else {
		int status;
		drop_effective_privs ();
		status = do_system (command);
		regain_effective_privs ();
		return status;
	}
	
#  else /* !POSIX_SAVED_IDS */

	pid_t child;
	int status;

	fflush (NULL);
	child = fork ();

	if (child < 0) {
		error (0, errno, _("can't fork"));
		status = 0;
	} else if (child == 0) {
		pop_all_cleanups ();
		if (SWAP_UIDS (ruid, ruid))
			gripe_set_euid ();
		exit (do_system (command));
	} else {
		pid_t res;
		int save = errno;
		do {	/* cope with non-restarting system calls */
			res = waitpid (child, &status, 0);
		} while (res == -1 && errno == EINTR);
		if (res == -1)
			status = -1;
		else
			errno = save;
	}

	return status;
#  endif /* all ways to do a sys command after dropping privs */

#else  /* !SECURE_MAN_UID */
	return do_system (command);
#endif /* SECURE_MAN_UID */
}
