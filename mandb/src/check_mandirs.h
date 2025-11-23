#include "libdb/db_storage.h"

/* check_mandirs.c */
extern char *make_filename (const char *path, const char *name,
			    struct mandata *in, char *type);
extern char *filename_info (char *file, struct mandata *info);
extern void test_manfile (char *file, const char *path);
extern void update_db_time (void);
extern void reset_db_time (void);
extern short create_db (const char *manpath);
extern short update_db (const char *manpath);
extern short purge_missing (const char *manpath);
