#define SO_LINK		0001
#define SOFT_LINK	0002
#define HARD_LINK	0004

extern char *ult_src(char *name, const char *path, struct stat *buf, int flags);
