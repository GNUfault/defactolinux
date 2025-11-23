/*
 * db_ndbm.c: low level ndbm interface routines for man.
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
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef NDBM

#include <stdio.h>

#if HAVE_SYS_FILE_H
#  include <sys/file.h> /* for flock() */
#endif

#include <sys/types.h> /* for open() */
#include <sys/stat.h>

#if HAVE_FCNTL_H
#  include <fcntl.h>
#endif

#ifdef STDC_HEADERS
#include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include "manconfig.h"
#include "lib/flock.h"
#include "mydbm.h"
#include "db_storage.h"

/* release the lock and close the database */
int ndbm_flclose(DBM *dbf)
{
	flock(dbm_dirfno(dbf), LOCK_UN);
	dbm_close(dbf);
	return 0;
}

/* open a ndbm type database, with file locking. */
DBM* ndbm_flopen(char *filename, int flags, int mode)
{
	DBM *dbf;
	int lock_op;
	int lock_failed;

	if (flags & ~O_RDONLY) {
		/* flags includes O_RDWR or O_WRONLY, need an exclusive lock */
		lock_op = LOCK_EX | LOCK_NB;
	} else {
		lock_op = LOCK_SH | LOCK_NB;
	}

	if (flags & O_TRUNC) {
		/* opening the db is destructive, need to lock first */
		char *dir_fname;
		int dir_fd;

		dbf = NULL;
		lock_failed = 1;
		dir_fname = xmalloc (strlen(filename) + 5);
		sprintf (dir_fname, "%s.dir", filename);
		dir_fd = open (dir_fname, flags & ~O_TRUNC, mode);
		free(dir_fname);
		if (dir_fd != -1) {
			if (!(lock_failed = flock (dir_fd, lock_op)))
				dbf = dbm_open(filename, flags, mode);
			close (dir_fd);
		}
	} else {
		dbf = dbm_open(filename, flags, mode);
		if (dbf)
			lock_failed = flock(dbm_dirfno(dbf), lock_op);
	}

	if (!dbf)
		return NULL;

	if (lock_failed) {
		gripe_lock(filename);
		dbm_close(dbf);
		return NULL;
	}

	return dbf;
}

#endif /* NDBM */
