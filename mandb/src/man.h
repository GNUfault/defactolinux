/* man.c */
static void man_getopt(int argc, char *argv[]);
static __inline__ char *is_section(char *name);
static __inline__ char **get_section_list (void);
static int man (char *name);
static int display (char *dir, char *man_file, char *cat_file, char *title);
static __inline__ char *make_roff_command(char *dir, char *file);
static __inline__ int do_prompt (char *name);

/* convert_name.c */
extern char *convert_name (char *name, char *alternate);
