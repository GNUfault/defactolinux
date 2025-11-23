/* cleanup.c -- simple dynamic cleanup function management
   Copyright (C) 1995 Markus Armbruster

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Library Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.LIB.  If not, write
   to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.  */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>		/* SunOS's loosing assert.h needs it */
#include <assert.h>
#include <signal.h>
#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif

#include "manconfig.h"		/* for FATAL */
#include "cleanup.h"


#ifndef HAVE_ATEXIT
#  ifdef HAVE_ON_EXIT
#    define atexit(fun) (on_exit ((void (*)())fun, NULL))
#  else
#    error need either atexit() or on_exit()
/* If necessary we could dummify the whole module in this case */
#  endif
#endif



/* Dealing with signals */


/* saved signal actions */
static struct sigaction saved_hup_action;
static struct sigaction saved_int_action;
static struct sigaction saved_term_action;


/* Call do_cleanups(), then reraise signal with default handler. */
static void
sighandler (int signo)
{
  struct sigaction act;
  sigset_t set;

  do_cleanups ();

  /* set default signal action */
  act.sa_handler = SIG_DFL;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;
  if (sigaction (signo, &act, NULL)) {
    /* should not happen */
    _exit (FATAL);		/* exit() is taboo from signal handlers! */
  }

  /* unmask signo */
  if (   sigemptyset (&set)
      || sigaddset (&set, signo)
      || sigprocmask (SIG_UNBLOCK, &set, NULL)) {
    /* shouldn't happen */
    _exit (FATAL);		/* exit() is taboo from signal handlers! */
  }

  /* signal has now default action and is unmasked,
     reraise it to terminate program abnormally */
  kill (getpid(), signo);
  abort();
}


/* Save signo's current action to oldact, if its handler is SIG_DFL
   install sighandler, return 0 on success, -1 on failure. */
static int
trap_signal (int signo, struct sigaction *oldact)
{
  if (sigaction (signo, NULL, oldact)) {
    return -1;
  }

  if (oldact->sa_handler == SIG_DFL) {
    struct sigaction act;

    act.sa_handler = sighandler;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    return sigaction (signo, &act, oldact);
  }

  return 0;
}


/* Trap some abnormal exits to call do_cleanups(). */
static int
trap_abnormal_exits (void)
{
  if (   trap_signal (SIGHUP, &saved_hup_action)
      || trap_signal (SIGINT, &saved_int_action)
      || trap_signal (SIGTERM, &saved_term_action))
    return -1;
  return 0;
}


/* Restore signo's action from oldact if its current handler is
   sighandler, return 0 on success, -1 on failure. */
static int
untrap_signal (int signo, struct sigaction *oldact)
{
  struct sigaction act;
  if (sigaction (signo, NULL, &act)) {
    return -1;
  }

  if (act.sa_handler == sighandler) {
    return sigaction (signo, oldact, NULL);
  }

  return 0;
}


/* Undo a previous trap_abnormal_exits(). */
static int
untrap_abnormal_exits (void)
{
  if (  untrap_signal (SIGHUP, &saved_hup_action)
      | untrap_signal (SIGINT, &saved_int_action)
      | untrap_signal (SIGTERM, &saved_term_action))
    return -1;
  return 0;
}



typedef struct {
  cleanup_fun fun;
  void *arg;
} slot;

static slot *stack = NULL;	/* stack of cleanup functions */
static unsigned nslots = 0;	/* #slots in stack */
static unsigned tos = 0;	/* top of stack, 0 <= tos <= nslots */

/* Call cleanup functions in stack from from top to bottom,
   Automatically called on program termination via exit(3) or default
   action for SIGHUP, SIGINT or SIGTERM. */
void
do_cleanups (void)
{
  unsigned i;

  assert (tos <= nslots);
  for (i = tos;  i > 0;  --i) {
    stack[i-1].fun (stack[i-1].arg);
  }
}


/* Push a cleanup function on the cleanup stack,
   return 0 on success, -1 on failure.
   Caution: the cleanup function may be called from signal handlers. */
int
push_cleanup (cleanup_fun fun, void *arg)
{
  static int handler_installed = 0;

  assert (tos <= nslots);

  if (!handler_installed) {
    if (atexit (do_cleanups))
      return -1;
    handler_installed = 1;
  }

  if (tos == nslots) {
    /* stack is full, allocate another slot */
    /* stack is not expected to grow much, otherwise we would double it */
    slot *new_stack;

    if (stack) {
      new_stack = xrealloc (stack, (nslots+1)*sizeof(slot));
    } else {
      new_stack = xmalloc ((nslots+1)*sizeof(slot));
    }
      
    if (!new_stack) return -1;
    stack = new_stack;
    ++nslots;
  }

  assert (tos < nslots);
  stack[tos].fun = fun;
  stack[tos].arg = arg;
  ++tos;


  trap_abnormal_exits();

  return 0;
}


/* Pop top cleanup function from the cleanup stack. */
void
pop_cleanup (void)
{
  assert (tos > 0);
  --tos;

  if (tos == 0) untrap_abnormal_exits();
}


/* Pop all cleanup functions from the cleanup stack. */
void
pop_all_cleanups (void)
{
  tos = 0;
  untrap_abnormal_exits();
}
