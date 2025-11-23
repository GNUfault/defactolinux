/*
 * hashtable.h: contains struct nlist
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Sat Aug 20 15:01:02 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 */

#include "libdb/mydbm.h"
#include "libdb/db_storage.h" 

struct nlist {
        struct nlist *next;	/* next in the chain */
        char *name;		/* the _name_ */
        void *defn;		/* the _definition_ */
        int is_text;		/* what did we store here ? */
};

extern struct nlist *lookup (char *s);
extern __inline__ struct nlist *install_text (char *name, char *text);
extern __inline__ struct nlist *install_db_ptr (char *name,
						struct mandata *db_ptr);
extern void free_hashtab (void);
