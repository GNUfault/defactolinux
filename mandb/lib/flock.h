/* Operations for the `flock' call.  */
#ifndef LOCK_SH
#  define LOCK_SH       1    /* Shared lock.  */
#endif

#ifndef LOCK_EX
#  define LOCK_EX       2    /* Exclusive lock.  */
#endif

#ifndef LOCK_UN
#  define LOCK_UN       8    /* Unlock.  */
#endif

/* Can be OR'd in to one of the above.  */
#ifndef LOCK_NB
#  define LOCK_NB       4    /* Don't block when locking.  */
#endif
