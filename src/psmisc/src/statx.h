/*
 * statx.h - Map modern statx(2) system call to older stat(2), lstat(2),
 *           and fstat(2) replacements named {,l,f}statn()
 *
 * Copyright (C) 2018 Werner Fink
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _STATX_H
#define _STATX_H

extern int stat_flags;
#ifdef WITH_STATX
# ifndef HAVE_STATX
#  define _ASM_GENERIC_FCNTL_H	/* Avoid collisions between asm/fcntl.h and bits/fcntl.h ! */
#  include <linux/fcntl.h>	/* Definition of AT_* and AT_STATX_* constants ! */
#  ifndef STATX_TYPE
#   include <linux/stat.h>	/* Provides 'struct statx' and STATX_* ! */
#  endif
# endif
extern int statn(const char*, unsigned int, struct stat*);
extern int fstatn(int, unsigned int, struct stat*);
extern int lstatn(const char*, unsigned int, struct stat*);
#else /* WITH_STATX */

#if !defined(HAVE_DECL_STATX_TYPE) || HAVE_DECL_STATX_TYPE == 0
#define STATX_TYPE		0
#define STATX_MODE		0
#define STATX_NLINK		0
#define STATX_UID		0
#define STATX_GID		0
#define STATX_ATIME		0
#define STATX_MTIME		0
#define STATX_CTIME		0
#define STATX_INO		0
#define STATX_SIZE		0
#define STATX_BLOCKS		0
#define STATX_BASIC_STATS	0
#define STATX_BTIME		0
#define STATX_ALL		0
#endif /* HAVE_DECL_STATX_TYPE */

extern inline int
statn(const char *path, unsigned int mask __attribute__((unused)), struct stat *st)
{
    return stat(path, st);
}
extern inline int
fstatn(int fd, unsigned int mask __attribute__((unused)), struct stat *st)
{
    return fstat(fd, st);
}
extern inline int
lstatn(const char *path, unsigned int mask __attribute__((unused)), struct stat *st)
{
    return lstat(path, st);
}
#endif /* WITH_STATX */

#endif
