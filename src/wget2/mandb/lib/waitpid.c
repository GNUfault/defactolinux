/* waitpid -- emulate waitpid with wait3
   Copyright (C) 1995 Carl Edman

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

int waitpid(int pid, int *statusp, int options)
   {
   if (pid==-1 || pid==0) return wait3((union wait *)statusp,options,0);
   return wait4(pid>0 ? pid : -pid,(union wait *)statusp,options,0);
   }
