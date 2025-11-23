/* strappend.c -- append to a dynamically allocated string
   Copyright (C) 1994 Markus Armbruster

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Library Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.LIB.  If not, write
   to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.  */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#if defined(STDC_HEADERS)
#  include <string.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strcpy();
#endif /* STDC_HEADERS */

#ifdef __STDC__
#  include <stdarg.h>
#  define VOID 		void
#  define VA_START	va_start (ap, str)
#else
#  include <varargs.h>
#  define VOID char
#  define VA_START	va_start (ap)
#endif

extern VOID *xrealloc(void *p, size_t n);

/* append strings to first argument, which is realloced to the correct size 
   first arg may be NULL */
#ifdef __STDC__
char *strappend (char *str, ...)
#else
char *strappend (str, va_alist)
     char *str;
     va_dcl
#endif
{
      va_list ap;
      int len, newlen;
      char *next, *end;

      len = str ? strlen (str) : 0;

      VA_START;
      newlen = len + 1;
      while ((next = va_arg (ap, char *)))
              newlen += strlen (next);
      va_end (ap);

      str = xrealloc (str, newlen);
      end = str + len;

      VA_START;
      while ((next = va_arg (ap, char *))) {
              strcpy (end, next);
              end += strlen (next);
      }
      va_end (ap);

      return str;
}
