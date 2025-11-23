/* manp.c */
extern char *cat_manpath (char *manp);
extern char *add_nls_manpath (char *manpathlist, const char *locale);
extern char *add_system_manpath (char *systems, char *manpathlist);
extern char *manpath (char *systems);
extern void create_pathlist (char *manp, char **mp);
extern char *get_mandb_manpath (void);
extern char *get_catpath (char *name, int cattype);
extern int is_global_mandir (const char *dir);
extern void read_config_file (void);
extern char *get_def (char *thing, char *def);
extern char **get_sections (void);
