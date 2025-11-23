/*
 * statx.c - Map modern statx(2) system call to older stat(2), lstat(2),
 *	     and fstat(2) replacements named {,l,f}statn()
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
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/sysmacros.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>		/* Definition of AT_* constants */

int stat_flags = AT_NO_AUTOMOUNT|AT_STATX_DONT_SYNC;
#ifdef WITH_STATX

#include <errno.h>
#ifndef HAVE_STATX
# define _ASM_GENERIC_FCNTL_H	/* Avoid collisions between asm/fcntl.h and bits/fcntl.h ! */
# include <linux/fcntl.h>	/* Definition of AT_* and AT_STATX_* constants ! */
#endif

#include <sys/stat.h>

#ifndef HAVE_STATX
# ifndef STATX_TYPE
#  include <linux/stat.h>	/* Provides 'struct statx' and STATX_* ! */
# endif
#endif


int statn(const char *pathname, unsigned int mask, struct stat *st)
{
    int flags = stat_flags;
    int dirfd = pathname && *pathname == '/' ? 0 : AT_FDCWD;
    int ret;
    struct statx stx;

#ifndef HAVE_STATX
    ret = syscall(SYS_statx, dirfd, pathname, flags, mask, &stx);
#else
    ret = statx(dirfd, pathname, flags, mask, &stx);
#endif
    if (ret >= 0) {
	st->st_dev  = makedev(stx.stx_dev_major,  stx.stx_dev_minor);
	st->st_rdev = makedev(stx.stx_rdev_major, stx.stx_rdev_minor);

	st->st_ino  = stx.stx_ino;
	st->st_mode = stx.stx_mode;
	st->st_nlink = stx.stx_nlink;
	st->st_uid = stx.stx_uid;
	st->st_gid = stx.stx_gid;
	st->st_size = stx.stx_size;
	st->st_blksize = stx.stx_blksize;
	st->st_blocks = stx.stx_blocks;

	st->st_atim.tv_sec = stx.stx_atime.tv_sec;
	st->st_atim.tv_nsec = stx.stx_atime.tv_nsec;
	st->st_mtim.tv_sec = stx.stx_mtime.tv_sec;
	st->st_mtim.tv_nsec = stx.stx_mtime.tv_nsec;
	st->st_ctim.tv_sec = stx.stx_ctime.tv_sec;
	st->st_ctim.tv_nsec = stx.stx_ctime.tv_nsec;
    } else if (errno==ENOSYS || errno==EINVAL)
        return stat(pathname, st);
    return ret;
}

int fstatn(int fd, unsigned int mask, struct stat *st)
{
    int flags = AT_EMPTY_PATH|stat_flags;
    int ret;
    struct statx stx;

#ifndef HAVE_STATX
    ret = syscall(SYS_statx, fd, "", flags, mask, &stx);
#else
    ret = statx(fd, "", flags, mask, &stx);
#endif
    if (ret >= 0) {
	st->st_dev  = makedev(stx.stx_dev_major,  stx.stx_dev_minor);
	st->st_rdev = makedev(stx.stx_rdev_major, stx.stx_rdev_minor);

	st->st_ino  = stx.stx_ino;
	st->st_mode = stx.stx_mode;
	st->st_nlink = stx.stx_nlink;
	st->st_uid = stx.stx_uid;
	st->st_gid = stx.stx_gid;
	st->st_size = stx.stx_size;
	st->st_blksize = stx.stx_blksize;
	st->st_blocks = stx.stx_blocks;

	st->st_atim.tv_sec = stx.stx_atime.tv_sec;
	st->st_atim.tv_nsec = stx.stx_atime.tv_nsec;
	st->st_mtim.tv_sec = stx.stx_mtime.tv_sec;
	st->st_mtim.tv_nsec = stx.stx_mtime.tv_nsec;
	st->st_ctim.tv_sec = stx.stx_ctime.tv_sec;
	st->st_ctim.tv_nsec = stx.stx_ctime.tv_nsec;
    } else if (errno==ENOSYS || errno==EINVAL)
        return fstat(fd, st);
    return ret;
}

int lstatn(const char *pathname, unsigned int mask, struct stat *st)
{
    int flags = AT_SYMLINK_NOFOLLOW|stat_flags;
    int dirfd = pathname && *pathname == '/' ? 0 : AT_FDCWD;
    int ret;
    struct statx stx;

#ifndef HAVE_STATX
    ret = syscall(SYS_statx, dirfd, pathname, flags, mask, &stx);
#else
    ret = statx(dirfd, pathname, flags, mask, &stx);
#endif
    if (ret >= 0) {
	st->st_dev  = makedev(stx.stx_dev_major,  stx.stx_dev_minor);
	st->st_rdev = makedev(stx.stx_rdev_major, stx.stx_rdev_minor);

	st->st_ino  = stx.stx_ino;
	st->st_mode = stx.stx_mode;
	st->st_nlink = stx.stx_nlink;
	st->st_uid = stx.stx_uid;
	st->st_gid = stx.stx_gid;
	st->st_size = stx.stx_size;
	st->st_blksize = stx.stx_blksize;
	st->st_blocks = stx.stx_blocks;

	st->st_atim.tv_sec = stx.stx_atime.tv_sec;
	st->st_atim.tv_nsec = stx.stx_atime.tv_nsec;
	st->st_mtim.tv_sec = stx.stx_mtime.tv_sec;
	st->st_mtim.tv_nsec = stx.stx_mtime.tv_nsec;
	st->st_ctim.tv_sec = stx.stx_ctime.tv_sec;
	st->st_ctim.tv_nsec = stx.stx_ctime.tv_nsec;
    } else if (errno==ENOSYS || errno==EINVAL)
        return lstat(pathname, st);
    return ret;
}
#endif /* WITH_STATX */
