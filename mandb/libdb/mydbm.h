/*
 * mydbm.h: database interface definitions and prototypes.
 *
 * Copyright (C) 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Header file to make programming independent of db type used
 *
 * Currently satisfies:
 *
 *	*hash based* 
 *		GNU dbm: 	(gdbm & ndbm)
 *		Berkeley db: 	(ndbm)
 *		`native': 	(ndbm)
 *
 *	*binary tree based*
 *		Berkeley db: 	(BTREE)
 *
 * Tue Apr 26 12:56:44 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#ifndef MYDBM_H
# define MYDBM_H

# if defined(GDBM) && !defined(NDBM) && !defined(BTREE)

#  include <gdbm.h>

#  ifndef HAVE_GDBM_EXISTS
extern __inline__ int gdbm_exists(GDBM_FILE dbf, datum key);
#  endif /* !HAVE_GDBM_EXISTS */

#  define BLK_SIZE			0  /* to invoke normal fs block size */
#  define DB_EXT				".db"
#  define MYDBM_FILE 			GDBM_FILE
#  define MYDBM_CTRWOPEN(file)		gdbm_open(file, BLK_SIZE,\
					  GDBM_NEWDB|GDBM_FAST, DBMODE, 0)
#  define MYDBM_CRWOPEN(file)		gdbm_open(file, BLK_SIZE,\
					  GDBM_WRCREAT|GDBM_FAST, DBMODE, 0)
#  define MYDBM_RWOPEN(file)		gdbm_open(file, BLK_SIZE,\
					  GDBM_WRITER|GDBM_FAST, DBMODE, 0) 
#  define MYDBM_RDOPEN(file)		gdbm_open(file, BLK_SIZE,\
					  GDBM_READER, DBMODE, 0)
#  define MYDBM_INSERT(dbf, key, cont)	gdbm_store(dbf, key, cont, GDBM_INSERT)
#  define MYDBM_REPLACE(dbf, key, cont) 	gdbm_store(dbf, key, cont, GDBM_REPLACE)
#  define MYDBM_EXISTS(dbf, key)		gdbm_exists(dbf, key)
#  define MYDBM_DELETE(dbf, key)		gdbm_delete(dbf, key)
#  define MYDBM_FETCH(dbf, key)		gdbm_fetch(dbf, key)
#  define MYDBM_CLOSE(dbf)		gdbm_close(dbf)
#  define MYDBM_FIRSTKEY(dbf)		gdbm_firstkey(dbf)
#  define MYDBM_NEXTKEY(dbf, key)		gdbm_nextkey(dbf, key)
#  define MYDBM_REORG(dbf)		gdbm_reorganize(dbf)
#  define MYDBM_FREE(x)			free(x)

# elif defined(NDBM) && !defined(GDBM) && !defined(BTREE)

#  include <ndbm.h>

#  if HAVE_FCNTL_H
#   include <fcntl.h>
#  endif

/* Berkeley db routines emulate ndbm but don't add .dir & .pag, just .db! */
#  ifdef _DB_H_ /* has Berkeley db.h been included? */
#   define BERKELEY_DB
#  endif /* _DB_H_ */

extern __inline__ datum copy_datum (datum dat);
extern DBM *ndbm_flopen(char *file, int flags, int mode);
extern int ndbm_flclose(DBM *dbf);

#  define DB_EXT				""
#  define MYDBM_FILE 			DBM*
#  define MYDBM_CTRWOPEN(file)		ndbm_flopen(file, O_TRUNC|O_CREAT|O_RDWR, DBMODE)
#  define MYDBM_CRWOPEN(file)             ndbm_flopen(file, O_CREAT|O_RDWR, DBMODE)
#  define MYDBM_RWOPEN(file)		ndbm_flopen(file, O_RDWR, DBMODE)
#  define MYDBM_RDOPEN(file)		ndbm_flopen(file, O_RDONLY, DBMODE)
#  define MYDBM_INSERT(dbf, key, cont)	dbm_store(dbf, key, cont, DBM_INSERT)
#  define MYDBM_REPLACE(dbf, key, cont)   dbm_store(dbf, key, cont, DBM_REPLACE)
#  define MYDBM_EXISTS(dbf, key)		(dbm_fetch(dbf, key).dptr != NULL)
#  define MYDBM_DELETE(dbf, key)		dbm_delete(dbf, key)
#  define MYDBM_FETCH(dbf, key) 		copy_datum(dbm_fetch(dbf, key))
#  define MYDBM_CLOSE(dbf)		ndbm_flclose(dbf)
#  define MYDBM_FIRSTKEY(dbf)		copy_datum(dbm_firstkey(dbf))
#  define MYDBM_NEXTKEY(dbf, key)		copy_datum(dbm_nextkey(dbf))
#  define MYDBM_REORG(dbf)		/* nothing - not implemented */
#  define MYDBM_FREE(x)			free (x)

# elif defined(BTREE) && !defined(NDBM) && !defined(GDBM)

#  include <sys/types.h>
#  include <fcntl.h>
#  include BDB_H
#  ifdef HAVE_LIMITS_H
#   include <limits.h>
#  endif

typedef struct {
	char *dptr;
	int dsize;
} datum;

extern __inline__ datum copy_datum (datum dat);
extern DB *btree_flopen(char *filename, int flags, int mode);
extern __inline__ int btree_close(DB *dbf);
extern __inline__ int btree_exists(DB *dbf, datum key);
extern __inline__ datum btree_fetch(DB *dbf, datum key);
extern int btree_insert(DB *dbf, datum key, datum cont);
extern __inline__ datum btree_firstkey(DB *dbf);
extern __inline__ datum btree_nextkey(DB *dbf);
extern __inline__ int btree_replace(DB *dbf, datum key, datum content);
extern __inline__ int btree_nextkeydata(DB *dbf, datum *key, datum *cont);

#  define DB_EXT			".bt"
#  define MYDBM_FILE			DB*
#  define MYDBM_CTRWOPEN(file)		btree_flopen(file, O_TRUNC|O_CREAT|O_RDWR, DBMODE)
#  define MYDBM_CRWOPEN(file)             btree_flopen(file, O_CREAT|O_RDWR, DBMODE)
#  define MYDBM_RWOPEN(file)		btree_flopen(file, O_RDWR, DBMODE)
#  define MYDBM_RDOPEN(file)		btree_flopen(file, O_RDONLY, DBMODE)
#  define MYDBM_INSERT(dbf, key, cont)	btree_insert(dbf, key, cont)
#  define MYDBM_REPLACE(dbf, key, cont)	btree_replace(dbf, key, cont)
#  define MYDBM_EXISTS(dbf, key)	btree_exists(dbf, key)
#  define MYDBM_DELETE(dbf, key)	((dbf->del)(dbf, (DBT *) &key, 0) ? -1 : 0)
#  define MYDBM_FETCH(dbf, key) 	btree_fetch(dbf, key)
#  define MYDBM_CLOSE(dbf)		btree_close(dbf)
#  define MYDBM_FIRSTKEY(dbf)		btree_firstkey(dbf)
#  define MYDBM_NEXTKEY(dbf, key)	btree_nextkey(dbf)
#  define MYDBM_REORG(dbf)		/* nothing - not implemented */
#  define MYDBM_FREE(x)			free(x)

# else /* not GDBM or NDBM or BTREE */
#  error Define either GDBM, NDBM or BTREE before including mydbm.h
# endif /* not GDBM or NDBM or BTREE */

extern char *database;
extern MYDBM_FILE dbf;

/* db_ver.c */
extern void dbver_wr(MYDBM_FILE dbf);
extern int dbver_rd(MYDBM_FILE dbf);

#endif /* MYDBM_H */
