#include <pwd.h>

/* security.c */
extern __inline__ void drop_effective_privs (void);
extern __inline__ void regain_effective_privs (void);
extern int do_system_drop_privs (const char *command);
extern int remove_with_dropped_privs (const char *filename);
extern void init_security (void);
extern struct passwd *get_man_owner (void);
