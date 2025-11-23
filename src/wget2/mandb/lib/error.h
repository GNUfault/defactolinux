#ifndef _ERROR_H
#define _ERROR_H

#include <errno.h>

#ifndef STDC_HEADERS
extern int errno;
#endif

#if __STDC__ && (HAVE_VPRINTF || HAVE_DOPRNT)
void error (int, int, const char *, ...);
#else
void error ();
#endif

#endif
